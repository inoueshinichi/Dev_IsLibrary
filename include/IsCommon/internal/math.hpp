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
            namespace eigen
            {
#define EIGEN_NO_DEBUG
#define EIGEN_MPL2_ONLY

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
            } // namespace eigen

 
            ////////////////////////////////////////////////////
            // 2次元ベクトル
            ////////////////////////////////////////////////////
            template <typename T>
            class Vector2
            {
                /**
                 * @brief 行優先データ配列, 列優先表記配列 = Numpy・OpenCVスタイル
                 * @note Shape = [2,1]
                 */
            public:
                T x_;
                T y_;
                int ndim {2};
                int shape_[2] = {2,1};
                std::vector<int> stride_;

                IS_DISABLE_COPY_AND_ASSIGN(Vector2<T>)

            public:
                explicit Vector2() : x_((T)0.0), y_((T)0.0) { stride_.reserve(1); stride_.at(0) = sizeof(T); }
                virtual ~Vector2() = default;
                Vector2(T x, T y) : x_(x), y_(y) { stride_.reserve(1); stride_.at(0) = sizeof(T); }
                Vector2(Vector2<T> &&other) = default;
                Vector2<T> &operator=(Vector2<T> &&other) = default;

                void transpose() { int tmp = shape_[0]; shape_[0] = shape_[1]; shape_[1] = tmp; }
                Vector2<T> transpose() const { Vector2<T> ret; this->copy(ret); ret.transpose(); return ret; }

                const T *get_as_ptr() const { return reinterpret_cast<const T *>(&x_); }

                void set(double x, double y)
                {
                    this->x_ = (T)x;
                    this->y_ = (T)y;
                }

                template <typename U>
                void copy(Vector2<U> &vec) const { vec.set((U)this->x_, (U)this->y_); }

                Vector2<t> clone() const
                {
                    Vector2<T> ret;
                    this->copy(ret);
                    return ret; 
                }
                

                double norm2() const { return (double)(x_ * x_ + y_ * y_); }
                double norm() const { return std::sqrt(norm2()); }

                void normalize()
                {
                    T norm = (T)this->norm();
                    IS_CHECK(norm != (T)0.0, error_code::value, "Zero division error! Given is %lf", norm);
                    x_ /= norm;
                    y_ /= norm;
                }

                static Vector2<T> normalize(const Vector2<T> &vec)
                {
                    Vector2<T> ret = vec.clone();
                    ret.normalize();
                    return ret;
                }

                // 1) opeartor (a+b)
                friend inline Vector2<T> operator+(const Vector2<T> &a, const Vector2<T> &b)
                {
                    return Vector2<T>(a.x_ + b.x_, a.y_ + b.y_);
                }

                // 2) operator (a-b)
                friend inline Vector2<T> operator-(const Vector2<T> &a, const Vector2<T> &b)
                {
                    return Vector2<T>(a.x_ - b.x_, a.y_ - b.y_);
                }

                // 3) operator (a*b)
                friend inline Vector2<T> operator*(const Vector2<T> &a, const Vector2<T> &b)
                {
                    return Vector2<T>(a.x_ * b.x_, a.y_ * b.y_);
                }

                // 4) operator (s*b)
                friend inline Vector2<T> operator*(double s, const Vector2<T> &b)
                {
                    return Vector2<T>((T)s * b.x_, (T)s * b.y_);
                }

                // 5) operator (a*s)
                friend inline Vector2<T> operator*(const Vector2<T> &a, double s)
                {
                    return Vector2<T>((T)s * a.x_, (T)s * a.y_);
                }

                // 6) operator *=
                Vector2<T> &operator*=(double s)
                {
                    x_ *= (T)s;
                    y_ *= (T)s;
                    return *this;
                }

                // 7) operator /=
                Vector2<T> &operator/=(double s)
                {
                    IS_CHECK((T)s != (T)0.0, error_code::value, "Zero division error! Given is %lf", s);
                    x_ /= (T)s;
                    y_ /= (T)s;
                    return *this;
                }

                // 8) operator +=
                Vector2<T> &operator+=(const Vector2<T> &other)
                {
                    x_ += other.x_;
                    y_ += other.y_;
                    return *this;
                }

                // 9) operator -=
                Vector2<T> &operator-=(const Vector2<T> &other)
                {
                    x_ -= other.x_;
                    y_ -= other.y_;
                    return *this;
                }

                // 10) operator *=
                Vector2<T> &operator*=(const Vector2<T> &other)
                {
                    x_ *= other.x_;
                    y_ *= other.y_;
                    return *this;
                }

                // 11) operator ==
                friend inline bool operator==(const Vector2<T> &a, const Vector2<T> &b)
                {
                    return (a.x_ == b.x_) && (a.y_ == b.y_);
                }

                // 12) operator !=
                friend inline bool operator!=(const Vector2<T> &a, const Vector2<T> &b)
                {
                    return !(a == b);
                }

                // 13) operator <<
                friend inline std::ostream& operator<<(const std::ostream& os, const Vector2<T> &v)
                {
                    IS_ERROR(error_code::not_implemented, "Use Eigen");
                }

                static double dot(const Vector2<T> &a, const Vector2<T> &b)
                {
                    return (double)(a.x_ * b.x_ + a.y_ * b.y_);
                }

                static const Vector2<T> zero;
                static const Vector2<T> unit_x;
                static const Vector2<T> unit_y;
                static const Vector2<T> ng_unit_x;
                static const Vector2<T> ng_unit_y;
                static const Vector2<T> inf;
                static const Vector2<T> ng_inf;
            };


            ////////////////////////////////////////////////////
            // 3次元ベクトル
            ////////////////////////////////////////////////////
            template <typename T>
            class Vector3
            {
                /**
                 * @brief 行優先データ配列, 列優先表記配列 = Numpy・OpenCVスタイル
                 * @note Shape = [3,1]
                 */
            public:
                T x_;
                T y_;
                T z_;
                int ndim {2};
                int shape_[2] = {3,1};
                std::vector<int> stride_;

                IS_DISABLE_COPY_AND_ASSIGN(Vector3<T>)

            public:
                Vector3() : x_((T)0.0), y_((T)0.0), z_((T)0.0) { stride_.reserve(1); stride_.at(0) = sizeof(T); }
                virtual ~Vector3() = default;
                Vector3(T x, T y, T z) : x_(x), y_(y), z_(z) { stride_.reserve(1); stride_.at(0) = sizeof(T); }
                Vector3(Vector3<T> &&other) = default;
                Vector3<T> &operator=(Vector3<T> &&other) = default;

                void transpose() { int tmp = shape_[0]; shape_[0] = shape_[1]; shape_[1] = tmp; }
                Vector3<T> transpose() const { Vector3<T> ret; this->copy(ret); ret.transpose(); return ret; }

                const T *get_as_ptr() const { return reinterpret_cast<const T *>(&x_); }

                void set(double x, double y, double z)
                {
                    this->x_ = (T)x;
                    this->y_ = (T)y;
                    this->z_ = (T)z;
                }

                template <typename U>
                void copy(Vector3<U> &vec) const { vec.set((U)this->x_, (U)this->y_, (U)this->z_); }

                Vector3<T> clone() const
                { 
                    Vector3<T> ret;
                    this->copy(ret);
                    return ret;
                }
                
                double norm2() const { return (double)(x_ * x_ + y_ * y_ + z_ * z_); }
                double norm() const { return std::sqrt(norm2()); }

                void normalize()
                {
                    T norm = (T)this->norm();
                    IS_CHECK(norm != (T)0.0, error_code::value, "Zero division error! Given is %lf", norm);
                    x_ /= norm;
                    y_ /= norm;
                    z_ /= norm;
                }

                static Vector3<T> normalize(const Vector3<T> &vec)
                {
                    Vector3<T> ret;
                    vec.copy(ret);
                    ret.normalize();
                    return ret;
                }

                // 1) opeartor (a+b)
                friend inline Vector3<T> operator+(const Vector3<T> &a, const Vector3<T> &b)
                {
                    return Vector3<T>(a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_);
                }

                // 2) operator (a-b)
                friend inline Vector3<T> operator-(const Vector3<T> &a, const Vector3<T> &b)
                {
                    return Vector3<T>(a.x_ - b.x_, a.y_ - b.y_, a.z_ - b.z_);
                }

                // 3) operator (a*b)
                friend inline Vector3<T> operator*(const Vector3<T> &a, const Vector3<T> &b)
                {
                    return Vector3<T>(a.x_ * b.x_, a.y_ * b.y_, a.z_ * b.z_);
                }

                // 4) operator (s*b)
                friend inline Vector3<T> operator*(double s, const Vector3<T> &b)
                {
                    return Vector3<T>((T)s * b.x_, (T)s * b.y_, (T)s * b.z_);
                }

                // 5) operator (a*s)
                friend inline Vector3<T> operator*(const Vector3<T> &a, double s)
                {
                    return Vector3<T>((T)s * a.x_, (T)s * a.y_, (T)s * a.z_);
                }

                // 6) operator *=
                Vector3<T> &operator*=(double s)
                {
                    x_ *= (T)s;
                    y_ *= (T)s;
                    z_ *= (T)s;
                    return *this;
                }

                // 7) operator /=
                Vector3<T> &operator/=(double s)
                {
                    IS_CHECK((T)s != (T)0.0, error_code::value, "Zero division error! Given is %lf", s);
                    x_ /= s;
                    y_ /= s;
                    z_ /= s;
                    return *this;
                }

                // 8) operator +=
                Vector3<T> &operator+=(const Vector3<T> &other)
                {
                    x_ += other.x_;
                    y_ += other.y_;
                    z_ += other.z_;
                    return *this;
                }

                // 9) operator -=
                Vector3<T> &operator-=(const Vector3<T> &other)
                {
                    x_ -= other.x_;
                    y_ -= other.y_;
                    z_ -= other.z_;
                    return *this;
                }

                // 10) operator *=
                Vector3<T> &operator*=(const Vector3<T> &other)
                {
                    x_ *= other.x_;
                    y_ *= other.y_;
                    z_ *= other.z_;
                    return *this;
                }

                // 11) operator ==
                friend inline bool operator==(const Vector3<T> &a, const Vector3<T> &b)
                {
                    return (a.x_ == b.x_) && (a.y_ == b.y_) && (a.z_ == b.z_);
                }

                // 12) operator !=
                friend inline bool operator!=(const Vector3<T> &a, const Vector3<T> &b)
                {
                    return !(a == b);
                }

                // 13) operator <<
                friend inline std::ostream& operator<<(const std::ostream& os, const Vector3<T> &v)
                {
                    IS_ERROR(error_code::not_implemented, "Use Eigen");
                }

                static double dot(const Vector3<T> &a, const Vector3<T> &b)
                {
                    return (double)(a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_);
                }

                static const Vector3<T> zero;
                static const Vector3<T> unit_x;
                static const Vector3<T> unit_y;
                static const Vector3<T> unit_z;
                static const Vector3<T> ng_unit_x;
                static const Vector3<T> ng_unit_y;
                static const Vector3<T> ng_unit_z;
                static const Vector3<T> inf;
                static const Vector3<T> ng_inf;
            };

            ////////////////////////////////////////////////////
            // 4次元ベクトル
            ////////////////////////////////////////////////////
            template <typename T>
            class Vector4
            {
            public:
                T x_;
                T y_;
                T z_;
                T w_;
                int ndim {2};
                int shape_[2] = {4,1};
                std::vector<int> stride_;

                IS_DISABLE_COPY_AND_ASSIGN(Vector4<T>)

            public:
                Vector4() : x_((T)0.0), y_((T)0.0), z_((T)0.0), w_((T)0.0) { stride_.reserve(1); stride_.at(0) = sizeof(T); }
                virtual ~Vector4() = default;
                Vector4(T x, T y, T z, T w) : x_(x), y_(y), z_(z), w_(w) { stride_.reserve(1); stride_.at(0) = sizeof(T); }
                Vector4(Vector4<T> &&other) = default;
                Vector4<T> &operator=(Vector4<T> &&other) = default;

                void transpose() { int tmp = shape_[0]; shape_[0] = shape_[1]; shape_[1] = tmp; }
                Vector4<T> transpose() const { Vector4<T> ret; this->copy(ret); ret.transpose(); return ret; }

                const T *get_as_ptr() const { return reinterpret_cast<const T *>(&x_); }

                void set(double x, double y, double z, double w)
                {
                    this->x_ = (T)x_;
                    this->y_ = (T)y_;
                    this->z_ = (T)z_;
                    this->w_ = (T)w_;
                }

                void copy(Vector4<T> &vec) const { vec.set(this->x_, this->y_, this->z_, this->w_); }

                Vector4<T> clone() const
                { 
                    Vector4<T> ret;
                    this->copy(ret);
                    return ret;
                }
                

                double norm2() const { return (double)(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_); }
                double norm() const { return std::sqrt(norm2()); }

                void normalize()
                {
                    T norm = (T)this->norm();
                    IS_CHECK(norm != (T)0.0, error_code::value, "Zero division error! Given is %lf", norm);
                    x_ /= norm;
                    y_ /= norm;
                    z_ /= norm;
                    w_ /= norm;
                }

                static Vector4<T> normalize(const Vector4<T> &vec)
                {
                    Vector4<T> ret = vec.clone();
                    ret.normalize();
                    return ret;
                }

                // 1) opeartor (a+b)
                friend inline Vector4<T> operator+(const Vector4<T> &a, const Vector4<T> &b)
                {
                    return Vector4<T>(a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_, a.w_ + b.w_);
                }

                // 2) operator (a-b)
                friend inline Vector4<T> operator-(const Vector4<T> &a, const Vector4<T> &b)
                {
                    return Vector4<T>(a.x_ - b.x_, a.y_ - b.y_, a.z_ - b.z_, a.w_ - b.w_);
                }

                // 3) operator (a*b)
                friend inline Vector4<T> operator*(const Vector4<T> &a, const Vector4<T> &b)
                {
                    return Vector4<T>(a.x_ * b.x_, a.y_ * b.y_, a.z_ * b.z_, a.w_ * b.w_);
                }

                // 4) operator (s*b)
                friend inline Vector4<T> operator*(double s, const Vector4<T> &b)
                {
                    return Vector4<T>((T)s * b.x_, (T)s * b.y_, (T)s * b.z_, (T)s * b.w_);
                }

                // 5) operator (a*s)
                friend inline Vector4<T> operator*(const Vector4 &a, double s)
                {
                    return Vector4<T>((T)s * a.x_, (T)s * a.y_, (T)s * a.z_, (T)s * a.w_);
                }

                // 6) operator *=
                Vector4<T> &operator*=(double s)
                {
                    x_ *= (T)s;
                    y_ *= (T)s;
                    z_ *= (T)s;
                    w_ *= (T)s;
                    return *this;
                }

                // 7) operator /=
                Vector4<T> &operator/=(double s)
                {
                    IS_CHECK((T)s != (T)0.0, error_code::value, "Zero division error! Given is %lf", s);
                    x_ /= (T)s;
                    y_ /= (T)s;
                    z_ /= (T)s;
                    w_ /= (T)s;
                    return *this;
                }

                // 8) operator +=
                Vector4<T> &operator+=(const Vector4<T> &other)
                {
                    x_ += other.x_;
                    y_ += other.y_;
                    z_ += other.z_;
                    w_ += other.w_;
                    return *this;
                }

                // 9) operator -=
                Vector4<T> &operator-=(const Vector4<T> &other)
                {
                    x_ -= other.x_;
                    y_ -= other.y_;
                    z_ -= other.z_;
                    w_ -= other.w_;
                    return *this;
                }

                // 10) operator *=
                Vector4<T> &operator*=(const Vector4<T> &other)
                {
                    x_ *= other.x_;
                    y_ *= other.y_;
                    z_ *= other.z_;
                    w_ *= other.w_;
                    return *this;
                }

                // 11) operator ==
                friend inline bool operator==(const Vector4<T> &a, const Vector4<T> &b)
                {
                    return (a.x_ == b.x_) && (a.y_ == b.y_) && (a.z_ == b.z_) && (a.w_ == b.w_);
                }

                // 12) operator !=
                friend inline bool operator!=(const Vector4<T> &a, const Vector4<T> &b)
                {
                    return !(a == b);
                }

                // 13) operator <<
                friend inline std::ostream& operator<<(const std::ostream& os, const Vector4<T> &v)
                {
                    IS_ERROR(error_code::not_implemented, "Use Eigen");
                }

                static double dot(const Vector4<T> &a, const Vector4<T> &b)
                {
                    return (double)(a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_ + a.w_ * b.w_);
                }

                static const Vector4<T> zero;
                static const Vector4<T> unit_x;
                static const Vector4<T> unit_y;
                static const Vector4<T> unit_z;
                static const Vector4<T> unit_w;
                static const Vector4<T> ng_unit_x;
                static const Vector4<T> ng_unit_y;
                static const Vector4<T> ng_unit_z;
                static const Vector4<T> ng_unit_w;
                static const Vector4<T> inf;
                static const Vector4<T> ng_inf;
            };

            ////////////////////////////////////////////////////
            // 正方3次元行列
            ////////////////////////////////////////////////////
            template <typename T>
            class Matrix3
            {
            public:
                /**
                 * @brief 行優先データ配列, 列優先表記配列 = Numpy・OpenCVスタイル
                 * 
                 */
                T mat_[3][3];
                int ndim {2};
                int shape_[2] = {3,3};
                std::vector<int> stride_;

                IS_DISABLE_COPY_AND_ASSIGN(Matrix3<T>)

            public:
                Matrix3() { stride_.reseve(2); stride_.at(0) = sizeof(T); stride_.at(1) = shape_[0] * stride_.at(0); }
                explicit Matrix3(T m[3][3]) { set(m); stride_.reseve(2); stride_.at(0) = sizeof(T); stride_.at(1) = shape_[0] * stride_.at(0); }

                virtual ~Matrix3() = default;
                Matrix3(Matrix3<T> &&other) = default;
                Matrix3<T> &operator=(Matrix3<T> &&other) = default;

                static Matrix3<T> zero() { return Matrix3<T>::diag((T)0.0, (T)0.0, (T)0.0); }

                static Matrix3<T> diag(double m00, double m11, double m22)
                {
                    Matrix3<T> ret;
                    ret.mat_[0][0] = (T)m00;
                    ret.mat_[0][1] = (T)0.0;
                    ret.mat_[0][2] = (T)0.0;
                    ret.mat_[1][0] = (T)0.0;
                    ret.mat_[1][1] = (T)m11;
                    ret.mat_[1][2] = (T)0.0;
                    ret.mat_[2][0] = (T)0.0;
                    ret.mat_[2][1] = (T)0.0;
                    ret.mat_[2][2] = (T)m22;
                    return ret;
                }

                static Matrix3<T> diag(const Vector3<T> &vec) { return Matrix3<T>::diag(vec.x_, vec.y_, vec.z_); }
                static Matrix3<T> identity() { return Matrix3<T>::diag(1.0, 1.0, 1.0); }

                static Matrix3<T> constant(double n)
                {
                    Matrix3<T> ret;
                    std::fill((T *)&ret.mat_[0][0], (T *)&ret.mat_[2][2], (T)n); // @todo ポインタの移動が正しいか確認すること
                    return ret;
                }

                void transpose() 
                { 
                    IS_ERROR(error_code::not_implemented, "Use Eigen");
                }

                Matrix3<T> transpose()
                {
                    Matrix3<T> ret;
                    this->copy(ret);
                    ret.transpose();
                    return ret;
                }

                const T *get_as_ptr() const { return reinterpret_cast<const T *>(&mat_[0][0]); }

                void set(const double m[3][3]) 
                { 
                    for (int y = 0; y < 3; ++y)
                    {
                        for (int x = 0; x < 3; ++x)
                        {
                            mat_[y][x] = (T)m[y][x];
                        }
                    }
                    
                } // m[3][3]は float(*)[3]型

                
                /* @todo 全配列のdouble型がfloatに正しくなるかチェックすること*/
                void copy(Matrix3<T> &m) const { m.set((double (*)[3])this->mat_ + 0); } // float[3][3] -> float(*)[3]

                Matrix3<T> clone() const
                { 
                    Matrix3<T> ret;
                    this->copy(ret);
                    return ret; 
                }
                

                double trace() const { return (double)(mat_[0][0] + mat_[1][1] + mat_[2][2]); }

                Vector3<T> rows(int row)
                {
                    IS_FORCE_ASSERT((row >= 0 && row < 3), "Out of range row in Matrix3. Given is %d.", row);
                    Vector3<T> ret(mat_[row][0], mat_[row][1], mat_[row][2]);
                    ret.transpose();
                    return ret;
                }

                Vector3<T> cols(int col)
                {
                    IS_FORCE_ASSERT((col >= 0 && col < 3), "Out of range col in Matrix3. Given is %d.", col);
                    return Vector3(mat_[0][col], mat_[1][col], mat_[2][col]);
                }

                void insert_rows(int row, const Vector3<T> &vec)
                {
                    IS_FORCE_ASSERT((row >= 0 && row < 3), "Out of range row in Matrix3. Given is %d.", row);
                    IS_FORCE_ASSERT((vec.shape_[0] == 1 && vec.shape_[1] == 3), "Not match shape of vector3. Given is (%d,%d).", vec.shape_[0], vec.shape_[1]);
                    mat_[row][0] = vec.x_;
                    mat_[row][1] = vec.y_;
                    mat_[row][2] = vec.z_;
                }

                void insert_cols(int col, const Vector3<T> &vec)
                {
                    IS_FORCE_ASSERT((col >= 0 && col < 3), "Out of range col in Matrix3. Given is %d.", col);
                    IS_FORCE_ASSERT((vec.shape_[0] == 3 && vec.shape_[1] == 1), "Not match shape of vector3. Given is (%d,%d).", vec.shape_[0], vec.shape_[1]);
                    mat_[0][col] = vec.x_;
                    mat_[1][col] = vec.y_;
                    mat_[2][col] = vec.z_;
                }

                int rows() const { return 3; }
                int cols() const { return 3; }

                

                // 1) operator L[m,n] @ R[n,o] = [m,o]
                friend inline Matrix3<T> operator*(const Matrix3<T> &left, const Matrix3<T> &right)
                {
                    IS_ERROR(error_code::not_implemented, "Use Eigen")
                }

                // 2) operator *= L[m,n] @ R[n,o] = [m,o]
                Matrix3<T> &operator*=(const Matrix3<T> &right)
                {
                    *this = *this * right;
                    return *this;
                }

                // 3) operator M[m,n] @ V[n,1] = [m,1]
                friend inline Vector3<T> operator*(const Matrix3<T> &m, const Vector3<T> &v)
                {
                    IS_ERROR(error_code::not_implemented, "Implement operator M[m,n] @ V[n,1] = [m,1]")
                }

                // 4) operator V[1,n] @ M[n,m] = [1,m]
                friend inline Vector3<T> operator*(const Vector3<T> &v, const Matrix3<T> &m)
                {
                    IS_ERROR(error_code::not_implemented, "Implement operator V[1,n] @ M[n,m] = [1,m]")
                }

                // 5) operator N[n,n] + M[n,n] = L[n,n]
                friend inline Matrix3<T> operator+(const Matrix3<T> &a, const Matrix3<T> &b)
                {
                    IS_ERROR(error_code::not_implemented, "Use Eigen")
                }

                // 6) operator N[n,n] - M[n,n] = L[n,n]
                friend inline Matrix3<T> operator-(const Matrix3<T> &a, const Matrix3<T> &b)
                {
                    IS_ERROR(error_code::not_implemented, "Use Eigen")
                }

                // 7) operator N[n,n] + s[1,1] = N[n,n]
                friend inline Matrix3<T> operator+(const Matrix3<T> &a, double s)
                {
                    IS_ERROR(error_code::not_implemented, "Use Eigen")
                }

                // 8) operator s[1,1] + N[n,n] = N[n,n]
                friend inline Matrix3<T> operator+(double s, const Matrix3<T> &b)
                {
                    IS_ERROR(error_code::not_implemented, "Use Eigen")
                }
            };

            ////////////////////////////////////////////////////
            // 正方4次元行列
            ////////////////////////////////////////////////////
            template <typename T>
            class Matrix4
            {
                /**
                 * @brief 行優先データ配列, 列優先表記配列 = Numpy・OpenCVスタイル
                 * 
                 */
            public:
                T mat_[4][4];
                int ndim {2};
                int shape_[2] = {4,4};
                std::vector<int> stride_;

                IS_DISABLE_COPY_AND_ASSIGN(Matrix4<T>)

            public:
                Matrix4() { stride_.reseve(2); stride_.at(0) = sizeof(T); stride_.at(1) = shape_[0] * stride_.at(0); }
                explicit Matrix4(T m[4][4]) { set(m); stride_.reseve(2); stride_.at(0) = sizeof(T); stride_.at(1) = shape_[0] * stride_.at(0);}
                Matrix4(Matrix4<T> &&other) = default;
                Matrix4 &operator=(Matrix4<T> &&other) = default;

                void transpose() 
                { 
                    IS_ERROR(error_code::not_implemented, "Use Eigen");
                }

                Matrix3<T> transpose()
                {
                    Matrix4<T> ret;
                    this->copy(ret);
                    ret.transpose();
                    return ret;
                }

                static Matrix4<T> zero() { return Matrix4::diag(0.0, 0.0, 0.0, 0.0); }

                static Matrix4<T> diag(double m00, double m11, double m22, double m33)
                {
                    Matrix4 ret;
                    ret.mat_[0][0] = (T)m00;
                    ret.mat_[0][1] = (T)0.0;
                    ret.mat_[0][2] = (T)0.0;
                    ret.mat_[0][3] = (T)0.0;
                    ret.mat_[1][0] = (T)0.0;
                    ret.mat_[1][1] = (T)m11;
                    ret.mat_[1][2] = (T)0.0;
                    ret.mat_[1][3] = (T)0.0;
                    ret.mat_[2][0] = (T)0.0;
                    ret.mat_[2][1] = (T)0.0;
                    ret.mat_[2][2] = (T)m22;
                    ret.mat_[2][3] = (T)0.0;
                    ret.mat_[3][0] = (T)0.0;
                    ret.mat_[3][1] = (T)0.0;
                    ret.mat_[3][2] = (T)0.0;
                    ret.mat_[3][3] = (T)m33;
                    return ret;
                }

                static Matrix4<T> diag(const Vector4<T> &vec) { return Matrix4::diag(vec.x_, vec.y_, vec.z_, vec.w_); }
                static Matrix4<T> identity() { return Matrix4::diag(1.0, 1.0, 1.0, 1.0); }

                static Matrix4 constant(float n)
                {
                    Matrix4 ret;
                    std::fill((T *)&ret.mat_[0][0], (T *)&ret.mat_[3][3], (T)n);
                    return ret;
                }

                const T *get_as_ptr() const { return reinterpret_cast<const T *>(&mat_[0][0]); }


                void set(const T m[4][4]) 
                {  
                    for (int y = 0; y < 4; ++y)
                    {
                        for (int x = 0; x < 4; ++x)
                        {
                            mat_[y][x] = (T)m[y][x];
                        }
                    }
                }

                void copy(Matrix4<T> &m) const { m.set(this->mat_ + 0); }

                Matrix4<T> clone() const
                { 
                    Matrix4<T> ret;
                    this->copy(ret);
                    return ret;
                }
                

                double trace() const { return (double)(mat_[0][0] + mat_[1][1] + mat_[2][2] + mat_[3][3]); }

                Vector4 rows(int row)
                {
                    IS_FORCE_ASSERT((row >= 0 && row < 4), "Out of range row in Matrix3. Given is %d.", row);
                    Vector4<T> ret(mat_[row][0], mat_[row][1], mat_[row][2], mat_[row][3]);
                    ret.transpose();
                    return ret;
                }

                Vector4 cols(int col)
                {
                    IS_FORCE_ASSERT((col >= 0 && col < 4), "Out of range col in Matrix3. Given is %d.", col);
                    return Vector4(mat_[0][col], mat_[1][col], mat_[2][col], mat_[3][col]);
                }

                void insert_rows(int row, const Vector4 &vec)
                {
                    IS_FORCE_ASSERT((row >= 0 && row < 4), "Out of range row in Matrix3. Given is %d.", row);
                    IS_FORCE_ASSERT((vec.shape_[0] == 1 && vec.shape_[1] == 4), "Not match shape of vector4. Given is (%d,%d).", vec.shape_[0], vec.shape_[1]);
                    mat_[row][0] = vec.x_;
                    mat_[row][1] = vec.y_;
                    mat_[row][2] = vec.z_;
                    mat_[row][3] = vec.w_;
                }

                void insert_cols(int col, const Vector4 &vec)
                {
                    IS_FORCE_ASSERT((col >= 0 && col < 4), "Out of range col in Matrix3. Given is %d.", col);
                    IS_FORCE_ASSERT((vec.shape_[0] == 4 && vec.shape_[1] == 1), "Not match shape of vector4. Given is (%d,%d).", vec.shape_[0], vec.shape_[1]);
                    mat_[0][col] = vec.x_;
                    mat_[1][col] = vec.y_;
                    mat_[2][col] = vec.z_;
                    mat_[3][col] = vec.w_;
                }

                int rows() const { return 4; }
                int cols() const { return 4; }

                // 1) operator L[m,n] @ R[n,o] = [m,o]
                friend Matrix4<T> operator*(const Matrix4<T> &left, const Matrix4<T> &right)
                {
                    IS_ERROR(error_code::not_implemented, "Implement operator L[m,n] @ R[n,o] = [m,o]")
                }

                // 2) operator *=
                Matrix4<T> &operator*=(const Matrix4<T> &right)
                {
                    *this = *this * right;
                    return *this;
                }

                // 3) operator M[m,n] @ V[n,1] = [m,1]
                friend Vector4<T> operator*(const Matrix4<T> &m, const Vector4<T> &v)
                {
                    IS_ERROR(error_code::not_implemented, "Implement operator M[m,n] @ V[n,1] = [m,1]")
                }

                // 4) operator V[1,n] @ M[n,m] = [1,m]
                friend Vector4<T> operator*(const Vector4<T> v, const Matrix4<T> &m)
                {
                    IS_ERROR(error_code::not_implemented, "Implement operator V[1,n] @ M[n,m] = [1,m]")
                }
            };
        }
    }
}