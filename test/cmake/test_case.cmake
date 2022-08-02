cmake_minimum_required(VERSION 3.14.6)


function(make_test_case SOURCE_CODE TEST_TARGET_EXE)

    # find_package(Python3 COMPONENTS Development NumPy)

    # Exe file
    add_executable(${TEST_TARGET_EXE}
        ${SOURCE_CODE}
    )

    # Include
    target_include_directories(${TEST_TARGET_EXE} PRIVATE
        ${Python3_INCLUDE_DIRS}
        ${Python3_NumPy_INCLUDE_DIRS}
    )

    # Libarary-link
    target_link_libraries(${TEST_TARGET_EXE} PRIVATE
        ${IS_COMMON_TARGET_NAME}
        ${IS_NDARRAY_TARGET_NAME}
        ${IS_COMPUTERVISION_TARGET_NAME}
        GTest::gtest
        GTest::gmock
        # Python3::Python
        # Python3::NumPy
    )

    # Build-order
    add_dependencies(${TEST_TARGET_EXE}
        ${IS_COMMON_TARGET_NAME}
        ${IS_NDARRAY_TARGET_NAME}
        ${IS_COMPUTERVISION_TARGET_NAME}
        GTest::gtest 
        GTest::gmock
    )

    # dllファイルを実行ファイルと同じフォルダにコピー
    add_custom_command(TARGET ${TEST_TARGET_EXE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${IS_COMMON_TARGET_NAME}> $<TARGET_FILE_DIR:${TEST_TARGET_EXE}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${IS_NDARRAY_TARGET_NAME}> $<TARGET_FILE_DIR:${TEST_TARGET_EXE}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${IS_COMPUTERVISION_TARGET_NAME}> $<TARGET_FILE_DIR:${TEST_TARGET_EXE}>
    )

    # ###############################################################################
    # # OpenCVライブラリのリンク
    # ###############################################################################
    # find_package(OpenCV REQUIRED)
    # if(OpenCV_FOUND)
    #     target_include_directories(${TEST_TARGET_EXE} PUBLIC 
    #         ${OpenCV_INCLUDE_DIRS}
    #     )
    #     target_link_libraries(${TEST_TARGET_EXE} PUBLIC 
    #         ${OpenCV_LIBS}
    #     )
    # endif()

endfunction()