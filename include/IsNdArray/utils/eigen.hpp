#pragma once

#define EIGEN_NO_DEBUG
#define EIGEN_MPL2_ONLY

#include <Eigen/Dense>

namespace is
{
    namespace nbla
    {
        namespace eigen
        {
            template <typename T>
            using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

            template <typename T>
            using RowVector = Eigen::Matrix<T, 1, Eigen::Dynamic>;

            template <typename T>
            using ColVector = Eigen::Matrix<T, Eigen::Dynamic, 1>;

            template <typename T>
            using MatrixMap = Eigen::Map<Matrix<T>>;

            template <typename T>
            using ConstMatrixMap = Eigen::Map<const Matrix<T>>;

            template <typename T> 
            using RowVectorMap = Eigen::Map<RowVector<T>>;

            template <typename T> 
            using ConstRowVectorMap = Eigen::Map<const RowVector<T>>;

            template <typename T> 
            using ColVectorMap = Eigen::Map<ColVector<T>>;

            template <typename T> 
            using ConstColVectorMap = Eigen::Map<const ColVector<T>>;

            // C OP A * B with all combination of transpose or non-transpose
            // OP is for example '=' and '+='.

            #define NBLA_EIGEN_MATMUL_T(C, Tc, A, Ta, B, Tb, OP)                    \
                if (Tc)                                                             \
                {                                                                   \
                    if (Ta)                                                         \
                    {                                                               \
                        if (Tb)                                                     \
                        {                                                           \
                            C.transpose() OP A.transpose() * B.transpose();         \
                        }                                                           \
                        else                                                        \
                        {                                                           \
                            C.transpose() OP A.transpose() * B;                     \
                        }                                                           \
                    }                                                               \
                    else                                                            \
                    {                                                               \
                        if (Tb)                                                     \
                        {                                                           \
                            C.transpose() OP A * B.transpose();                     \
                        }                                                           \
                        else                                                        \
                        {                                                           \
                            C.transpose() OP A * B;                                 \
                        }                                                           \
                    }                                                               \
                }                                                                   \
                else                                                                \
                {                                                                   \
                    if (Ta)                                                         \
                    {                                                               \
                        if (Tb)                                                     \
                        {                                                           \
                            C OP A.transpose() * B.transpose();                     \
                        }                                                           \
                        else                                                        \
                        {                                                           \
                            C OP A.transpose() * B;                                 \
                        }                                                           \
                    }                                                               \
                    else                                                            \
                    {                                                               \
                        if (Tb)                                                     \
                        {                                                           \
                            C OP A * B.transpose();                                 \
                        }                                                           \
                        else                                                        \
                        {                                                           \
                            C OP A * B;                                             \
                        }                                                           \
                    }                                                               \
                }
        }   
    }
}