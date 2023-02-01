/**
 * @file join_string.hpp
 * @author your name (you@domain.com)
 * @brief 文字列の連結
 * @version 0.1
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace is {
    namespace common {

        /**
         * @brief Pythonの" ".join(['A', 'B', 'C'])と同じ文字連結処理
         * 
         * @tparam T 
         * @param vector 
         * @param delimiter 
         * @return string 
         */
        template <typename T>
        inline std::string join_string(const std::vector<T> &vector, 
                                        const std::string &delimiter) {
            std::stringstream ss;
            if (vector.empty()) {
                return "";
            }

            for (int32 i = 0; i < vector.size() - 1; ++i) {
                ss << vector[i] << delimiter;
            }
            ss << vector[vector.size() - 1];
            return ss.str();
        }
    }
}