// GoogleTest
#include <gtest/gtest.h>


// Test Target
#include <IsComputerVision/optical/optical_params.hpp>

using namespace is::common;
using namespace is::nbla;
using namespace is::cv;


namespace
{
    TEST(cv_func, permissible_circle_of_confusion)
    {
        /*許容錯乱円*/
        double distance = 1200; // 1200[mm]
        std::cout << "WD " << distance << std::endl;
        std::cout << "許容錯乱円 " << permissible_circle_of_confusion(distance) << std::endl; 
    }

    TEST(cv_func, cvt_35mm_focus_length)
    {
        /*35mmフィルム換算の焦点距離*/
        double focus_length = 22.0; // 22[mm]
        double diag_dist = 18.0;    // 18[mm]
        std::cout << "焦点距離 " << focus_length << std::endl;
        std::cout << "撮像素子の対角長さ " << diag_dist << std::endl;
        std::cout << "35mmフィルム換算の焦点距離 " << cvt_35mm_focus_length(focus_length, diag_dist) << std::endl;
    }

    TEST(cv_func, lens_category)
    {
        /*レンズカテゴリ*/
        // 0~40mm:広角レンズ
        // 40~100mm:標準レンズ
        // 100mm~:望遠レンズ

        double focus_length = 22.0; // 22[mm]
        double diag_dist = 18.0;    // 18[mm]
        double focus_length_35mm = cvt_35mm_focus_length(focus_length, diag_dist);
        std::cout << "焦点距離 " << focus_length << std::endl;
        std::cout << "撮像素子の対角長さ " << diag_dist << std::endl;
        std::cout << "35mmフィルム換算の焦点距離 " << focus_length_35mm << std::endl;
        std::cout << lens_category(focus_length_35mm) << std::endl;
    }

    TEST(cv_func, angle_of_view)
    {
        /*画角*/
        double focus_length = 35.0; // 35[mm]
        double diag_dist = 43.0;    // 43[mm]
        double focus_length_35mm = cvt_35mm_focus_length(focus_length, diag_dist);
        std::cout << "焦点距離 " << focus_length << std::endl;
        std::cout << "撮像素子の対角長さ " << diag_dist << std::endl;
        std::cout << "35mmフィルム換算の焦点距離 " << focus_length_35mm << std::endl;
        std::cout << lens_category(focus_length_35mm) << std::endl;

        double aov = angle_of_view(focus_length_35mm, diag_dist);
        std::cout << "画角 " << aov << std::endl;
    }

    TEST(cv_func, effective_aperture)
    {
        /*有効口径*/
        double focus_length = 35.0; // 35[mm]
        double f_number = 2.1;

        std::cout << "焦点距離 " << focus_length << std::endl;
        std::cout << "F値 " << f_number << std::endl;

        double ea = effective_aperture(focus_length, f_number);
        std::cout << "有効口径 " << ea << std::endl;
    }

    TEST(cv_func, lens_na)
    {
        /*レンズ開口数(NA)*/
        // 明るさ, 分解能, 焦点距離の指標
        double focus_length = 35.0; // 35[mm]
        double diag_dist = 43.0;    // 43[mm]
        double reflective_index = 1.000292; // 空気
        std::cout << "焦点距離 " << focus_length << std::endl;
        std::cout << "撮像素子の対角長さ " << diag_dist << std::endl;
        std::cout << "空気の屈折率 " << reflective_index << std::endl;
        double na = lens_na(focus_length, diag_dist, reflective_index);
        std::cout << "空気中のレンズの開口数(NA) " << na << std::endl;
    }
}


int main(int, char **)
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}