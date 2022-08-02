/**
 * @file type.hpp
 * @author your name (you@domain.com)
 * @brief 型情報
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
        // // 前方宣言
        // template <typename T>
        // class Rect_<T>;

        ////////////////////////////////////////////////////
        // Point_ 2次元座標プライマリーテンプレートクラス
        ////////////////////////////////////////////////////
        template <typename T>
        class Point_
        {
        public:
            T x_;
            T y_;

        public:
            Point_() : x_(T(0)), y_(T(0)) {}
            virtual ~Point_() = default;
            Point_(T x, T y) : x_(x), y_(y) {}

            Point_(const Point_<T> &other) = default;
            Point_<T> &operator=(const Point_<T> &other) = default;
            Point_(Point_<T> &&other) = default;
            Point_<T> &operator=(Point_<T> &&other) = default;

            template <typename U>
            operator Point_<U>() const { return Point_<U>(saturate_clamp<U>(x_), saturate_clamp<U>(y_)); }
            double norm() const { return std::sqrt(x_ * x_ + y_ * y_); }
            double dot(const Point_<T> &point) const { return std::sqrt(x_ * point.x_ + y_ * point.y_); }

            // // ポインタ使わないと前方宣言できないからどうやって実現しているかわからない
            // bool inside(const Rect_<T> &rect) const
            // {
            //     return rect.contains(*this);
            // }

            //     /*static operator*/
            //     // 1) Point_<T> &operator+=(Point_<T> &a, const Point_<T> &b);
            //     // 2) Point_<T> &operator-=(Point_<T> &a, const Point_<T> &b);
            //     // 3) Point_<T> &operator*=(Point_<T> &a, U b);
            //     // 4) Point_<T> &operatpr/=(Point_<T> &a, U b);
            //     // 5) bool operator==(const Point_<T> &a, const Point_<T> &b);
            //     // 6) bool operator!=(const Point_<T> &a, const Point_<T> &b);
            //     // 7) Point_<T> operator+(const Point_<T> &a, const Point_<T> &b);
            //     // 8) Point_<T> operator-(const Point_<T> &a, const Point_<T> &b);
            //     // 9) Point_<T> operator*(const Point_<T> &a, U b);
            //     // 10) Point_<T> operator/(const Point_<T> &a, U b);
        };

        /*エイリアス*/
        typedef Point_<int> Point2i;
        typedef Point2i Point;
        typedef Point_<float> Point2f;
        typedef Point_<double> Point2d;


        ////////////////////////////////////////////////////
        // Size_ 縦横プライマリーテンプレートクラス
        ////////////////////////////////////////////////////
        template <typename T>
        class Size_
        {
            T width_;
            T height_;

        public:
            Size_() : width_(T(0)), height_(T(0)) {}
            virtual ~Size_() = default;
            Size_(T width, T height) : width_(width), height_(height) {}
            Size_(const Point_<T> &point) : width_(point.x_), height_(point.y_) {}
            Size_(const Size_<T> &other) = default;
            Size_ &operator=(const Size_<T> &other) = default;
            Size_(Size_<T> &&other) = default;
            Size_ &operator=(Size_<T> &&other) = default;

            T width() const { return width_; }
            T height() const { return height_; }
            T area() const { return width_ * height_; }
        };

        /*エイリアス*/
        typedef Size_<int> Size;
        typedef Size_<float> Sizef;
        typedef Size_<double> Sized;



        ////////////////////////////////////////////////////
        // Rect_ 2次元領域プライマリーテンプレートクラス
        ////////////////////////////////////////////////////
        template <typename T>
        class Rect_
        {
        public:
            T top_;
            T left_;
            T bottom_;
            T right_;

        public:
            Rect_()
                : top_(T(0)), left_(T(0)), bottom_(T(0)), right_(T(0)) {}
            virtual ~Rect_() = default;
            Rect_(T x, T y, T width, T height)
                : top_(y), left_(x), bottom_(y + height), right_(x + width) {}

            Rect_(const Rect_<T> &other) = default;
            Rect_<T> &operator=(const Rect_<T> &other) = default;
            Rect_(Rect_<T> &&other) = default;
            Rect_<T> &operator=(Rect_<T> &&other) = default;

            operator Rect_<int>() const
            {
                return Rect_<int>(saturate_clamp<int>(top_),
                                    saturate_clamp<int>(left_),
                                    saturate_clamp<int>(bottom_),
                                    saturate_clamp<int>(right_));
            }

            operator Rect_<float>() const
            {
                return Rect_<float>(saturate_clamp<float>(top_),
                                    saturate_clamp<float>(left_),
                                    saturate_clamp<float>(bottom_),
                                    saturate_clamp<float>(right_));
            }

            operator Rect_<double>() const
            {
                return Rect_<double>(saturate_clamp<double>(top_),
                                        saturate_clamp<double>(left_),
                                        saturate_clamp<double>(bottom_),
                                        saturate_clamp<double>(right_));
            }

            bool contains(const Point_<T> &point) const;
            T width() const;
            T height() const;
            Point_<T> left_top() const;
            Point_<T> right_bottom() const;
            T area() const { return width() * height(); }
        };

        /*エイリアス*/
        typedef Rect_<int> Rect;
        typedef Rect_<float> Rectf;
        typedef Rect_<double> Rectd;

        ////////////////////////////////////////////////////
        // Point3_ 3次元座標プライマリーテンプレートクラス
        ////////////////////////////////////////////////////
        template <typename T>
        class Point3_
        {
        public:
            T x_;
            T y_;
            T z_;

        public:
            Point3_() : x_(T(0)), y_(T(0)), z_(T(0)) {}
            Point3_(T x, T y, T z) : x_(x), y_(y), z_(z) {}
            virtual ~Point3_() = default;
            // explicit Point3_(const Point_<T> &point)
            //     : x_(point.x_), y_(point.y_), z_(T(0)) {}
            Point3_(const Point3_<T> &other) = default;
            Point3_ &operator=(const Point3_<T> &other) = default;
            Point3_(Point3_<T> &&other) = default;
            Point3_ &operator=(Point3_<T> &&other) = default;

            template <typename U>
            operator Point3_<U>() const
            {
                return Point3_<U>(saturate_clamp<U>(x_),
                                    saturate_clamp<U>(y_),
                                    saturate_clamp<U>(z_));
            }

            double norm() const { return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_); }
            double dot(const Point3_<T> &point) const { return std::sqrt(x_ * point.x_ + y_ * point.y_ + z_ * point.z_); }

            //     /*static operator*/
            //     // 1) Point3_<T> &operator+=(Point3_<T> &a, const Point3_<T> &b);
            //     // 2) Point3_<T> &operator-=(Point3_<T> &a, const Point3_<T> &b);
            //     // 3) Point3_<T> &operator*=(Point3_<T> &a, U b);
            //     // 4) Point3_<T> &operatpr/=(Point3_<T> &a, U b);
            //     // 5) bool operator==(const Point3_<T> &a, const Point3_<T> &b);
            //     // 6) bool operator!=(const Point3_<T> &a, const Point3_<T> &b);
            //     // 7) Point3_<T> operator+(const Point3_<T> &a, const Point3_<T> &b);
            //     // 8) Point3_<T> operator-(const Point3_<T> &a, const Point3_<T> &b);
            //     // 9) Point3_<T> operator*(const Point3_<T> &a, U b);
            //     // 10) Point3_<T> operator/(const Point3_<T> &a, U b);
        };

        /*エイリアス*/
        typedef Point3_<int> Point3i;
        typedef Point3i Point3;
        typedef Point3_<float> Point3f;
        typedef Point3_<double> Point3d;



        ////////////////////////////////////////////////////
        // Voxel_ 3次元領域プライマリーテンプレートクラス ※完成してない!!!
        ////////////////////////////////////////////////////
        template <typename T>
        class Voxel_
        {
            /* 3D Cube

            立体図
               p1 ---- p4
              /|      / |
            p2 ---- p3  |
            |  |    |   |
            |  p5 --|- p8
            | /     |  /
            p6 ---- p7

            展開図
                   p5 --- p8
                   | (6)  |
                   |      |
            p5 --- p1 --- p4 --- p8 --- p5
            | (3)  | (1)  | (4)  | (2)  |
            |      |      |      |      |
            p6 --- p2 --- p3 --- p7 --- p6
                   | (5)  |
                   |      |
                   p6 --- p7


            ファセット (点の指定順序は半時計回り)
            <plate-1>
            facet-1 : (p1, p2, p3)
            facet-2 : (p1, p3, p4)
            <plate-2>
            facet-3 : (p8, p7, p6)
            facet-4 : (p8, p6, p5)
            <plate-3>
            facet-5 : (p1, p5, p6)
            facet-6 : (p1, p6, p2)
            <plate-4>
            facet-7 : (p3, p7, p8)
            facet-8 : (p3, p8, p4)
            <plate-5>
            facet-9 : (p2, p6, p7)
            facet-10: (p2, p7, p3)
            <plate-6>
            facet-11: (p4, p8, p5)
            facet-12: (p4, p5, p1)
        */

        public:
            Point3_<T> p1_;
            Point3_<T> p2_;
            Point3_<T> p3_;
            Point3_<T> p4_;
            Point3_<T> p5_;
            Point3_<T> p6_;
            Point3_<T> p7_;
            Point3_<T> p8_;

        public:
            Voxel_() {}
            Voxel_(const Point3_<T> p1,
                    const Point3_<T> p2,
                    const Point3_<T> p3,
                    const Point3_<T> p4,
                    const Point3_<T> p5,
                    const Point3_<T> p6,
                    const Point3_<T> p7,
                    const Point3_<T> p8)
                : p1_(p1), p2_(p2), p3_(p3), p4_(p4), p5_(p5), p6_(p6), p7_(p7), p8_(p8) {}

            virtual ~Voxel_() = default;

            Voxel_(const Voxel_<T> &other) = default;
            Voxel_ &operator=(const Voxel_<T> &other) = default;
            Voxel_(Voxel_<T> &&other) = default;
            Voxel_ &operator=(Voxel_<T> &&other) = default;
        };
    }
}


#include <IsComputerVision/type-impl.hpp>