cmake_minimum_required(VERSION 3.14.6)

set(TEST_TARGET_EXE "tutorial_googletest")

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/test_case.cmake)

set(TEST_SOURCES 
    test_gtest.cpp
    deal.hpp
    deal.cpp
    account.hpp
    mock_account.hpp
)

make_test_case(${TEST_TARGET_EXE} ${TEST_SOURCES})
