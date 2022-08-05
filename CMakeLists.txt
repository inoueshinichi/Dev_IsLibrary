cmake_minimum_required(VERSION 3.14.6)

###############################################################################
# Version
###############################################################################
# Project Version
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/VERSION.txt versions NEWLINE_CONSUME)
string(REGEX REPLACE [[^([A-z0-9.\-_]+).*$]] [[\1]] version ${versions})
string(REGEX REPLACE [[^([0-9]+)\.([0-9]+)\.(.*)$]] [[\1]] version_major ${version})
string(REGEX REPLACE [[^([0-9]+)\.([0-9]+)\.(.*)$]] [[\2]] version_minor ${version})
string(REGEX REPLACE [[^([0-9]+)\.([0-9]+)\.(.*)$]] [[\3]] version_patch ${version})

# Project
get_filename_component(DIRNAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(LIB_TARGET_NAME ${DIRNAME})
project(${LIB_TARGET_NAME} LANGUAGES C CXX VERSION ${version_major}.${version_minor}.${version_patch})

message(STATUS "========== [START] Version Management ==========")
message(STATUS "CMake Ver: ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}.${CMAKE_TWEAK_VERSION}")
message(STATUS "Project Ver: ${version_major}.${version_minor}.${version_patch}")
message(STATUS "========== [END] Version Management ==========")

message(STATUS ">>>>> ${PROJECT_NAME}")


if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
 
    ###############################################################################
    # 全ターゲットに影響を与えるオプション
    ###############################################################################
    message(STATUS "========== [START] C++ All Target Options ==========")

    # Switching Option
    option(BUILD_PYTHON_INTERPRETER "Enable/Disable: Python interpreter" OFF)
    option(BUILD_CPP_OPENMP "Enable/Disable: OpenMP" OFF)
    option(BUILD_CPP_TEST "Enable/Disable: Building Test" ON)
    option(BUILD_CPP_SHARED_LIBS "Enable/Disable: Shared Libraries" ON)
    option(BUILD_CPP_ZERO_CHECK "Enable/Disable: ZERO_CHECK" OFF)
    option(BUILD_CPP_MSVC_DEFAULT_LOCALE_UTF8 "Enable/Disable utf-8 default locale" OFF) # ANSI: ShiftJIS -> UTF8 @ Windows OS
    option(BUILD_CPP_MSVC_STATIC_VCRUNTIME "Enable/Disable Static MSVC Runtime" OFF) # MD, MDd -> MT, MTd

    # ZERO_CHECKプロジェクトの有無
    if(NOT BUILD_CPP_ZERO_CHECK)
        set(CMAKE_SUPPRESS_REGENERATION true)
    endif()

    # C++20
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

    # ライブラリフラグ
    set(CMAKE_RELEASE_POSTFIX "")
    set(CMAKE_DEBUG_POSTFIX d)
    set(CMAKE_MINSIZEREL_POSTFIX _minsizerel)
    set(CMAKE_RELWITHDEBINFO_POSTFIX _relwithdebinfo)

    

    # プラットフォーム毎のコンパイラオプション(マルチ・コンフィギュレーションに対応)
    if (MSVC)
        message(STATUS "Compiler: MSVC in Windows")

        # Build Options
        string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # デフォルトの`/W[0-4]`を削除
        set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /GR /EHsc /W4")
        set(CMAKE_CXX_FLAGS_DEBUG "/Ob0 /Od /MDd /Zi /RTC1") # /MDd or /Od が指定されている場合、#include <windows>内部で_DEBUGマクロが定義される
        set(CMAKE_CXX_FLAGS_RELEASE "/Ob2 /O2 /MD /DNDEBUG") # /DNDEBUGマクロはstatic_assert()で利用する
        set(CMAKE_CXX_FLAGS_MINSIZEREL "/O1 /MD /DNDEBUG")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Od /MDd /Zi /RTC1")

        # Select VC Runtime (Static or Shared)
        if(BUILD_CPP_MSVC_STATIC_VCRUNTIME)
            string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
            string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
            string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")
            string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
        endif()

        if(BUILD_CPP_OPENMP)
            message(STATUS "Enable OpenMP")
            find_package(OpenMP REQUIRED)
            include_directories(${OpenMP_INCLUDE_DIRS})
            link_directories(${OpenMP_LIB_DIRS})
            add_compile_options("$<$<C_COMPILER_ID:MSVC>:/openmp>")
            add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/openmp>")
        endif()

        # ソースコードでUTF-8を使う(MSVC)
        add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

        # C++11以降を利用した場合にマクロ変数__cplusplusの値を利用中のC++バージョンに合わせる(つけないとC++98相当の値になる)
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>")

        # オブジェクトファイル(*.obj)のサイズ制限を解放
        add_compile_options("$<$<C_COMPILER_ID:MSVC>:/bigobj>")
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/bigobj>")

        # プリプロセッサ準拠モードを有効にする
        # https://docs.microsoft.com/ja-jp/cpp/preprocessor/preprocessor-experimental-overview?view=msvc-160
        # https://docs.microsoft.com/ja-jp/cpp/build/reference/zc-preprocessor?view=msvc-160
        add_compile_options("$<$<C_COMPILER_ID:MSVC>:/Zc:preprocessor>")
        add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>")


        # MSVCの標準逸脱のコンパイラ仕様を回避する
        add_compile_definitions(_CRT_NONSTDC_NO_DEPRECATE) # mkdir, closeなどPOSIX関数でコンパイル時に警告発生（unsafe）を抑制.
        add_compile_definitions(_CRT_SECURE_NO_WARNINGS) # strcpy、sprinitfなどでコンパイル時に警告発生（unsafe）を抑制.
        add_compile_definitions(_USE_MATH_DEFINES) # math.hに定義されている円周率等の物理定数を有効化する.

    elseif(APPLE)
        message(STATUS "Compiler: Clang in macOS")    

        # message(FATAL_ERROR "No Implimentation of LLVM with Clang(++) in macOS.")

        # Build Options(マルチコンフィギュレーション対応)
        set(CMAKE_CXX_FLAGS "-O1")
        set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

        if(BUILD_CPP_OPENMP)
            message(STATUS "Enable OpenMP")
            find_package(OpenMP REQUIRED)
            include_directories(${OpenMP_INCLUDE_DIRS})
            link_directories(${OpenMP_LIB_DIRS})
            add_compile_options("$<$<C_COMPILER_ID:Clang>:-fopenmp>")
            add_compile_options("$<$<CXX_COMPILER_ID:Clang>:-fopenmp>")
        endif()

        # C++標準に準拠しない拡張機能を無効化
        # string(APPEND CMAKE_CXX_FLAGS " --pedantic-errors")

        string(APPEND CMAKE_CXX_FLAGS " -pedantic")
        string(APPEND CMAKE_CXX_FLAGS " -Wall")

    else()
        message(STATUS "Compiler: GNU in Linux")

        # 詳細情報の表示
        set(CMAKE_VERBOSE_MAKEFILE ON)

        # Build Options(マルチコンフィギュレーション対応)
        set(CMAKE_CXX_FLAGS "-O1")
        set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

        if(BUILD_CPP_OPENMP)
            message(STATUS "Enable OpenMP")
            find_package(OpenMP REQUIRED)
            include_directories(${OpenMP_INCLUDE_DIRS})
            link_directories(${OpenMP_LIB_DIRS})
            add_compile_options("$<$<C_COMPILER_ID:GNU>:-fopenmp>")
            add_compile_options("$<$<CXX_COMPILER_ID:GNU>:-fopenmp>")
        endif()

        # C++標準に準拠しない拡張機能を無効化
        string(APPEND CMAKE_CXX_FLAGS " --pedantic-errors")

        # https://stackoverflow.com/questions/5088460/flags-to-enable-thorough-and-verbose-g-warnings/9862800#9862800
        string(APPEND CMAKE_CXX_FLAGS " -pedantic")
        string(APPEND CMAKE_CXX_FLAGS " -Wall")
        # string(APPEND CMAKE_CXX_FLAGS " -Wextra")
        # string(APPEND CMAKE_CXX_FLAGS " -Wcast-align")
        # string(APPEND CMAKE_CXX_FLAGS " -Wcast-qual")
        # string(APPEND CMAKE_CXX_FLAGS " -Wctor-dtor-privacy")
        # string(APPEND CMAKE_CXX_FLAGS " -Wdisabled-optimization")
        # string(APPEND CMAKE_CXX_FLAGS " -Wformat=2 ")
        # string(APPEND CMAKE_CXX_FLAGS " -Winit-self")
        # string(APPEND CMAKE_CXX_FLAGS " -Wlogical-op")
        # string(APPEND CMAKE_CXX_FLAGS " -Wmissing-declarations")
        # string(APPEND CMAKE_CXX_FLAGS " -Wmissing-include-dirs")
        # string(APPEND CMAKE_CXX_FLAGS " -Wnoexcept")
        # string(APPEND CMAKE_CXX_FLAGS " -Wold-style-cast")
        # string(APPEND CMAKE_CXX_FLAGS " -Woverloaded-virtual")
        # string(APPEND CMAKE_CXX_FLAGS " -Wredundant-decls")
        # string(APPEND CMAKE_CXX_FLAGS " -Wshadow")
        # string(APPEND CMAKE_CXX_FLAGS " -Wsign-conversion")
        # string(APPEND CMAKE_CXX_FLAGS " -Wsign-promo")
        # string(APPEND CMAKE_CXX_FLAGS " -Wstrict-null-sentinel")
        # string(APPEND CMAKE_CXX_FLAGS " -Wstrict-overflow=5")
        # string(APPEND CMAKE_CXX_FLAGS " -Wswitch-default")
        # string(APPEND CMAKE_CXX_FLAGS " -Wundef")
        # string(APPEND CMAKE_CXX_FLAGS " -Werror")
        # string(APPEND CMAKE_CXX_FLAGS " -Wno-unused")

    endif()

    if(UNIX)
        message(STATUS "---------- Compiler Options for Single Configuration such as Makefile, Ninja ----------")
    else()
        message(STATUS "---------- Compiler Options for Multi Configuration such as XCode, MSVC ----------")
    endif()

    message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
    message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
    message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
    message(STATUS "CMAKE_CXX_FLAGS_MINSIZEREL: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
    message(STATUS "CMAKE_CXX_FLAGS_RELWITHDEBINFO: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

    message(STATUS "========== [END] C++ All Target Options ==========")

endif()

###############################################################################
# 前処理
###############################################################################
message(STATUS "========== [START] Preprocessing ==========")

if(BUILD_PYTHON_INTERPRETER)

    # pythonコマンドをCMakeで使えるようにする
    set(PYTHON_COMMAND_NAME "python") # どのインタープリタが選ばれるかはPCの設定次第. もっと詳細設定をコマンドライン引数から与えても良いと思う.
    find_program(PYTHON_INTERPRETER ${PYTHON_COMMAND_NAME})
    if(NOT EXISTS ${PYTHON_INTERPRETER})
        message(FATAL_ERROR "Python [${PYTHON_INTERPRETER}] not found.")
        error() # Place Unknown command to stop cmake process here.
    endif()
    set(PYTHON_COMMAND "${PYTHON_INTERPRETER}")
    message(STATUS "Python3 Interpreter: ${PYTHON_INTERPRETER}")

endif()

message(STATUS "========== [END] Preprocessing ==========")
  
        
###############################################################################
# C++ core
###############################################################################
message(STATUS "========== [START] C++ Build Core ==========")

# Third-party
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party)

# Include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source
set(IS_COMMON_TARGET_NAME "")
set(IS_NDARRAY_TARGET_NAME "")
set(IS_COMPUTERVISION_TARGET_NAME "")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IsCommon)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IsNdArray)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/IsComputerVision)

if(NOT ${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    set(IS_COMMON_TARGET_NAME ${IS_COMMON_TARGET_NAME} PARENT_SCOPE)
    set(IS_NDARRAY_TARGET_NAME ${IS_NDARRAY_TARGET_NAME} PARENT_SCOPE)
    set(IS_COMPUTERVISION_TARGET_NAME ${IS_COMPUTERVISION_TARGET_NAME} PARENT_SCOPE)
endif()

# Version @ Windows
if(MSVC)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
        ${CMAKE_CURRENT_BINARY_DIR}/version-utf8.rc
    )

    execute_process(
        COMMAND ${PYTHON_INTERPRETER}
        ${CMAKE_CURRENT_SOURCE_DIR}/build-tools/utf8_to_utf16.py
        ${CMAKE_CURRENT_BINARY_DIR}/version-utf8.rc ${CMAKE_CURRENT_BINARY_DIR}/version.rc
    )

    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
endif()


   
message(STATUS "========== [END] C++ Build Core ==========")


###############################################################################
# C++ Testing
###############################################################################
message(STATUS "========== [START] Test ==========")


if(BUILD_CPP_TEST)
include(CTest)
    enable_testing()
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test) # Google Test
endif()

# CPackがよくわからないので後々調査すること
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

message(STATUS "========== [END] Test ==========")


###############################################################################
# Python Binding
###############################################################################
#add_subdirectory(${CMAKE_SOURCE_DIR}/python)


message(STATUS "<<<<< ${PROJECT_NAME}")