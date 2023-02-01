#pragma once

#include <IsComputerVision/IsComputerVision.hpp>

namespace is 
{
    namespace cv 
    {
        using std::mutex;
        using std::vector;
        using std::tuple;
        using std::promise;
        using std::function;
        using std::enable_shared_from_this;

        /**
         * @brief カメラフレーム読み取りの抽象インターフェースクラス
         */
        class IS_CV_API CameraFrameReader : public enable_shared_from_this<CameraFrameReader>
        {
        public:
            using FrameDesc = tuple<vector<ubyte>, float>; // Image, Fps
            using Shape_t = vector<int>;

        protected:
            // 画像
            ubyte *framePtr_ {nullptr}; // (H, W, C)
            int width_;
            int height_;
            int channels_;
            size_t memSizePerLine_;
            size_t memDataSize_;
            
            Shape_t shape_;
            Shape_t strides_;
            int ndim_;

            // 制御
            mutable mutex mtx_;
            bool isInitialized_ {false};
            bool isRunning_ {false};
            int deviceId_;
            int delay_; // ms
            float fps_;
            std::chrono::high_resolution_clock::time_point ticks_;

        private : 
            void spin();

        public:
            

            CameraFrameReader();
            virtual ~CameraFrameReader() {}

            virtual std::string getClassName() const { "CameraFrameReader"; }

            int width() const { return width_; }
            int height() const  { return height_; }
            int channels() const  { return channels_; }
            size_t memSizePerLine() const  { return memSizePerLine_; }
            size_t memDataSize() const  { return memDataSize_; }

            Shape_t shape() const { return shape_; }
            Shape_t strides() const { return strides_; }
            int ndim() const { return ndim_; }

            bool isRunning() const  { return isRunning_; }

            void setDeviceId(int deviceId) { deviceId_ = deviceId; }
            int getDeviceId() const  { return deviceId_; }

            void setDelay(int delay) { delay_ = delay; }
            int getDelay() const  { return delay_; }

            void stop() { isRunning_ = false; }
            void start(promise<int> result); // フレーム読み込みループ
            function<void(promise<int>)> wrapedStart();

            FrameDesc retrieveFrame() const;

            bool initialize();
            void release();
            bool capture();

        protected:
            virtual bool initializeImpl() = 0;
            virtual void releaseImpl() = 0;
            virtual bool captureImpl() = 0;
        };
    }
}

