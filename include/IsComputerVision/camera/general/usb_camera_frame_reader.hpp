#pragma once

#include <IsComputerVision/IsComputerVision.hpp>
#include <IsComputerVision/camera/camera_frame_reader.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

namespace is 
{
    namespace imgproc 
    {
        /**
         * @brief USBカメラフレーム取り込みクラス
         * 
         */
        class IS_IMGPROC_API UsbCameraFrameReader : public CameraFrameReader 
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