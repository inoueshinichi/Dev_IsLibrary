#pragma once

#include <IsComputerVision/IsComputerVision.hpp>
#include <IsComputerVision/camera/general/usb_camera_frame_reader.hpp>

#include <string>
#include <thread>
#include <future>

namespace is 
{
    namespace imgproc 
    {
        using std::string;
        using std::vector;
        using std::thread;
        using std::future;

        /**
         * @brief カメラのコントローラ
         * 
         */
        class IS_CV_API CameraController 
        {
            CameraFrameReader* frameReader_ {nullptr};
            thread* workerThread_ {nullptr};
            future<int> result_;

        public:
            CameraController();
            virtual ~CameraController();

            CameraController(const CameraController&) = delete;
            CameraController& operator=(const CameraController&) = delete;
            CameraController(CameraController&&) = default;
            CameraController& operator=(CameraController&&) = default;

            int width() const;
            int height() const;
            int channels() const;
            size_t memSizePerLine() const;
            size_t memDataSize() const;
            int ndim() const;
            vector<int> shape() const;
            vector<int> strides() const;

            void setCameraType(const string& maker = "general", const string& type = "usb");
            bool startCamera(int deviceId, int delay);
            void stopCamera();
            CameraFrameReader::FrameDesc fetchFrame();
            
        };
    }
}