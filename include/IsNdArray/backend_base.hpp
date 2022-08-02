#pragma once

#include <IsNdArray/common.hpp>
#include <IsNdArray/memory/allocator.hpp>
#include <IsNdArray/singleton_manager.hpp>

#include <memory>
#include <string>
#include <vector>

namespace is
{
    namespace nbla
    {
        using std::vector;
        using std::string;
        using std::shared_ptr;

        /**
        Singletonクラスのインターフェースクラス
        */
        class NBLA_API BackendBase
        {
        public:
            virtual ~BackendBase() {}

            /**
             * @brief Available array class list used in CPU Function implementations.
             * 
             * @return vector<string> 
             */
            virtual vector<string> array_classes() const = 0;


            /**
             * @brief Set array class list.
             * 
             * @param a 
             */
            virtual void _set_array_classes(const vector<string> &a) = 0;


           /**
            * @brief Register array class to available list by name.
            * 
            * @param name 
            */
            virtual void register_array_class(const string &name) = 0;


            /**
             * @brief Get a caching allocator.
             * 
             * @return shared_ptr<Allocator> 
             */
            virtual shared_ptr<Allocator> caching_allocator() = 0;


            /**
             * @brief Get a no-cache allocator.
             * 
             * @return shared_ptr<Allocator> 
             */
            virtual shared_ptr<Allocator> native_allocator() = 0;


            /**
             * @brief Free all unused host memory caches
             * 
             */
            virtual void free_unused_host_caches() = 0;


            /**
             * @brief Synchronize host to device.
             * 
             * @param device 
             */
            virtual void device_synchronize(const string &device) = 0;


            /**
             * @brief Synchronize host to default stream of device.
             * 
             * @param device 
             */
            virtual void default_stream_synchronize(const string &device) = 0;


            /**
             * @brief Create non blockuing streams for data transfer
             * 
             * @param device 
             */
            virtual void create_lms_streams(int device = -1) = 0;

        protected:
            vector<string> array_classes_; // Available array classes

            /*
				AllocatorMemoryのデストラクタが呼ばれる前にAllocatorのデストラクタが呼ばれるのを
				防ぐことを目的に、AllocatorMemoryクラスにAllocatorのポインタをメンバ変数にする.
				そのため、Allocatorはshared_ptrにする必要がある.
			*/
			shared_ptr<Allocator> native_allocator_;
			shared_ptr<Allocator> caching_allocator_;

            BackendBase() {} // Never called by users.

        private:
            friend SingletonManager;
            DISABLE_COPY_AND_ASSIGN(BackendBase)
        };
    }
}