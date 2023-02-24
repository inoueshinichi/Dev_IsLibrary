/**
 * @file linalg.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

#include <IsCommon/internal/defs.hpp>
#include <IsCommon/internal/exception.hpp>
#include <IsCommon/internal/math.hpp>

namespace is
{
    namespace common
    {
         namespace linalg
        {
            template <typename T>
            Matrix3<T> inv(const Matrix3<T> &mat) { IS_ERROR(error_code::not_implemented, "Use Eigen"); }

            template <typename T>
            double det(const Matrix3<T> &mat) { IS_ERROR(error_code::not_implemented, "Use Eigen"); }

            template <typename T>
            int rank(const Matrix3<T> &mat) { IS_ERROR(error_code::not_implemented, "Use Eigen") }


            template <typename T>
            Matrix4<T> inv(const Matrix4<T> &mat) { IS_ERROR(error_code::not_implemented, "Use Eigen"); }

            template <typename T>
            double det(const Matrix4<T> &mat) { IS_ERROR(error_code::not_implemented, "Use Eigen"); }

            template <typename T>
            int rank(const Matrix4<T> &mat) { IS_ERROR(error_code::not_implemented, "Use Eigen") }

        } // namespace linalg
    }
}