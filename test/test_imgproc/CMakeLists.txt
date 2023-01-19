cmake_minimum_required(VERSION 3.14.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/test_case.cmake)

make_test_case(test_cv_io.cpp TEST_CV_IO)
make_test_case(test_cv_padding.cpp TEST_CV_PADDING)
make_test_case(test_cv_saturate_cast.cpp TEST_CV_SATURATE_CAST)
make_test_case(test_cv_blur.cpp TEST_CV_BLUR)
make_test_case(test_cv_edge_detector.cpp TEST_CV_EDGE_DETECTOR)
make_test_case(test_cv_opt.cpp TEST_CV_OPTICAL)



