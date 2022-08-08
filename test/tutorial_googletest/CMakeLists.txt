cmake_minimum_required(VERSION 3.14.6)

set(TEST_TARGET_EXE "tutorial_googletest")

# Tutorial GoogleTest
add_executable(${TEST_TARGET_EXE}
    test_gtest.cpp
    deal.hpp
    deal.cpp
    account.hpp
    mock_account.hpp
)

target_link_libraries(${TEST_TARGET_EXE}
    GTest::gtest
    GTest::gmock
)

add_dependencies(${TEST_TARGET_EXE} 
    GTest::gtest 
    GTest::gmock
)


# dllファイルを実行ファイルと同じフォルダにコピー
add_custom_command(TARGET ${TEST_TARGET_EXE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:GTest::gtest> $<TARGET_FILE_DIR:${TEST_TARGET_EXE}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:GTest::gmock> $<TARGET_FILE_DIR:${TEST_TARGET_EXE}>
)