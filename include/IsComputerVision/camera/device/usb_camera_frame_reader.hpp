#pragma once

#include <IsComputerVision/camera/camera_frame_reader.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

namespace is 
{
    namespace cv 
    {
        /**
         * @brief USBカメラフレーム取り込みクラス
         * 
         */
        class IS_CV_API UsbCameraFrameReader : public CameraFrameReader 
        {
        private:
            class ::cv::VideoCapture* capPtr_;
            class ::cv::Mat* cvFramePtr_;

        public:
            UsbCameraFrameReader();
            virtual ~UsbCameraFrameReader();

            std::string getClassName() const override final { return "UsbCameraFrameReader"; }

        protected:
            bool initializeImpl() override final;
            void releaseImpl() override final;
            bool captureImpl() override final;
        };

    }
}