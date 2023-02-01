cmake_minimum_required(VERSION 3.14.6)

# Get dirname and set it to libname.
get_filename_component(DIRNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(LIB_TARGET_NAME ${DIRNAME})

project(${LIB_TARGET_NAME} LANGUAGES C CXX VERSION ${version_major}.${version_minor}.${version_patch})

message(STATUS ">>>>> ${LIB_TARGET_NAME}")


# Sources
file(GLOB LIB_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ./*.cpp
    ./io/*.cpp
    ./filter/blur/*.cpp
    ./filter/edge_detector/*.cpp
#    ./filter/sharpness/*.cpp
    ./camera/*.cpp
)


# Library
if(BUILD_CPP_SHARED_LIBS)
    add_library(${LIB_TARGET_NAME} SHARED ${LIB_SOURCES})
else()
    add_library(${LIB_TARGET_NAME} STATIC ${LIB_SOURCES})
endif()


if(MSVC)
    # Win32 sources
    file(GLOB LIB_WIN32_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
        ./win32/*.cpp
    )
    message(STATUS "Win32 Sources: ${LIB_WIN32_SOURCES}")
    
    target_sources(${LIB_TARGET_NAME} PRIVATE 
        ${LIB_WIN32_SOURCES}
    )

    # PDBファイルを出力する
    set_target_properties(${LIB_TARGET_NAME} PROPERTIES
        COMPILE_PDB_NAME ${LIB_TARGET_NAME}
    )
    
    # Window(10以降)の場合で, デフォルト(ANSI)コードページ(FileIOなどのエンコード)を強制的にUTF-8にする.
    # プログラム内部からはsetlocale(LC_ALL, ""), std::locale::global(std::locale(""))などで, 
    # システムのデフォルトコードページにすると, OS側でANSIコードページがUTF-8の場合、プログラムに反映できる.
    # ここでは外部(CMake)から設定する方式となる.
    # if(BUILD_CPP_MSVC_DEFAULT_LOCALE_UTF8)
    #     list(APPEND LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../../build_tool/ansi_system_locale_utf8-manifext.xml)
    # endif()
endif()





# EXPORTS or dbg_EXPORTS 定義済みマクロを設定 for windows *.dll
target_compile_definitions(${LIB_TARGET_NAME} PUBLIC
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:${LIB_TARGET_NAME}_EXPORTS>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Debug>>:${LIB_TARGET_NAME}_dbg_EXPORTS>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:MinSizeRel>>:${LIB_TARGET_NAME}_EXPORTS>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:RelWithDebgInfo>>:${LIB_TARGET_NAME}_dbg_EXPORTS>
)


################################################################################
# リンク
################################################################################
target_link_libraries(${LIB_TARGET_NAME} PUBLIC
    ${IS_COMMON_TARGET_NAME}
    ${IS_NDARRAY_TARGET_NAME}
)

# 依存関係
add_dependencies(${LIB_TARGET_NAME}
    ${IS_COMMON_TARGET_NAME}
    ${IS_NDARRAY_TARGET_NAME}
)

# # dllファイルを実行ファイルと同じフォルダにコピー
# add_custom_command(TARGET ${LIB_TARGET_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:myzlib> $<TARGET_FILE_DIR:${LIB_TARGET_NAME}>
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:mylibpng> $<TARGET_FILE_DIR:${LIB_TARGET_NAME}>
# )

###############################################################################
# OpenCVライブラリのリンク
###############################################################################
set(OpenCV_DIR "")
if(MSVC)
    # 環境変数からOpenCVのビルドパスを探す
    if($ENV{OpenCV_DIR} STREQUAL "")
        message(FATAL_ERROR "Not found OpenCV_DIR env path.")
    else()
        set(OpenCV_DIR $ENV{OpenCV_DIR})
        string(REPLACE "\\" "/" OpenCV_DIR $ENV{OpenCv_DIR})
        message(STATUS "OpenCV_DIR: ${OpenCV_DIR}")
    endif()
    
endif()

find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    target_include_directories(${LIB_TARGET_NAME} PUBLIC 
        ${OpenCV_INCLUDE_DIRS}
    )
    target_link_libraries(${LIB_TARGET_NAME} PUBLIC 
        ${OpenCV_LIBS}
    )
endif()


set(IS_COMPUTERVISION_TARGET_NAME ${LIB_TARGET_NAME} PARENT_SCOPE)

message(STATUS "<<<<< ${LIB_TARGET_NAME}")
