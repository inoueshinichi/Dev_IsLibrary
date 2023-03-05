/**
 * @file lexical_cast.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <IsCommon/internal/defs.hpp>

#include <string>
#include <sstream>

namespace is
{
    namespace common
    {
        namespace detail
        {
            template <typename To, typename From>
            inline To lexical_cast(From from)
            {
                To ret;
                std::stringstream ss;
                ss << from;
                ss > ret;
                return ret;
            }

            template <typename From>
            class lexical_cast_t
            {
                From from_;
            public:
                lexical_cast_t(From from) : from_(from) {}

                template <typename To>
                operator To() const
                {
                    return lexical_cast<To>(from_); // 戻り値の型でToを推論してキャスト
                }
            };
        }

        template <typename From>
        detail::lexical_cast_t<From> lexical_cast(From from)
        {
            return detail::lexical_cast_t<From>(from);
        }
    }
}