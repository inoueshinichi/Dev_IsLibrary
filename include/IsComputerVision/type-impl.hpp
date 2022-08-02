/**
 * @file type-impl.hpp
 * @author your name (you@domain.com)
 * @brief 型情報テンプレート実装
 * @version 0.1
 * @date 2022-02-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */


namespace is
{
    namespace imgproc
    {
        ////////////////////////////////////////////////////
        // Point_ 2次元座標プライマリーテンプレートクラス
        ////////////////////////////////////////////////////
        // 1) Point_<T> &operator+=(Point_<T> &a, const Point_<T> &b);
        template <typename T>
        static inline Point_<T> &operator+=(Point_<T> &a, const Point_<T> &b)
        {
            a.x_ = saturate_clamp<T>(a.x_ + b.x_);
            a.y_ = saturate_clamp<T>(a.y_ + b.y_);
            return a;
        }

        // 2) Point_<T> &operator-=(Point_<T> &a, const Point_<T> &b);
        template <typename T>
        static inline Point_<T> &operator-=(Point_<T> &a, const Point_<T> &b)
        {
            a.x_ = saturate_clamp<T>(a.x_ - b.x_);
            a.y_ = saturate_clamp<T>(a.y_ - b.y_);
            return a;
        }

        // 3) Point_<T> &operator*=(Point_<T> &a, U b);
        template <typename T, typename U>
        static inline Point_<T> &operator*=(Point_<T> &a, U b)
        {
            a.x_ = saturate_clamp<T>(a.x_ * b);
            a.y_ = saturate_clamp<T>(a.y_ * b);
            return a;
        }

        // 4) Point_<T> &operatpr/=(Point_<T> &a, U b);
        template <typename T, typename U>
        static inline Point_<T> &operator/=(Point_<T> &a, U b)
        {
            a.x_ = saturate_clamp<T>(a.x_ / b);
            a.y_ = saturate_clamp<T>(a.y_ / b);
            return a;
        }

        // 5) bool operator==(const Point_<T> &a, const Point_<T> &b);
        template <typename T>
        static inline bool operator==(const Point_<T> &a, const Point_<T> &b)
        {
            return a.x_ == b.x_ && a.y_ == b.y_;
        }

        // 6) bool operator!=(const Point_<T> &a, const Point_<T> &b);
        template <typename T>
        static inline bool operator!=(const Point_<T> &a, const Point_<T> &b)
        {
            return !operator==(a, b);
        }

        // 7) Point_<T> operator+(const Point_<T> &a, const Point_<T> &b)
        template <typename T>
        static inline Point_<T> operator+(const Point_<T> &a, const Point_<T> &b)
        {
            return operator+=(a, b);
        }

        // 8) Point_<T> operator-(const Point_<T> &a, const Point_<T> &b)
        template <typename T>
        static inline Point_<T> operator-(const Point_<T> &a, const Point_<T> &b)
        {
            return operator-(a, b);
        }

        // 9) Point_<T> operator*(const Point_<T> &a, U b)
        template <typename T, typename U>
        static inline Point_<T> operator*(const Point_<T> &a, U b)
        {
            return operator*(a, b);
        }

        // 10) Point_<T> operator/(const Point_<T> &a, U b)
        template <typename T, typename U>
        static inline Point_<T> operator/(const Point_<T> &a, U b)
        {
            return operator/(a, b);
        }


        ////////////////////////////////////////////////////
        // Size_ 縦横プライマリーテンプレートクラス
        ////////////////////////////////////////////////////

        ////////////////////////////////////////////////////
        // Rect_ 2次元領域プライマリーテンプレートクラス
        ////////////////////////////////////////////////////
        template <typename T>
        inline bool Rect_<T>::contains(const Point_<T> &point) const
        {
            return point.x_ >= left_ &&
                   point.x_ < right_ &&
                   point.y_ >= top_ &&
                   point.y_ < bottom_;
        }

        template <typename T>
        inline T Rect_<T>::width() const { return right_ - left_; }

        template <typename T>
        inline T Rect_<T>::height() const { return bottom_ - top_; }

        template <typename T>
        Point_<T> Rect_<T>::left_top() const
        {
            return Point_<T>(left_, top_);
        }

        template <typename T>
        Point_<T> Rect_<T>::right_bottom() const
        {
            return Point_<T>(right_, bottom_);
        }

        ////////////////////////////////////////////////////
        // Point3_ 3次元座標プライマリーテンプレートクラス
        ////////////////////////////////////////////////////
        // 1) Point3_<T> &operator+=(Point3_<T> &a, const Point3_<T> &b);
        template <typename T>
        static inline Point3_<T> &operator+=(Point3_<T> &a, const Point3_<T> &b)
        {
            a.x_ = saturate_clamp<T>(a.x_ + b.x_);
            a.y_ = saturate_clamp<T>(a.y_ + b.y_);
            a.z_ = saturate_clamp<T>(a.z_ + b.z_);
            return a;
        }

        // 2) Point3_<T> &operator-=(Point3_<T> &a, const Point3_<T> &b);
        template <typename T>
        static inline Point3_<T> &operator-=(Point3_<T> &a, const Point3_<T> &b)
        {
            a.x_ = saturate_clamp<T>(a.x_ - b.x_);
            a.y_ = saturate_clamp<T>(a.y_ - b.y_);
            a.z_ = saturate_clamp<T>(a.z_ - b.z_);
            return a;
        }

        // 3) Point3_<T> &operator*=(Point3_<T> &a, U b);
        template <typename T, typename U>
        static inline Point3_<T> &operator*=(Point3_<T> &a, U b)
        {
            a.x_ = saturate_clamp<T>(a.x_ * b);
            a.y_ = saturate_clamp<T>(a.y_ * b);
            a.z_ = saturate_clamp<T>(a.z_ * b);
            return a;
        }

        // 4) Point3_<T> &operatpr/=(Point3_<T> &a, U b);
        template <typename T, typename U>
        static inline Point3_<T> &operator/=(Point3_<T> &a, U b)
        {
            a.x_ = saturate_clamp<T>(a.x_ / b);
            a.y_ = saturate_clamp<T>(a.y_ / b);
            a.z_ = saturate_clamp<T>(a.z_ / b);
            return a;
        }

        // 5) bool operator==(const Point3_<T> &a, const Point3_<T> &b);
        template <typename T>
        static inline bool operator==(const Point3_<T> &a, const Point3_<T> &b)
        {
            return a.x_ == b.x_ && a.y_ == b.y_ && a.z_ == b.z_;
        }

        // 6) bool operator!=(const Point3_<T> &a, const Point3_<T> &b);
        template <typename T>
        static inline bool operator!=(const Point3_<T> &a, const Point3_<T> &b)
        {
            return !operator==(a, b);
        }

        // 7) Point3_<T> operator+(const Point3_<T> &a, const Point3_<T> &b)
        template <typename T>
        static inline Point3_<T> operator+(const Point3_<T> &a, const Point3_<T> &b)
        {
            return operator+=(a, b);
        }

        // 8) Point3_<T> operator-(const Point3_<T> &a, const Point3_<T> &b)
        template <typename T>
        static inline Point3_<T> operator-(const Point3_<T> &a, const Point3_<T> &b)
        {
            return operator-(a, b);
        }

        // 9) Point3_<T> operator*(const Point3_<T> &a, U b)
        template <typename T, typename U>
        static inline Point3_<T> operator*(const Point3_<T> &a, U b)
        {
            return operator*(a, b);
        }

        // 10) Point3_<T> operator/(const Point3_<T> &a, U b)
        template <typename T, typename U>
        static inline Point3_<T> operator/(const Point3_<T> &a, U b)
        {
            return operator/(a, b);
        }


        ////////////////////////////////////////////////////
        // Voxel 3次元座標プライマリーテンプレートクラス
        ////////////////////////////////////////////////////

    } // imgproc
}
