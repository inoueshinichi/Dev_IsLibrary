#pragma once

#include <IsNdArray/IsNdArray.hpp>

#include <string>
#include <vector>

namespace is
{
    namespace nbla
    {
        using std::string;
        using std::vector;

        /**
         * @brief NNabla CPU features.
         * 
         * @note User usually does not need to call this function manually.
         *       ※一部を抽出して利用したいので、手動で記述した. 
         */
        NBLA_API void init_cpu();

        /**
         * @brief Clear CPU memory cache.
         * 
         */
        NBLA_API void clear_cpu_memory_cache(); 


        /**
         * @brief Print CPU memory cache map.
         * 
         */
        NBLA_API void print_cpu_memory_cache_map();


        /**
         * @brief Get CPU array classes.
         * 
         * @return vector<string> 
         */
        NBLA_API vector<string> cpu_array_classes();


        /**
         * @brief Set CPU array classes
         * 
         * @param a 
         */
        NBLA_API void _cpu_set_array_classes(const vector<string>& a);


        /**
         * @brief 
         * 
         * @param device 
         */
        NBLA_API void cpu_device_synchronize(const string& device);


        /**
         * @brief 
         * 
         */
        NBLA_API int cpu_get_device_count();


        /**
         * @brief 
         * 
         */
        NBLA_API vector<string> cpu_get_devices();
    }
}