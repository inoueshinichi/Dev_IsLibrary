#include <IsComputerVision/camera/camera_frame_reader.hpp>

#include <cstdio>
#include <cstring>

using namespace std::chrono;

namespace is 
{
    namespace cv 
    {
        // -------------------------- CameraFrameReader --------------------------

        //////////////////////////////////////////////////////////
        // ctor/dtor
        //////////////////////////////////////////////////////////
        CameraFrameReader::CameraFrameReader()
            : deviceId_(-1)
            , delay_(0)
            , fps_(0)
            , width_(0)
            , height_(0)
            , channels_(0)
            , memSizePerLine_(0)
            , memDataSize_(0) 
        {}

        //////////////////////////////////////////////////////////
        // private method
        //////////////////////////////////////////////////////////
        void CameraFrameReader::spin() 
        {
            using std::chrono::high_resolution_clock;
            // std::this_thread::sleep_for(milliseconds(delay_));

            if (delay_ > 0) {
                auto waitTime = ticks_ + milliseconds(delay_);
                std::this_thread::sleep_until(waitTime);
            }

            // while (high_resolution_clock::now() < abs_time) {
            //     ; // busy loop
            // }


            // auto abs_time = tp_base + milliseconds(delay_);
            // std::cout << "tp_start " << tp_start.time_since_epoch().count() << std::endl;
            // std::cout << "abs_time " << abs_time.time_since_epoch().count() << std::endl;
            // auto t_abs = abs_time.time_since_epoch().count();

            // for (;;) {
            //     auto t_now = high_resolution_clock::now().time_since_epoch().count();
            //     std::cout << "t_now " << t_now << " : t_abs" << t_abs << std::endl;
            //     if (t_now > t_abs)
            //         break;
            // }

            // while (high_resolution_clock::now().time_since_epoch().count() < abs_time.time_since_epoch().count()) {
            //     std::printf("busy loop\n");
            // }
        }

        //////////////////////////////////////////////////////////
        // protected method
        //////////////////////////////////////////////////////////


        //////////////////////////////////////////////////////////
        // public method
        //////////////////////////////////////////////////////////
        bool CameraFrameReader::initialize() { return this->initializeImpl(); }
        void CameraFrameReader::release() { this->releaseImpl(); }
        bool CameraFrameReader::capture() { return this->captureImpl(); }

        void CameraFrameReader::start(std::promise<int> result) 
        {
            /* ワーカースレッド側で処理される */
            using std::chrono::high_resolution_clock;

            if (!isInitialized_) 
            {
                IS_DEBUG_LOG("No initialization.\n");
                return;
            }

            bool status;
            isRunning_ = true;
            high_resolution_clock::time_point tp_start;
            high_resolution_clock::time_point tp_end;

            try 
            {
                while (isRunning_) 
                {
                    tp_start = high_resolution_clock::now();

                    // クリティカルセクション
                    {
                        
                        std::lock_guard<std::mutex> locker(mtx_);
                        status = capture();
                    }
                    
                    if (!status) 
                    {
                        IS_DEBUG_LOG("Miss frame!\n");
                    }

                    spin(); // カメラのフレームレートより遅いdelayの場合、間隔を揃える.
            
                    tp_end = high_resolution_clock::now();
                    auto msec = duration_cast<milliseconds>(tp_end - tp_start).count();
                    fps_ = 1000.0f / msec;

                    IS_DEBUG_LOG("CaptureLoop: %ld[ms] Fps: %f\n", msec, fps_);
                }
                result.set_value(0);
            }
            catch (...) 
            {
                // 全例外を捕まえて promiseにセットする
                result.set_exception(std::current_exception());
            }
        }

        std::function<void(std::promise<int>)> CameraFrameReader::wrapedStart() 
        {
            return [this](std::promise<int> result) -> void {
                this->start(std::move(result));
            };
        }

        CameraFrameReader::FrameDesc CameraFrameReader::retrieveFrame() const 
        {
            // クリティカルセクション
            std::lock_guard<std::mutex> locker(mtx_);

            std::vector<ubyte> pickupImgArray(memDataSize_, 0); 
            float fps;

            // deepcopy
            std::memcpy(pickupImgArray.data(), framePtr_, memDataSize_);
            fps = fps_; 

            return std::make_tuple(pickupImgArray, fps);
        }
    }
}
