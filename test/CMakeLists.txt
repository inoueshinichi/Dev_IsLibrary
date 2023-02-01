cmake_minimum_required(VERSION 3.14.6)

# GoogleTest
include(${CMAKE_CURRENT_SOURCE_DIR}/../build-tool/cmake/googletest.cmake)
BuildGoogleTest("GoogleTest" "1.12.0" ON) # 1.12.0 : >= C+14

if((TARGET GTest::gtest)      AND
   (TARGET GTest::gmock)      AND
   (TARGET GTest::gtest_main) AND
   (TARGET GTest::gmock_main))
   message(STATUS "Enter each test projects... bacause of exsising googletest library.")

   include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # include test_utils.hpp

   add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tutorial_googletest)
   add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test_nnabla)
   add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test_cv)
endif()
