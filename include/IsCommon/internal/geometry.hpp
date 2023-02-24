/**
 * @file geometry.hpp
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
        namespace geometry
        {
            const float pi = 3.1415926535f;
            const float two_pi = pi * 2.0f;
            const float pi_over_2 = pi / 2.0f;
            const float inf = std::numeric_limits<float>::infinity();
            const float ng_inf = -inf;

            inline float to_radians(float degrees)
            {
                return degrees * pi / 180.0f;
            }
    
            inline float to_degrees(float radians)
            {
                return radians * 180.0f / pi;
            }

            inline bool near_zero(float val, float epsilon = 0.0001f)
            {
                if (fabs(val) <= epsilon)
                    return true;
                else
                    return false;
            }

            template <typename T>
            T max(const T &a, const T &b)
            {
                return (a < b ? b : a);
            }

            template <typename T>
            T min(const T &a, const T &b)
            {
                return (a < b ? a : b);
            }

            template <typename T>
            T clamp(const T &value, const T &lower, const T &upper)
            {
                return min(upper, max(lower, value));
            }

            inline float abs(float val) { return fabs(val); }
            inline float cos(float val) { return cosf(val); }
            inline float sin(float val) { return sinf(val); }
            inline float tan(float val) { return tanf(val); }
            inline float acos(float val) { return acosf(val); }
            inline float asin(float val) { return asinf(val); }
            inline float atan2(float y, float x) { return atan2f(y, x); }
            inline float cot(float val) { return 1.0f / tan(val); }
            inline float sqrt(float val) { return sqrtf(val); }
            inline float fmod(float numer, float denom) { return fmod(numer, denom); }
            inline float lerp(float a, float b, float f) { return a + f * (a - b); }


            using Vector2 = math::Vector2<float>;
            using Vector3 = math::Vector3<float>;
            using Vector4 = math::Vector4<float>;
            using Matrix2 = math::Matrix2<float>;
            using Matrix3 = math::Matrix3<float>;
            using Matrix4 = math::Matrix4<float>;

            template <typename T>
            Vector2<T> lerp(const Vector2<T> &a, const Vector2<T> &b, double f)
            {
                return Vector2<T>(a + (T)f * (b - a));
            }

            template <typename T>
            Vector2<T> reflect(const Vector2<T> &v, const Vector2<T> &n)
            {
                return v - (T)2.0 * (T)Vector2::dot(v, n) * n;
            }

            template <typename T>
            Vector2<T> transform(const Vector2<T> &vec, const Matrix3<T> &mat, double w = 1.0)
            {
                IS_ERROR(error_code::not_implemented, "Use Eigen");
            }

            template <typename T>
            Vector3<T> cross(const Vector3<T> &a, const Vector3<T> &b)
            {
                Vector3<T> ret;
                ret.x_ = a.y_ * b.z_ - a.z_ * b.y_;
                ret.y_ = a.z_ * b.x_ - a.x_ * b.z_;
                ret.z_ = a.x_ * b.y_ - a.y_ * b.x_;
                return ret;
            }

            template <typename T>
            Vector3<T> lerp(const Vector3<T> &a, const Vector3<T> &b, double f)
            {
                return Vector3<T>(a + (T)f * (b - a));
            }

            template <typename T>
            Vector3<T> reflect(const Vector3<T> &v, const Vector3<T> &n)
            {
                return v - (T)2.0 * Vector3<T>::dot(v, n) * n;
            }

            template <typename T> class Quaternion;

            template <typename T>
            Vector3<T> transform(const Vector3<T> &vec, const Matrix4<T> &mat, double w = 1.0)
            {
                IS_ERROR(error_code::not_implemented, "Use Eigen");
            }

            template <typename T>
            Vector3<T> transform_with_persp_div(const Vector3<T> &vec, const Matrix4<T> mat, double w = 1.0)
            {
                IS_ERROR(error_code::not_implemented, "Use Eigen");
            }

            template <typename T>
            Vector3<T> transform(const Vector3<T> &vec, const Quaternion<T> &q)
            {
                IS_ERROR(error_code::not_implemented, "Use Eigen");
            }

            template <typename T>
            Matrix3<T> create_translation(const Vector2<T> &trans)
            {
                /* 数式と同じ列優先表記*/
                T tmp[3][3] = {
                    {(T)1.0, (T)0.0,  trans.x_},
                    {(T)0.0, (T)1.0, trans.y_},
                    {(T)0.0, (T)0.0, (T)1.0},
                };

                return Matrix3<T>(tmp);
            }

            template <typename T>
            Matrix3<T> create_scale(double sx, double sy)
            {
                return Matrix3<T>::diag((T)sx, (T)sy, (T)1.0);
            }

            template <typename T>
            Matrix3<T> create_scale(const Vector2<T> &sv)
            {
                return Matrix3<T>::diag(sv.x_, sv.y_, (T)1.0);
            }

            template <typename T>
            Matrix3<T> create_rotation(double theta)
            {
                /* 数式と同じ列優先表記*/
                T tmp[3][3] = {
                    { (T)std::cos(theta), -(T)std::sin(theta), (T)0.0},
                    { (T)std::sin(theta), (T)std::cos(theta), (T)0.0},
                    {(T)0.0, (T)0.0, (T)1.0},
                };

                return Matrix3<T>(tmp);
            }


            template <typename T>
            Vector2<T> axis_x_as_poture(const Matrix3<T> &mat)
            {
                // ワールド座標系に対するローカル座標のX軸を抽出
                /* 数式と同じ列優先表記*/

                /*
                    数学表記は
                        [xv1, yv1, X]
                        [xv2, yv2, Y]
                        [0, 0, 1]
                    vx = (xv1, xv2)
                */
                return Vector2<T>::normalize(Vector2<T>(mat[0][0], mat[1][0]]));
            }

            template <typename T>
            Vector2<T> axis_y_as_poture(const Matrix3<T> &mat)
            {
                // ワールド座標系に対するローカル座標系のY軸を抽出
                /* 数式と同じ列優先表記*/

                /*
                    数学表記は
                        [xv1, yv1, X]
                        [xv2, yv2, Y]
                        [0, 0, 1]
                    vy = (yv1, yv2)
                */
                return Vector2<T>::normalize(Vector2<T>(mat[0][1], mat[1][1]));
            }

            template <typename T>
            Vector2<T> translation(const Matrix3<T> &mat)
            {
                /* 数式と同じ列優先表記*/
                /*
                    数学表記は
                        [xv1, yv1, X]
                        [xv2, yv2, Y]
                        [0, 0, 1]
                    trans = (X, Y)
                */
                return Vector2<T>(mat[0][2], mat[1][2]);
            }

            template <typename T>
            Vector2<T> scale(const Matrix3<T> &mat)
            {
                // スケールを取得
                /* 数式と同じ列優先表記*/

                /*
                数学表記は
                    [xv1, yv1, X]
                    [xv2, yv2, Y]
                    [0, 0, 1]
                scale = (sx, sy)
                sx = norm(vector(xv1, xv2))
                sy = norm(vector(yv1, yv2))
            */
                Vector2<T> ret;
                ret.x_ = Vector2<T>(mat[0][0], mat[1][0]).norm();
                ret.y_ = Vector2<T>(mat[0][1], mat[1][1]).norm();
                return ret;
            }

            template <typename T>
            Matrix4<T> create_scale(double sx, double sy, double sz)
            {
                return Matrix4<T>::diag(sx, sy, sz, 1.0);
            }

            template <typename T>
            Matrix4<T> create_scale(const Vector3<T> &sv)
            {
                return Matrix4<T>::diag(sv.x_, sv.y_, sv.z_, 1.0);
            }

            template <typename T>
            Matrix4<T> create_rotation_x(double theta)
            {
                /* 列優先表記を使用
                    行列計算は A = B @ C のため回転行列の表記は
                        1,  0, 0, 0
                        0,  C, -S, 0
                        0,  S, 1, 0
                        0,  0, 0, 0
                */
                T tmp[4][4] = {
                    {(T)1.0, (T)0.0, (T)0.0, (T)0.0},
                    {(T)0.0, (T)std::cos(theta), -(T)std::sin(theta), (T)0.0},
                    {(T)0.0, (T)std::sin(theta), (T)std::cos(theta), (T)0.0},
                    {(T)0.0, (T)0.0, (T)0.0, (T)1.0},
                };

                return Matrix4<T>(tmp);
            }

            template <typename T>
            Matrix4<T> create_rotation_y(double theta)
            {
                /*  列優先表記を使用
                    行列計算は A = B @ C のため回転行列の表記は
                        C,  0, S, 0
                        0,  1, 0, 0
                        -S, 0, C, 0
                        0,  0, 0, 1
                */
                T tmp[4][4] = {
                    {(T)std::cos(theta), (T)0.0, (T)std::sin(theta), (T)0.0},
                    { (T)0.0, (T)1.0, (T)0.0, (T)0.0},
                    {-(T)std::sin(theta), (T)0.0, (T)std::cos(theta), (T)0.0},
                    { (T)0.0, (T)0.0, (T)0.0, (T)1.0 },
                };

                return Matrix4<T>(tmp);
            }

            template <typename T>
            Matrix4<T> create_rotation_z(double theta)
            {
                /* 列優先表記を使用
                    行列計算は A = B @ C のため回転行列の表記は
                        C, -S, 0, 0
                        S,  C, 0, 0
                        0,  0, 1, 0
                        0, 0, 0, 1
                */
                T tmp[4][4] = {
                    {(T)std::cos(theta), (T)std::sin(theta), (T)0.0, (T)0.0},
                    {-(T)std::sin(theta), std::cos(theta), (T)0.0, (T)0.0},
                    {(T)0.0, (T)0.0, (T)1.0, (T)0.0},
                    {(T)0.0, (T)0.0, (T)0.0, (T)1.0},
                };

                return Matrix4<T>(tmp);
            }

            template <typename T>
            Matrix4<T> create_translation(const Vector3<T> &trans)
            {
                /* 列優先表記を使用
                    行列計算の数学表記は A = B @ C のため回転行列の表記は
                        1, 0, 0, X
                        0, 1, 0. Y
                        0, 0, 0, Z
                        0, 0, 0, 1
                */
                T tmp[4][4] = {
                    {(T)1.0, (T)0.0, (T)0.0, trans.x_},
                    {(T)0.0, (T)1.0, (T)0.0, trans.y_},
                    {(T)0.0, (T)0.0, (T)1.0, trans.z_},
                    {(T)0.0, (T)0.0, (T)0.0, (T)1.0,},
                };

                return Matrix4<T>(tmp);
            }

            template <typename T>
            Vector3<T> axis_x_as_poture(const Matrix4<T> &mat)
            {
                // ワールド座標系に対するローカル座標のX軸を抽出(単位ベクトル)
                /* 列優先表記を使用
                        [xv1, yv1, zv1, X]
                        [xv2, yv2, zv2, Y]
                        [xv3, yv3, zv3, Z]
                        [0, 0, 0, 1]
                    vx = (xv1, xv2, xv3).normalize
                */
                return Vector3<T>::normalize(Vector3<T>(mat[0][0], mat[1][0], mat_[2][0]));
            }

            template <typename T>
            Vector3<T> axis_y_as_poture(const Matrix4<T> &mat)
            {
                // ワールド座標系に対するローカル座標のY軸を抽出(単位ベクトル)
                    /* 列優先表記を使用
                            [xv1, yv1, zv1, X]
                            [xv2, yv2, zv2, Y]
                            [xv3, yv3, zv3, Z]
                            [0, 0, 0, 1]
                        vy = (yv1, yv2, yv3).normalize
                    */
                return Vector3<T>::normalize(Vector3<T>(mat[0][1], mat[1][1], mat[2][1]));
            }

            template <typename T>
            Vector3<T> axis_z_as_poture(const Matrix4<T> &mat)
            {
                // ワールド座標系に対するローカル座標のZ軸を抽出(単位ベクトル)
                    /* 列優先表記を使用
                            [xv1, yv1, zv1, X]
                            [xv2, yv2, zv2, Y]
                            [xv3, yv3, zv3, Z]
                            [0, 0, 0, 1]
                        vz = (zv1, zv2, zv3).normalize
                    */
                return Vector3<T>::normalize(Vector3<T>(mat[0][2], mat[1][2], mat[2][2]));
            }

            template <typename T>
            Vector3<T> translation(const Matrix4<T> &mat)
            {
                return Vector3<T>(mat_[0][3], mat_[1][3], mat_[2][3]);
            }

            template <typename T>
            Vector3<T> scale(const Matrix4<T> &mat)
            {
                // スケールを取得
                /* 列優先表記を使用
                    [xv1, yv1, zv1, X]
                    [xv2, yv2, zv2, Y]
                    [xv3, yv3, zv3, Z]
                    [0, 0, 0, 1]
                    nx = (xv1,xv2,xv3).norm
                    ny = (yv1,yv2,yv3).norm
                    nz = (zv1,zv2,zv3).norm
                    sv = (nx, ny, nz)
                */
                Vector3<T> ret;
                ret.x_ = Vector3<T>(mat[0][0], mat[1][0], mat[2][0]).norm();
                ret.y_ = Vector3<T>(mat[0][1], mat[1][1], mat[2][1]).norm();
                ret.z_ = Vector3<T>(mat[0][2], mat[1][2], mat[2][2]).norm();
                return ret;
            }

        } // namespace geometry
    }
}


namespace is
{
    namespace common
    {
        namespace geometry
        {
            /*シングルトン*/
            class GeometryContext
            {
                IS_DISABLE_COPY_AND_ASSIGN(GeometryContext)
                
            public:
                ~GeometryContext() {}
                GeometryContext(GeometryContext&&) = default;
                GeometryContext& operator=(GeometryContext&&) = default;

                GeometryContext& global_context() const;
            private:
                friend class SingletonManager;
                GeometryContext();

                /*
                    1) X-up, Y-up, Z-up
                    2) Left-coor, Right-coor
                    3) intrinsic-euler, extrinsic-euler
                    4) euler-order 12 pattern
                        XXX, XXY, XXZ, XYX, XYY, XYZ, XZX, XZY, XZZ, ZXX, ZXY, ZXZ, ZYX, ZYY, ZYZ, ZZX, ZZY, ZZZ
                */
            };
        }
    } // namespace common
}