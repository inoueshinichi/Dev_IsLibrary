#pragma once

#include <IsNdArray/IsNdArray.hpp>
#include <IsNdArray/backend_base.hpp>
#include <IsNdArray/context.hpp>


#include <functional>
#include <istream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

namespace is
{
    namespace nbla
    {
        using std::string;
        using std::unordered_map;
        using std::shared_ptr;

        /**
         * @brief BackendUitls Class
         * This class is never be instantiated.
         * Cpuクラス or Gpuクラス
         */
        class NBLA_API BackendUtils
        {
        public:
            using BackendGetter = std::function<BackendBase*(void)>;
            using Registry_t = unordered_map<string, BackendGetter>;

            /**
             * @brief Register new synchronizer
             * 
             * @param backend_name 
             * @param backend_getter 
             */
            static void add_backend(const string& backend_name,
                                    BackendGetter backend_getter);

            
            /**
             * @brief Call array_classes of the backend in a context.
             * 
             * @param ctx 
             * @return vector<string> 
             */
            static vector<string> array_classes(const Context& ctx);


            /**
             * @brief Call _set_array_classes of the backend in a context.
             * @note Dangerous to call. End users shouldn't call.
             * 
             * @param ctx 
             * @param a 
             */
            static void _set_array_classes(const Context& ctx, const vector<string>& a);


            /**
             * @brief Call register_array_class of the backend in a context.
             * 
             * @param ctx 
             * @param name 
             */
            static void register_array_class(const Context& ctx, const string& name);


            /**
             * @brief Call caching_allocator of the backend in a context.
             * 
             * @param ctx 
             * @return shared_ptr<Allocator> 
             */
            static shared_ptr<Allocator> caching_allocator(const Context& ctx);


            /**
             * @brief Call native_allocator of the backend in a context.
             * 
             * @param ctx 
             * @return shared_ptr<Allocator> 
             */
            static shared_ptr<Allocator> native_allocator(const Context& ctx);


            /**
             * @brief Free all unused host memory caches
             * 
             * @param ctx 
             */
            static void free_unused_host_caches(const Context& ctx);


            /**
             * @brief Call device_synchronize of the backend in a context.
             * 
             * @param ctx 
             */
            static void device_synchronize(const Context& ctx);


            /**
             * @brief Synchronize host to default stream of device.
             * 
             * @param ctx 
             */
            static void default_stream_synchronize(const Context& ctx);


            /**
             * @brief Call create_lms_streams of the backend in a context.
             * 
             * @param ctx 
             */
            static void create_lms_streams(const Context& ctx);

        private:
            // Never be created
            inline BackendUtils() {}

            /**
             * @brief Get registry of creator function.
             * 
             * @return Registry_t& 
             */
            static Registry_t& get_registry();


            /**
             * @brief Get backend getter.
             * 
             * @param ctx 
             * @return BackendGetter 
             */
            static BackendGetter get_backend_getter(const Context& ctx);

        };

#define NBLA_REGISTER_BACKEND(BACKEND_NAME, BACKEND_GETTER)        \
    { BackendUtils::add_backend(BACKEND_NAME, BACKEND_GETTER); }
    
    }
}