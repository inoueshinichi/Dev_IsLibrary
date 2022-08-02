cmake_minimum_required(VERSION 3.14.6)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/test_case.cmake)


make_test_case(test_imgproc_io.cpp TEST_IMGPROC_IO)
make_test_case(test_imgproc_padding.cpp TEST_IMGPROC_PADDING)
make_test_case(test_imgproc_saturate_cast.cpp TEST_IMGPROC_SATURATE_CAST)
make_test_case(test_imgproc_blur.cpp TEST_IMGPROC_BLUR)
make_test_case(test_imgproc_edge_detector.cpp TEST_IMGPROC_EDGE_DETECTOR)
make_test_case(test_imgproc_opt.cpp TEST_IMGPROC_OPT)



