#include <IsComputerVision/camera/device/usb_camera_frame_reader.hpp>

/* OpenCV Lib Config for Windows */
#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "opencv_world460d.lib")
#else
#pragma comment(lib, "opencv_world460.lib")
#endif
#endif

namespace is 
{
    namespace cv 
    {
        // -------------------------- UsbCameraFrameReader --------------------------

        //////////////////////////////////////////////////////////
        // ctor/dtor
        //////////////////////////////////////////////////////////
        UsbCameraFrameReader::UsbCameraFrameReader() 
            : CameraFrameReader() {}


        UsbCameraFrameReader::~UsbCameraFrameReader() 
        {
            CameraFrameReader::release();
            IS_DEBUG_LOG("Dtor of UsbCameraFrameReader\n");
        }


        //////////////////////////////////////////////////////////
        // private method
        //////////////////////////////////////////////////////////
        bool UsbCameraFrameReader::initializeImpl() 
        {
            if (isInitialized_) 
            {
                IS_DEBUG_LOG("Already initialized.\n");
                return false;
            }

            if (CameraFrameReader::getDeviceId() < 0) 
            {
                IS_DEBUG_LOG("Please set device id.\n");
                return false;
            }

            IS_DEBUG_LOG("[Enter] >>> Initialize\n");
            IS_DEBUG_LOG("Device Id: %d\n", CameraFrameReader::getDeviceId());

            capPtr_ = new ::cv::VideoCapture(CameraFrameReader::getDeviceId()); //, cv::CAP_GSTREAMER); // cv::CAP_V4L2 cv::CAP_GSTREAMER
            cvFramePtr_ = new ::cv::Mat();
            
            if (!capPtr_->isOpened()) 
            {
                IS_DEBUG_LOG("[Error] Can not open cv::VideoCapture.\n");
                IS_DEBUG_LOG("[Exit] <<< Initialize\n");
                delete capPtr_; capPtr_ = nullptr;
                delete cvFramePtr_; cvFramePtr_ = nullptr;
                return false;
            }

            IS_DEBUG_LOG("Success to initialize usb camera with device id: %d\n", CameraFrameReader::getDeviceId());

            // clangだとなぜかgetBackendName()関数が未定義.
            // std::string backApiStr = capPtr_->getBackendName();
            // IS_DEBUG_STREAM("Background api: %s\n", backApiStr.c_str());

            // GStreamerだと設定が反映されない仕様らしい
            // https://qiita.com/iwatake2222/items/b8c442a9ec0406883950
            // capPtr_->set(cv::CAP_PROP_FRAME_WIDTH, 640);
            // capPtr_->set(cv::CAP_PROP_FRAME_HEIGHT, 480);

            // capPtr_->set(cv::CAP_PROP_FPS, 30);

            auto width = capPtr_->get(::cv::CAP_PROP_FRAME_WIDTH);
            auto height = capPtr_->get(::cv::CAP_PROP_FRAME_HEIGHT);
            auto fps = capPtr_->get(::cv::CAP_PROP_FPS);
            IS_DEBUG_LOG("Fps: %f, Width: %d, Height: %d\n", fps, (int)width, (int)height);

            // capPtr_->set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('H', '2', '6', '4'));
            // capPtr_->set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
            char forcc[4];
            int symbol = (int)capPtr_->get(::cv::CAP_PROP_FOURCC);
            for (int i = 0; i < 4; ++i) {
                forcc[i] = (char)(symbol >> 8 * i) & 0xff;
            }
            IS_DEBUG_LOG("FourCC: %c%c%c%c\n", forcc[0], forcc[1], forcc[2], forcc[3]);

            if (capPtr_->read(*cvFramePtr_))
            {
                width_ = cvFramePtr_->cols;
                height_ = cvFramePtr_->rows;
                channels_ = cvFramePtr_->channels();
                memSizePerLine_ = cvFramePtr_->step;
                memDataSize_ = height_ * memSizePerLine_;
                IS_DEBUG_LOG("Retrieve frame: (H, W, C) = (%d, %d, %d)\n", height_, width_, channels_);

                // shape, strides, ndim
                shape_ = { height_, width_, channels_ };
                ndim_ = shape_.size();
                strides_.assign(ndim_, 1);
                for (int i = strides_.size() - 1; i > 0; --i) {
                    strides_[i-1] = strides_[i] * shape_[i];
                }
                
                // Frame画像のメモリ確保
                framePtr_ = (ubyte *)std::malloc(memDataSize_);
                std::memset(framePtr_, 0, memDataSize_);
                IS_DEBUG_LOG("Malloc framePtr_. Address: %p. Bytesize: %ld\n",
                             (void *)framePtr_, memDataSize_);
            }
            else {
                IS_DEBUG_LOG("[Error] Can not read frame.\n");
                IS_DEBUG_LOG("[Exit] <<< Initialize\n");
                delete capPtr_; capPtr_ = nullptr;
                return false;
            }

            isInitialized_ = true; // 初期化完了フラグ

            IS_DEBUG_LOG("[Exit] <<< Initialize\n");
            return true;
        }


        //////////////////////////////////////////////////////////
        // protected method
        //////////////////////////////////////////////////////////

        void UsbCameraFrameReader::releaseImpl() {
            if (isInitialized_) {
                IS_DEBUG_LOG("[Enter] >>> Release\n");
                capPtr_->release(); // OpenCV release
                delete capPtr_; capPtr_ = nullptr;
                delete cvFramePtr_; cvFramePtr_ = nullptr;
                isInitialized_ = false;
                std::free(framePtr_); framePtr_ = nullptr;
                IS_DEBUG_LOG("Success to release.\n");
                IS_DEBUG_LOG("[Exit] <<< Release\n");
            }
        }


        bool UsbCameraFrameReader::captureImpl() {

            // auto start = high_resolution_clock::now();
            bool bret = capPtr_->read(*cvFramePtr_);
            // auto end = high_resolution_clock::now();
            // auto msec = duration_cast<milliseconds>(end - start).count();
            // std::printf("Read elapsed time: %ld[ms]\n", msec);
            
            if (bret) {

                if (channels_ == 3) {
                    ::cv::cvtColor(*cvFramePtr_, *cvFramePtr_, ::cv::COLOR_BGR2RGB);
                }
                else if (channels_ == 4) {
                    ::cv::cvtColor(*cvFramePtr_, *cvFramePtr_, ::cv::COLOR_BGRA2RGBA);
                }

                if (cvFramePtr_->isContinuous())
                {
                    // 連続メモリ

                    // deepcopy
                    std::memcpy(framePtr_, cvFramePtr_->data, memDataSize_);
                }
                else {
                    // 非連続メモリ(各行間にギャップがある)
                    size_t memLine = 0;
                    for (int y = 0; y < height_; ++y) {
                        // 行単位でdeepcopy
                        std::memcpy((void *)&framePtr_[memLine],
                                    (void *)cvFramePtr_->row(y).data, memSizePerLine_);
                        memLine += memSizePerLine_;
                    }
                }
            }

            return bret;
        }

        //////////////////////////////////////////////////////////
        // public method
        //////////////////////////////////////////////////////////

        
    }
}