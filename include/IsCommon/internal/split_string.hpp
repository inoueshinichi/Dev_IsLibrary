/**
 * @file split_string.hpp
 * @author your name (you@domain.com)
 * @brief 文字列の分割
 * @version 0.1
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <IsCommon/internal/defs.hpp>

#include <string>
#include <vector>

namespace is {
    namespace common {

        /**
         * @brief split関数
         * https://marycore.jp/prog/cpp/std-string-split/
         * @param msg 
         * @param sep 
         * @return std::vector<std::string> 
         */
        inline std::vector<std::string> 
        split_string(const std::string &msg, const std::string &sep) {
            uint64 len_sep = sep.length();
            std::vector<std::string> tokens;

            if (len_sep == 0) {
                tokens.push_back(msg);
            }
            else {
                auto offset = std::string::size_type(0);
                while (true) {
                    auto pos = msg.find(sep, offset);
                    if (pos == std::string::npos) {
                        tokens.push_back(msg.substr(offset));
                        break;
                    }
                    tokens.push_back(msg.substr(offset, pos - offset));
                    offset = pos + len_sep;
                }
            }

            return tokens;
        }
    }
}