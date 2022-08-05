cmake_minimum_required(VERSION 3.14.6)

# Get dirname and set it to libname.
get_filename_component(DIRNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(LIB_TARGET_NAME ${DIRNAME})

project(${LIB_TARGET_NAME} LANGUAGES C CXX VERSION ${version_major}.${version_minor}.${version_patch})

message(STATUS ">>>>> ${LIB_TARGET_NAME}")

# Sources
file(GLOB LIB_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    ./*.cpp
    memory/*.cpp
    array/*.cpp
    function/generic/*.cpp
)

# Library
if(BUILD_CPP_SHARED_LIBS)
    add_library(${LIB_TARGET_NAME} SHARED ${LIB_SOURCES})
else()
    add_library(${LIB_TARGET_NAME} STATIC ${LIB_SOURCES})
endif()



if(MSVC)
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

# include
target_include_directories(${LIB_TARGET_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third-party/eigen-3.4.0
)



set(IS_NDARRAY_TARGET_NAME ${LIB_TARGET_NAME} PARENT_SCOPE)

message(STATUS "<<<<< ${LIB_TARGET_NAME}")
