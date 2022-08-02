#pragma once

#include <IsNdArray/IsNdArray.hpp>
#include <IsNdArray/backend_base.hpp>
#include <IsNdArray/memory/allocator.hpp>
#include <IsNdArray/singleton_manager.hpp>

#include <memory>
#include <string>

namespace is
{
    namespace nbla
    {
        using std::vector;
		using std::string;
		using std::shared_ptr;

        /**
         * @brief CPU計算のためのいくつかのハンドルと設定を保持するシングルトンクラス
         * 1) ~Cpu()はprivateにする
         * 2) SingletonManagerをfriendにする
         */
        class NBLA_API Cpu : public BackendBase
        {
        public:
            ~Cpu();

            /**
             * @brief CPU上での関数実装で使用できる配列クラスのリスト
             * 
             * @return vector<string> 
             */
            virtual vector<string> array_classes() const override;


            /**
             * @brief  配列クラスを設定
             * エンドユーザーは呼び出してはいけない
             * @param a 
             */
            virtual void _set_array_classes(const vector<string>& a) override;


            /**
             * @brief 名前によって使用可能な配列クラスを登録
             * 
             * @param name 
             */
            virtual void register_array_class(const string& name) override;


            /**
             * @brief cachingアロケータを取得
             * 
             * @return shared_ptr<Allocator> 
             */
            virtual shared_ptr<Allocator> caching_allocator() override;


            /**
             * @brief nativeアロケータを取得
             * 
             * @return shared_ptr<Allocator> 
             */
            virtual shared_ptr<Allocator> native_allocator() override;


            /**
             * @brief Free all unused host memory caches
             * 
             */
            virtual void free_unused_host_caches() override;


            /**
             * @brief Synchronize host to device.
             * 
             * @param device 
             */
            virtual void device_synchronize(const string &device) override;


            /**
             * @brief Synchronize host to default stream of device.
             * 
             * @param device 
             */
            virtual void default_stream_synchronize(const string &device) override;


            /**
             * @brief Create non blockuing streams for data transfer.
             * Noting to do in CPU backend.
             * @param device 
             */
            virtual void create_lms_streams(int device = -1) override {}


        protected:
            vector<string> array_classes_; // Available array classes

            /*
				AllocatorMemoryのデストラクタが呼ばれる前にAllocatorのデストラクタが呼ばれるのを
				防ぐことを目的に、AllocatorMemoryクラスにAllocatorのポインタをメンバ変数にする.
				そのため、Allocatorはshared_ptrにする必要がある.
			*/
			shared_ptr<Allocator> native_allocator_;
			shared_ptr<Allocator> caching_allocator_;


        private:
            friend SingletonManager;

            // ユーザーによってCpuコンストラクタは呼ばれない
			Cpu();

            DISABLE_COPY_AND_ASSIGN(Cpu)
        };
    }
}