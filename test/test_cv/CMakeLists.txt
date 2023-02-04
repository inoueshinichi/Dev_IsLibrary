cmake_minimum_required(VERSION 3.14.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/test_case.cmake)

make_test_case(TEST_CV_IO test_cv_io.cpp)
make_test_case(TEST_CV_PADDING test_cv_padding.cpp)
make_test_case(TEST_CV_SATURATE_CAST test_cv_saturate_cast.cpp)
make_test_case(TEST_CV_BLUR test_cv_blur.cpp)
make_test_case(TEST_CV_EDGE_DETECTOR test_cv_edge_detector.cpp)
make_test_case(TEST_CV_OPTICAL test_cv_opt.cpp )



