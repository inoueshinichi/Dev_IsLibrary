/**
 * @file math.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-02-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <IsCommon/internal/defs.hpp>
#include <IsCommon/internal/exception.hpp>

#if _MSC_VER
#define _USE_MATH_DEFINES
#endif
#include <cmath>

#include <algorithm>

// #include <Eigen/Dense>

namespace is
{
    namespace common
    {
        namespace math
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
                if (std::fabs(val) <= epsilon)
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

            inline float abs(float value) { return fabs(value); }
            inline float cos(float value) { return cosf(value); }
            inline float sin(float value) { return sinf(value); }
            inline float tan(float value) { return tanf(value); }
            inline float acos(float value) { return acosf(value); }
            inline float asin(float value) { return asinf(value); }
            inline float atan2(float y, float x) { return atan2f(y, x); }
            inline float cot(float value) { return 1.0f / tan(value); }
            inline float sqrt(float value) { return sqrtf(value); }
            inline float fmod(float numer, float denom) { return fmod(numer, denom); }
            inline float lerp(float a, float b, float f) { return a + f * (a - b); }
        }

//         namespace eigen
//         {
// #define EIGEN_NO_DEBUG
// #define EIGEN_MPL2_ONLY

//             template <typename T>
//             using Matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

//             template <typename T>
//             using RowVector = Eigen::Matrix<T, 1, Eigen::Dynamic>;

//             template <typename T>
//             using ColVector = Eigen::Matrix<T, Eigen::Dynamic, 1>;

//             template <typename T>
//             using MatrixMap = Eigen::Map<Matrix<T>>;

//             template <typename T>
//             using ConstMatrixMap = Eigen::Map<const Matrix<T>>;

//             template <typename T>
//             using RowVectorMap = Eigen::Map<RowVector<T>>;

//             template <typename T>
//             using ConstRowVectorMap = Eigen::Map<const RowVector<T>>;

//             template <typename T>
//             using ColVectorMap = Eigen::Map<ColVector<T>>;

//             template <typename T>
//             using ConstColVectorMap = Eigen::Map<const ColVector<T>>;
//         }

        ////////////////////////////////////////////////////
        // 2次元ベクトル
        ////////////////////////////////////////////////////
        class Vector2
        {
        public:
            float x_;
            float y_;

            IS_DISABLE_COPY_AND_ASSIGN(Vector2)

        public:
            explicit Vector2() : x_(0.0f), y_(0.0f) {}
            virtual ~Vector2() = default;
            Vector2(float x, float y) : x_(x), y_(y) {}
            Vector2(Vector2 &&other) = default;
            Vector2 &operator=(Vector2 &&other) = default;

            const float *get_as_ptr() const { return reinterpret_cast<const float *>(&x_); }

            void set(float x, float y)
            {
                this->x_ = x;
                this->y_ = y;
            }

            void copy(Vector2 &vec) const { vec.set(this->x_, this->y_); }

            Vector2 clone() const
            {
                Vector2 ret;
                this->copy(ret);
                return ret; 
            }
            

            float norm2() const { return x_ * x_ + y_ * y_; }
            float norm() const { return sqrt(norm2()); }

            void normalize()
            {
                float norm = this->norm();
                x_ /= norm;
                y_ /= norm;
            }

            static Vector2 normalize(const Vector2 &vec)
            {
                Vector2 ret = vec.clone();
                ret.normalize();
                return ret;
            }

            // 1) opeartor (a+b)
            friend inline Vector2 operator+(const Vector2 &a, const Vector2 &b)
            {
                return Vector2(a.x_ + b.x_, a.y_ + b.y_);
            }

            // 2) operator (a-b)
            friend inline Vector2 operator-(const Vector2 &a, const Vector2 &b)
            {
                return Vector2(a.x_ - b.x_, a.y_ - b.y_);
            }

            // 3) operator (a*b)
            friend inline Vector2 operator*(const Vector2 &a, const Vector2 &b)
            {
                return Vector2(a.x_ * b.x_, a.y_ * b.y_);
            }

            // 4) operator (s*b)
            friend inline Vector2 operator*(float s, const Vector2 &b)
            {
                return Vector2(s * b.x_, s * b.y_);
            }

            // 5) operator (a*s)
            friend inline Vector2 operator*(const Vector2 &a, float s)
            {
                return Vector2(s * a.x_, s * a.y_);
            }

            // 6) operator *=
            Vector2 &operator*=(float s)
            {
                x_ *= s;
                y_ *= s;
                return *this;
            }

            // 7) operator /=
            Vector2 &operator/=(float s)
            {
                x_ /= s;
                y_ /= s;
                return *this;
            }

            // 8) operator +=
            Vector2 &operator+=(const Vector2 &other)
            {
                x_ += other.x_;
                y_ += other.y_;
                return *this;
            }

            // 9) operator -=
            Vector2 &operator-=(const Vector2 &other)
            {
                x_ -= other.x_;
                y_ -= other.y_;
                return *this;
            }

            // 10) operator *=
            Vector2 &operator*=(const Vector2 &other)
            {
                x_ *= other.x_;
                y_ *= other.y_;
                return *this;
            }

            // 11) operator ==
            friend inline bool operator==(const Vector2 &a, const Vector2 &b)
            {
                return (a.x_ == b.x_) && (a.y_ == b.y_);
            }

            // 12) operator !=
            friend inline bool operator!=(const Vector2 &a, const Vector2 &b)
            {
                return !(a == b);
            }

            static float dot(const Vector2 &a, const Vector2 &b)
            {
                return (a.x_ * b.x_ + a.y_ * b.y_);
            }

            static Vector2 lerp(const Vector2 &a, const Vector2 &b, float f)
            {
                return Vector2(a + f * (b - a));
            }

            static Vector2 reflect(const Vector2 &v, const Vector2 &n)
            {
                return v - 2.0f * Vector2::dot(v, n) * n;
            }

            // transform
            static Vector2 transform(const Vector2 &vec, const class Matrix3 &mat, float w = 1.0f);

            static const Vector2 zero;
            static const Vector2 unit_x;
            static const Vector2 unit_y;
            static const Vector2 ng_unit_x;
            static const Vector2 ng_unit_y;
            static const Vector2 inf;
            static const Vector2 ng_inf;
        };

        ////////////////////////////////////////////////////
        // 3次元ベクトル
        ////////////////////////////////////////////////////
        class Vector3
        {
        public:
            float x_;
            float y_;
            float z_;

            IS_DISABLE_COPY_AND_ASSIGN(Vector3)

        public:
            Vector3() : x_(0.0f), y_(0.0f), z_(0.0f) {}
            virtual ~Vector3() = default;
            Vector3(float x, float y, float z) : x_(x), y_(y), z_(z) {}
            Vector3(Vector3 &&other) = default;
            Vector3 &operator=(Vector3 &&other) = default;

            const float *get_as_ptr() const { return reinterpret_cast<const float *>(&x_); }

            void set(float x, float y, float z)
            {
                this->x_ = x;
                this->y_ = y;
                this->z_ = z;
            }

            void copy(Vector3 &vec) const { vec.set(this->x_, this->y_, this->z_); }

            Vector3 clone() const
            { 
                Vector3 ret;
                this->copy(ret);
                return ret;
            }
            
            float norm2() const { return x_ * x_ + y_ * y_ + z_ * z_; }
            float norm() const { return sqrt(norm2()); }

            void normalize()
            {
                float norm = this->norm();
                x_ /= norm;
                y_ /= norm;
                z_ /= norm;
            }

            static Vector3 normalize(const Vector3 &vec)
            {
                Vector3 ret;
                vec.copy(ret);
                ret.normalize();
                return ret;
            }

            // 1) opeartor (a+b)
            friend inline Vector3 operator+(const Vector3 &a, const Vector3 &b)
            {
                return Vector3(a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_);
            }

            // 2) operator (a-b)
            friend inline Vector3 operator-(const Vector3 &a, const Vector3 &b)
            {
                return Vector3(a.x_ - b.x_, a.y_ - b.y_, a.z_ - b.z_);
            }

            // 3) operator (a*b)
            friend inline Vector3 operator*(const Vector3 &a, const Vector3 &b)
            {
                return Vector3(a.x_ * b.x_, a.y_ * b.y_, a.z_ * b.z_);
            }

            // 4) operator (s*b)
            friend inline Vector3 operator*(float s, const Vector3 &b)
            {
                return Vector3(s * b.x_, s * b.y_, s * b.z_);
            }

            // 5) operator (a*s)
            friend inline Vector3 operator*(const Vector3 &a, float s)
            {
                return Vector3(s * a.x_, s * a.y_, s * a.z_);
            }

            // 6) operator *=
            Vector3 &operator*=(float s)
            {
                x_ *= s;
                y_ *= s;
                z_ *= s;
                return *this;
            }

            // 7) operator /=
            Vector3 &operator/=(float s)
            {
                x_ /= s;
                y_ /= s;
                z_ /= s;
                return *this;
            }

            // 8) operator +=
            Vector3 &operator+=(const Vector3 &other)
            {
                x_ += other.x_;
                y_ += other.y_;
                z_ += other.z_;
                return *this;
            }

            // 9) operator -=
            Vector3 &operator-=(const Vector3 &other)
            {
                x_ -= other.x_;
                y_ -= other.y_;
                z_ -= other.z_;
                return *this;
            }

            // 10) operator *=
            Vector3 &operator*=(const Vector3 &other)
            {
                x_ *= other.x_;
                y_ *= other.y_;
                z_ *= other.z_;
                return *this;
            }

            // 11) operator ==
            friend inline bool operator==(const Vector3 &a, const Vector3 &b)
            {
                return (a.x_ == b.x_) && (a.y_ == b.y_) && (a.z_ == b.z_);
            }

            // 12) operator !=
            friend inline bool operator!=(const Vector3 &a, const Vector3 &b)
            {
                return !(a == b);
            }

            static float dot(const Vector3 &a, const Vector3 &b)
            {
                return (a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_);
            }

            static Vector3 lerp(const Vector3 &a, const Vector3 &b, float f)
            {
                return Vector3(a + f * (b - a));
            }

            static Vector3 reflect(const Vector3 &v, const Vector3 &n)
            {
                return v - 2.0f * Vector3::dot(v, n) * n;
            }

            static Vector3 cross(const Vector3 &a, const Vector3 &b)
            {
                Vector3 ret;
                ret.x_ = a.y_ * b.z_ - a.z_ * b.y_;
                ret.y_ = a.z_ * b.x_ - a.x_ * b.z_;
                ret.z_ = a.x_ * b.y_ - a.y_ * b.x_;
                return ret;
            }

            static Vector3 transform(const Vector3 &vec, const class Matrix4 &mat, float w = 1.0f);
            static Vector3 transform_with_persp_div(const Vector3 &vec, const class Matrix4 mat, float w = 1.0f);

            static Vector3 transform(const Vector3 &vec, const class Quaternion &q);

            static const Vector3 zero;
            static const Vector3 unit_x;
            static const Vector3 unit_y;
            static const Vector3 unit_z;
            static const Vector3 ng_unit_x;
            static const Vector3 ng_unit_y;
            static const Vector3 ng_unit_z;
            static const Vector3 inf;
            static const Vector3 ng_inf;
        };

        ////////////////////////////////////////////////////
        // 4次元ベクトル
        ////////////////////////////////////////////////////
        class Vector4
        {
        public:
            float x_;
            float y_;
            float z_;
            float w_;

            IS_DISABLE_COPY_AND_ASSIGN(Vector4)

        public:
            Vector4() : x_(0.0f), y_(0.0f), z_(0.0f), w_(0.0f) {}
            virtual ~Vector4() = default;
            Vector4(float x, float y, float z, float w) : x_(x), y_(y), z_(z), w_(w) {}
            Vector4(Vector4 &&other) = default;
            Vector4 &operator=(Vector4 &&other) = default;

            const float *get_as_ptr() const { return reinterpret_cast<const float *>(&x_); }

            void set(float x, float y, float z, float w)
            {
                this->x_ = x_;
                this->y_ = y_;
                this->z_ = z_;
                this->w_ = w_;
            }

            void copy(Vector4 &vec) const { vec.set(this->x_, this->y_, this->z_, this->w_); }

            Vector4 clone() const
            { 
                Vector4 ret;
                this->copy(ret);
                return ret;
            }
            

            float norm2() const { return x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_; }
            float norm() const { return sqrt(norm2()); }

            void normalize()
            {
                float norm = this->norm();
                x_ /= norm;
                y_ /= norm;
                z_ /= norm;
                w_ /= norm;
            }

            static Vector4 normalize(const Vector4 &vec)
            {
                Vector4 ret = vec.clone();
                ret.normalize();
                return ret;
            }

            // 1) opeartor (a+b)
            friend inline Vector4 operator+(const Vector4 &a, const Vector4 &b)
            {
                return Vector4(a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_, a.w_ + b.w_);
            }

            // 2) operator (a-b)
            friend inline Vector4 operator-(const Vector4 &a, const Vector4 &b)
            {
                return Vector4(a.x_ - b.x_, a.y_ - b.y_, a.z_ - b.z_, a.w_ - b.w_);
            }

            // 3) operator (a*b)
            friend inline Vector4 operator*(const Vector4 &a, const Vector4 &b)
            {
                return Vector4(a.x_ * b.x_, a.y_ * b.y_, a.z_ * b.z_, a.w_ * b.w_);
            }

            // 4) operator (s*b)
            friend inline Vector4 operator*(float s, const Vector4 &b)
            {
                return Vector4(s * b.x_, s * b.y_, s * b.z_, s * b.w_);
            }

            // 5) operator (a*s)
            friend inline Vector4 operator*(const Vector4 &a, float s)
            {
                return Vector4(s * a.x_, s * a.y_, s * a.z_, s * a.w_);
            }

            // 6) operator *=
            Vector4 &operator*=(float s)
            {
                x_ *= s;
                y_ *= s;
                z_ *= s;
                w_ *= s;
                return *this;
            }

            // 7) operator /=
            Vector4 &operator/=(float s)
            {
                x_ /= s;
                y_ /= s;
                z_ /= s;
                w_ /= s;
                return *this;
            }

            // 8) operator +=
            Vector4 &operator+=(const Vector4 &other)
            {
                x_ += other.x_;
                y_ += other.y_;
                z_ += other.z_;
                w_ += other.w_;
                return *this;
            }

            // 9) operator -=
            Vector4 &operator-=(const Vector4 &other)
            {
                x_ -= other.x_;
                y_ -= other.y_;
                z_ -= other.z_;
                w_ -= other.w_;
                return *this;
            }

            // 10) operator *=
            Vector4 &operator*=(const Vector4 &other)
            {
                x_ *= other.x_;
                y_ *= other.y_;
                z_ *= other.z_;
                w_ *= other.w_;
                return *this;
            }

            // 11) operator ==
            friend inline bool operator==(const Vector4 &a, const Vector4 &b)
            {
                return (a.x_ == b.x_) && (a.y_ == b.y_) && (a.z_ == b.z_) && (a.w_ == b.w_);
            }

            // 12) operator !=
            friend inline bool operator!=(const Vector4 &a, const Vector4 &b)
            {
                return !(a == b);
            }

            static float dot(const Vector4 &a, const Vector4 &b)
            {
                return (a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_ + a.w_ * b.w_);
            }

            static const Vector4 zero;
            static const Vector4 unit_x;
            static const Vector4 unit_y;
            static const Vector4 unit_z;
            static const Vector4 unit_w;
            static const Vector4 ng_unit_x;
            static const Vector4 ng_unit_y;
            static const Vector4 ng_unit_z;
            static const Vector4 ng_unit_w;
            static const Vector4 inf;
            static const Vector4 ng_inf;
        };

        ////////////////////////////////////////////////////
        // 正方3次元行列
        ////////////////////////////////////////////////////
        class Matrix3
        {
        public:
            float mat_[3][3];

            IS_DISABLE_COPY_AND_ASSIGN(Matrix3)

        public:
            Matrix3() { *this = Matrix3::zero(); }
            explicit Matrix3(float m[3][3]) { set(m); }

            virtual ~Matrix3() = default;
            Matrix3(Matrix3 &&other) = default;
            Matrix3 &operator=(Matrix3 &&other) = default;

            static Matrix3 zero() { return Matrix3::diag(0.f, 0.f, 0.f); }

            static Matrix3 diag(float m00, float m11, float m22)
            {
                Matrix3 ret;
                ret.mat_[0][0] = m00;
                ret.mat_[0][1] = 0.f;
                ret.mat_[0][2] = 0.f;
                ret.mat_[1][0] = 0.f;
                ret.mat_[1][1] = m11;
                ret.mat_[1][2] = 0.f;
                ret.mat_[2][0] = 0.f;
                ret.mat_[2][1] = 0.f;
                ret.mat_[2][2] = m22;
                return ret;
            }

            static Matrix3 diag(const Vector3 &vec) { return Matrix3::diag(vec.x_, vec.y_, vec.z_); }
            static Matrix3 identity() { return Matrix3::diag(1.f, 1.f, 1.f); }

            static Matrix3 constant(float n)
            {
                Matrix3 ret;
                std::fill((float *)&ret.mat_[0][0], (float *)&ret.mat_[2][2], n);
                return ret;
            }

            const float *get_as_ptr() const { return reinterpret_cast<const float *>(&mat_[0][0]); }
            void set(const float m[3][3]) { std::memcpy(mat_, m, 9 * sizeof(float)); } // m[3][3]は float(*)[3]型

            void copy(Matrix3 &m) const { m.set(this->mat_+0); } // float[3][3] -> float(*)[3]

            Matrix3 clone() const
            { 
                Matrix3 ret;
                this->copy(ret);
                return ret; 
            }
            

            float trace() const { return (mat_[0][0] + mat_[1][1] + mat_[2][2]); }

            Vector3 rows(int row)
            {
                IS_FORCE_ASSERT((row >= 0 && row < 3), "Out of range row in Matrix3. Given is %d.", row);
                return Vector3(mat_[row][0], mat_[row][1], mat_[row][2]);
            }

            Vector3 cols(int col)
            {
                IS_FORCE_ASSERT((col >= 0 && col < 3), "Out of range col in Matrix3. Given is %d.", col);
                return Vector3(mat_[0][col], mat_[1][col], mat_[2][col]);
            }

            void insert_rows(int row, const Vector3 &vec)
            {
                IS_FORCE_ASSERT((row >= 0 && row < 3), "Out of range row in Matrix3. Given is %d.", row);
                mat_[row][0] = vec.x_;
                mat_[row][1] = vec.y_;
                mat_[row][2] = vec.z_;
            }

            void insert_cols(int col, const Vector3 &vec)
            {
                IS_FORCE_ASSERT((col >= 0 && col < 3), "Out of range col in Matrix3. Given is %d.", col);
                mat_[0][col] = vec.x_;
                mat_[1][col] = vec.y_;
                mat_[2][col] = vec.z_;
            }

            int rows() const { return 3; }
            int cols() const { return 3; }

            // 未実装
            Matrix3 inv() const { return Matrix3::identity(); }
            float det() const { return 1.0f; }
            Matrix3 t() const { return Matrix3::identity(); }
            int rank() const { return 1.0f; }

            // 1) operator L[m,n] @ R[n,o] = [m,o]
            friend Matrix3 operator*(const Matrix3 &left, const Matrix3 &right)
            {
            }

            // 2) operator *=
            Matrix3 &operator*=(const Matrix3 &right)
            {
                *this = *this * right;
                return *this;
            }

            // 3) operator M[m,n] @ V[n,1] = [m,1]
            friend Vector3 operator*(const Matrix3 &m, const Vector3 &v)
            {
                IS_ERROR(error_code::not_implemented, "Implement operator M[m,n] @ V[n,1] = [m,1]")
            }

            // 4) operator V[1,n] @ M[n,m] = [1,m]
            friend Vector3 operator*(const Vector3 &v, const Matrix3 &m)
            {
                IS_ERROR(error_code::not_implemented, "Implement operator V[1,n] @ M[n,m] = [1,m]")
            }

            static Matrix3 create_scale(float sx, float sy)
            {
                return Matrix3::diag(sx, sy, 1.0f);
            }

            static Matrix3 create_scale(const Vector2 &sv)
            {
                return Matrix3::diag(sv.x_, sv.y_, 1.0f);
            }

            static Matrix3 create_scale(float s)
            {
                return Matrix3::create_scale(s, s);
            }

            static Matrix3 create_rotation(float theta)
            {
                /*
                    行列計算の数学表記は A = B @ C のため回転行列の表記は
                        C, -S, 0
                        S,  C, 0
                        0,  0, 1
                    だが, プログラム上では左から右に更新掛け算する慣例のため
                    計算表記が A^T = C^T @ B^T となる.
                    そのため, 回転行列の表記が転置となる.
                        C, S, 0
                       -S, C, 0
                        0, 0, 1
                */
                float tmp[3][3] = {
                    {math::cos(theta), math::sin(theta), 0.f},
                    {-math::sin(theta), math::cos(theta), 0.f},
                    {0.f, 0.f, 1.f},
                };

                return Matrix3(tmp);
            }

            static Matrix3 create_translation(const Vector2 &trans)
            {
                /*
                    行列計算の数学表記は A = B @ C のため回転行列の表記は
                        1, 0, X
                        0, 1, Y
                        0, 0, 1
                    だが, プログラム上では左から右に更新掛け算する慣例のため
                    計算表記が A^T = C^T @ B^T となる.
                    そのため, 回転行列の表記が転置となる.
                        1, 0, 0
                        0, 1, 0
                        X, Y, 1
                */
                float tmp[3][3] = {
                    {1.f, 0.f, 0.f},
                    {0.f, 1.f, 0.f},
                    {trans.x_, trans.y_, 1.f},
                };

                return Matrix3(tmp);
            }

            Vector2 axis_x_as_poture() const
            {
                // ワールド座標系に対するローカル座標のX軸を抽出
                /*
                    数学表記は
                        [xv1, yv1, X]
                        [xv2, yv2, Y]
                        [0, 0, 1]
                    プログラム表記は
                        [xv1, xv2, 0]
                        [yv1, yv2, 0]
                        [X, Y, 1]

                    vx = (xv1, xv2)
                */
                return Vector2::normalize(Vector2(mat_[0][0], mat_[0][1]));
            }

            Vector2 axis_y_as_poture() const
            {
                // ワールド座標系に対するローカル座標系のY軸を抽出
                /*
                    数学表記は
                        [xv1, yv1, X]
                        [xv2, yv2, Y]
                        [0, 0, 1]
                    プログラム表記は
                        [xv1, xv2, 0]
                        [yv1, yv2, 0]
                        [X, Y, 1]

                    vy = (yv1, yv2)
                */
                return Vector2::normalize(Vector2(mat_[1][0], mat_[1][1]));
            }

            Vector2 translation() const
            {
                return Vector2(mat_[2][0], mat_[2][1]);
            }

            Vector2 scale() const
            {
                // スケールを取得
                /*
                   数学表記は
                       [xv1, yv1, X]
                       [xv2, yv2, Y]
                       [0, 0, 1]
                   プログラム表記は
                       [xv1, xv2, 0]
                       [yv1, yv2, 0]
                       [X, Y, 1]

                   scale = (sx, sy)
                   sx = norm(vector(xv1, xv2))
                   sy = norm(vector(yv1, yv2))
               */
                Vector2 ret;
                ret.x_ = Vector2(mat_[0][0], mat_[0][1]).norm();
                ret.y_ = Vector2(mat_[1][0], mat_[1][1]).norm();
                return ret;
            }
        };

        ////////////////////////////////////////////////////
        // 正方4次元行列
        ////////////////////////////////////////////////////
        class Matrix4
        {
        public:
            float mat_[4][4];

            IS_DISABLE_COPY_AND_ASSIGN(Matrix4)

        public:
            Matrix4() { *this = Matrix4::zero(); }
            explicit Matrix4(float m[4][4]) { set(m); }
            Matrix4(Matrix4 &&other) = default;
            Matrix4 &operator=(Matrix4 &&other) = default;

            static Matrix4 zero() { return Matrix4::diag(0.f, 0.f, 0.f, 0.f); }

            static Matrix4 diag(float m00, float m11, float m22, float m33)
            {
                Matrix4 ret;
                ret.mat_[0][0] = m00;
                ret.mat_[0][1] = 0.f;
                ret.mat_[0][2] = 0.f;
                ret.mat_[0][3] = 0.f;
                ret.mat_[1][0] = 0.f;
                ret.mat_[1][1] = m11;
                ret.mat_[1][2] = 0.f;
                ret.mat_[1][3] = 0.f;
                ret.mat_[2][0] = 0.f;
                ret.mat_[2][1] = 0.f;
                ret.mat_[2][2] = m22;
                ret.mat_[2][3] = 0.f;
                ret.mat_[3][0] = 0.f;
                ret.mat_[3][1] = 0.f;
                ret.mat_[3][2] = 0.f;
                ret.mat_[3][3] = m33;
                return ret;
            }

            static Matrix4 diag(const Vector4 &vec) { return Matrix4::diag(vec.x_, vec.y_, vec.z_, vec.w_); }
            static Matrix4 identity() { return Matrix4::diag(1.f, 1.f, 1.f, 1.f); }

            static Matrix4 constant(float n)
            {
                Matrix4 ret;
                std::fill((float *)&ret.mat_[0][0], (float *)&ret.mat_[3][3], n);
                return ret;
            }

            const float *get_as_ptr() const { return reinterpret_cast<const float *>(&mat_[0][0]); }
            void set(const float m[4][4]) { std::memcpy(mat_, m, 16 * sizeof(float)); }

            void copy(Matrix4 &m) const { m.set(this->mat_+0); }

            Matrix4 clone() const
            { 
                Matrix4 ret;
                this->copy(ret);
                return ret;
            }
            

            float trace() const { return (mat_[0][0] + mat_[1][1] + mat_[2][2] + mat_[3][3]); }

            Vector4 rows(int row)
            {
                IS_FORCE_ASSERT((row >= 0 && row < 4), "Out of range row in Matrix3. Given is %d.", row);
                return Vector4(mat_[row][0], mat_[row][1], mat_[row][2], mat_[row][3]);
            }

            Vector4 cols(int col)
            {
                IS_FORCE_ASSERT((col >= 0 && col < 4), "Out of range col in Matrix3. Given is %d.", col);
                return Vector4(mat_[0][col], mat_[1][col], mat_[2][col], mat_[3][col]);
            }

            void insert_rows(int row, const Vector4 &vec)
            {
                IS_FORCE_ASSERT((row >= 0 && row < 4), "Out of range row in Matrix3. Given is %d.", row);
                mat_[row][0] = vec.x_;
                mat_[row][1] = vec.y_;
                mat_[row][2] = vec.z_;
                mat_[row][3] = vec.w_;
            }

            void insert_cols(int col, const Vector4 &vec)
            {
                IS_FORCE_ASSERT((col >= 0 && col < 4), "Out of range col in Matrix3. Given is %d.", col);
                mat_[0][col] = vec.x_;
                mat_[1][col] = vec.y_;
                mat_[2][col] = vec.z_;
                mat_[3][col] = vec.w_;
            }

            int rows() const { return 4; }
            int cols() const { return 4; }

            // 未実装
            Matrix4 inv() const { return Matrix4::identity(); }
            float det() const { return 1.0f; }
            Matrix4 t() const { return Matrix4::identity(); }
            int rank() const { return 1.0f; }

            // 1) operator L[m,n] @ R[n,o] = [m,o]
            friend Matrix4 operator*(const Matrix4 &left, const Matrix4 &right){
                IS_ERROR(error_code::not_implemented, "Implement operator L[m,n] @ R[n,o] = [m,o]")}

            // 2) operator *=
            Matrix4 &operator*=(const Matrix4 &right)
            {
                *this = *this * right;
                return *this;
            }

            // 3) operator M[m,n] @ V[n,1] = [m,1]
            friend Vector4 operator*(const Matrix4 &m, const Vector4 &v)
            {
                IS_ERROR(error_code::not_implemented, "Implement operator M[m,n] @ V[n,1] = [m,1]")
            }

            // 4) operator V[1,n] @ M[n,m] = [1,m]
            friend Vector4 operator*(const Vector4 v, const Matrix4 &m)
            {
                IS_ERROR(error_code::not_implemented, "Implement operator V[1,n] @ M[n,m] = [1,m]")
            }

            static Matrix4 create_scale(float sx, float sy, float sz)
            {
                return Matrix4::diag(sx, sy, sz, 1.0f);
            }

            static Matrix4 create_scale(const Vector3 &sv)
            {
                return Matrix4::diag(sv.x_, sv.y_, sv.z_, 1.0f);
            }

            static Matrix4 create_scale(float s)
            {
                return Matrix4::create_scale(s, s, s);
            }

            static Matrix4 create_rotation_x(float theta)
            {
                /*
                    行列計算の数学表記は A = B @ C のため回転行列の表記は
                        C, -S, 0
                        S,  C, 0
                        0,  0, 1
                    だが, プログラム上では左から右に更新掛け算する慣例のため
                    計算表記が A^T = C^T @ B^T となる.
                    そのため, 回転行列の表記が転置となる.
                        C, S, 0
                       -S, C, 0
                        0, 0, 1
                */
                float tmp[4][4] = {
                    {1.f, 0.f, 0.f, 0.f},
                    {0.f, math::cos(theta), math::sin(theta), 0.f},
                    {0.f, -math::sin(theta), math::cos(theta), 0.f},
                    {0.f, 0.f, 0.f, 1.f},
                };

                return Matrix4(tmp);
            }

            static Matrix4 create_rotation_y(float theta)
            {
                /*
                    行列計算の数学表記は A = B @ C のため回転行列の表記は
                        C, -S, 0
                        S,  C, 0
                        0,  0, 1
                    だが, プログラム上では左から右に更新掛け算する慣例のため
                    計算表記が A^T = C^T @ B^T となる.
                    そのため, 回転行列の表記が転置となる.
                        C, S, 0
                       -S, C, 0
                        0, 0, 1
                */
                float tmp[4][4] = {
                    {math::cos(theta), 0.f, -math::sin(theta), 0.f},
                    {0.f, 1.f, 0.f, 0.f},
                    {math::sin(theta), 0.f, math::cos(theta), 0.f},
                    {0.f, 0.f, 0.f, 1.f},
                };

                return Matrix4(tmp);
            }

            static Matrix4 create_rotation_z(float theta)
            {
                /*
                    行列計算の数学表記は A = B @ C のため回転行列の表記は
                        C, -S, 0
                        S,  C, 0
                        0,  0, 1
                    だが, プログラム上では左から右に更新掛け算する慣例のため
                    計算表記が A^T = C^T @ B^T となる.
                    そのため, 回転行列の表記が転置となる.
                        C, S, 0
                       -S, C, 0
                        0, 0, 1
                */
                float tmp[4][4] = {
                    {math::cos(theta), math::sin(theta), 0.f, 0.f},
                    {-math::sin(theta), math::cos(theta), 0.f, 0.f},
                    {0.f, 0.f, 1.f, 0.f},
                    {0.f, 0.f, 0.f, 1.f},
                };

                return Matrix4(tmp);
            }

            static Matrix4 create_translation(const Vector3 &trans)
            {
                /*
                    行列計算の数学表記は A = B @ C のため回転行列の表記は
                        1, 0, X
                        0, 1, Y
                        0, 0, 1
                    だが, プログラム上では左から右に更新掛け算する慣例のため
                    計算表記が A^T = C^T @ B^T となる.
                    そのため, 回転行列の表記が転置となる.
                        1, 0, 0
                        0, 1, 0
                        X, Y, 1
                */
                float tmp[4][4] = {
                    {1.f, 0.f, 0.f, 0.f},
                    {0.f, 1.f, 0.f, 0.f},
                    {0.f, 0.f, 1.f, 0.f},
                    {trans.x_, trans.y_, trans.z_, 1.f},
                };

                return Matrix4(tmp);
            }

            Vector3 axis_x_as_poture() const
            {
                // ワールド座標系に対するローカル座標のX軸を抽出
                /*
                    数学表記は
                        [xv1, yv1, X]
                        [xv2, yv2, Y]
                        [0, 0, 1]
                    プログラム表記は
                        [xv1, xv2, 0]
                        [yv1, yv2, 0]
                        [X, Y, 1]

                    vx = (xv1, xv2)
                */
                return Vector3::normalize(Vector3(mat_[0][0], mat_[0][1], mat_[0][2]));
            }

            Vector3 axis_y_as_poture() const
            {
                // ワールド座標系に対するローカル座標系のY軸を抽出
                /*
                    数学表記は
                        [xv1, yv1, X]
                        [xv2, yv2, Y]
                        [0, 0, 1]
                    プログラム表記は
                        [xv1, xv2, 0]
                        [yv1, yv2, 0]
                        [X, Y, 1]

                    vy = (yv1, yv2)
                */
                return Vector3::normalize(Vector3(mat_[1][0], mat_[1][1], mat_[1][2]));
            }

            Vector3 axis_z_as_poture() const
            {
                // ワールド座標系に対するローカル座標系のZ軸を抽出
                return Vector3::normalize(Vector3(mat_[2][0], mat_[2][1], mat_[2][2]));
            }

            Vector3 translation() const
            {
                return Vector3(mat_[3][0], mat_[3][1], mat_[3][2]);
            }

            Vector3 scale() const
            {
                // スケールを取得
                /*
                   数学表記は
                       [xv1, yv1, X]
                       [xv2, yv2, Y]
                       [0, 0, 1]
                   プログラム表記は
                       [xv1, xv2, 0]
                       [yv1, yv2, 0]
                       [X, Y, 1]

                   scale = (sx, sy)
                   sx = norm(vector(xv1, xv2))
                   sy = norm(vector(yv1, yv2))
               */
                Vector3 ret;
                ret.x_ = Vector3(mat_[0][0], mat_[0][1], mat_[0][2]).norm();
                ret.y_ = Vector3(mat_[1][0], mat_[1][1], mat_[1][2]).norm();
                ret.z_ = Vector3(mat_[2][0], mat_[2][1], mat_[2][2]).norm();
                return ret;
            }
        };
    }
}