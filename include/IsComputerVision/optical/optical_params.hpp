/**
 * @file optical_params.hpp
 * @author your name (you@domain.com)
 * @brief 光学パラメータの計算
 * @version 0.1
 * @date 2021-11-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <IsComputerVision/IsComputerVision.hpp>

//#include <numbers>
#include <cmath>
#include <tuple>

/**
 * [OK] 許容錯乱円
 * [OK] 35mmフィルム換算の焦点距離
 * [OK] レンズカテゴリ
 * [OK] 画角
 * [OK] 有効口径
 * [OK] 開口数(NA)
 * [] 被写界深度
 * [] 分解能
 * [] F値
 * [] 焦点深度
 * []
 */


namespace is {
    namespace cv {

        /**
         * @brief 許容錯乱円
         * 
         * @param distance 
         * @return double 
         */
        inline double permissible_circle_of_confusion(double distance) {
            return distance / 1500.0;
        }


        /**
         * @brief 35mmフィルム換算の焦点距離
         * 
         * @param focus_length 
         * @param diag_distance 
         * @return double 
         */
        inline double cvt_35mm_focus_length(double focus_length, double diag_distance) {
            return 43.3 * focus_length / diag_distance;
        }


        /**
         * @brief レンズカテゴリ
         * 
         * @param focus_length_of_35mm 
         * @return std::string 
         */
        inline std::string lens_category(double focus_length_of_35mm) {
            // 0~40mm:広角レンズ
            // 40~100mm:標準レンズ
            // 100mm~:望遠レンズ
	        std::string category;
	        if (focus_length_of_35mm < 40) category = "広角レンズ";
	        else if (focus_length_of_35mm > 100) category = "望遠レンズ";
	        else category = "標準レンズ";

	        return category;
        }


        /**
         * @brief 画角
         * 
         * @param focus_length 
         * @param diag_dist 撮像素子の対角距離
         * @return double 
         */
        inline double angle_of_view(double focus_length, double diag_dist) {
            double r2d = 180.0 / /*std::numbers::pi*/ M_PI;
            return 2 * std::atan2(diag_dist, 2 * focus_length) * r2d;
        }


        /**
         * @brief 有効口径
         * 
         * @param focus_length 
         * @param f_number 
         * @return double 
         */
        inline double effective_aperture(double focus_length, double f_number) {
            return focus_length / f_number;
        }


        /**
         * @brief 開口数(NA)
         * NAが大きいほど明るいレンズ
         * 0 < NA < 1
         * 
         * @param focus_length 
         * @param diag_dist 
         * @param reflective_index 
         * @return double 
         */
        inline double lens_na(double focus_length, double diag_dist, double reflective_index = 1.000292)
        {
            /**
             * @brief 屈折率 
             * 空気       1.000292 (0℃ 1気圧)
             * 二酸化炭素 1.00045
             * 氷         1.309    (0℃)
             * 水         1.3334   (20℃)
             * ダイアモンド 2.417
             */
            double aov = angle_of_view(focus_length, diag_dist);
            double na = reflective_index * std::sin(aov * M_PI / 180.0);
            return na;
        }


        /**
         * @brief 被写界深度
         * 
         * @param permissible_circle_of_confusion 
         * @param work_distance 
         * @param f_number 
         * @param focus_length 
         * @return std::tuple<double, double, double> 
         */
        inline std::tuple<double, double, double>
        depth_of_fields(double permissible_circle_of_confusion, double work_distance, 
                        double f_number, double focus_length) 
        {
            double diff_len = (work_distance - focus_length);
            double coeff = permissible_circle_of_confusion * diff_len * f_number;

            // 前側被写界深度
            double front_depth = coeff * diff_len / (focus_length * 2 + coeff);

            // 後側被写界深度
            double back_depth = coeff * diff_len / (focus_length * 2 - coeff);

            // 総被写界深度
            double total_depth = front_depth + back_depth;

            return std::make_tuple(front_depth, back_depth, total_depth);
        }
    }
}