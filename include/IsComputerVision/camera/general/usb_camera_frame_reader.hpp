#pragma once

#include <IsComputerVision/IsComputerVision.hpp>
#include <IsComputerVision/camera/camera_frame_reader.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

/* OpenCV Lib Config for Windows */
#ifdef WIN32
    #ifdef _DEBUG
        #pragma comment(lib,"opencv_world460d.lib")
    #else
        #pragma comment(lib, "opencv_world460.lib")
    #endif
#endif

namespace is 
{
    namespace imgproc 
    {
        /**
         * @brief USBカメラフレーム取り込みクラス
         * 
         */
        class IS_CV_API UsbCameraFrameReader : public CameraFrameReader 
        {
            cv::VideoCapture* capPtr_;
            cv::Mat cvFrame;

        public:
            UsbCameraFrameReader();
            virtual ~UsbCameraFrameReader();

        protected:
            bool initializeImpl() override final;
            void releaseImpl() override final;
            bool captureImpl() override final;
        };

    }
}