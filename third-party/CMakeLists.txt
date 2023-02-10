cmake_minimum_required(VERSION 3.14.6)

###############################################################################
# 以下に追加したいサードパーティ製のライブラリを明記 Download
###############################################################################
include(${CMAKE_CURRENT_SOURCE_DIR}/../build-tool/cmake/download_and_extract.cmake)

# eigen-3.4.0 ------------------------------------------------------------------------
# https://eigen.tuxfamily.org/index.php?title=Main_Page
# https://gitlab.com/libeigen/eigen
set(EIGEN_DIRNAME eigen-3.4.0)
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${EIGEN_DIRNAME}")
    download_and_extract_library(${EIGEN_DIRNAME} .zip https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip)
endif()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${EIGEN_DIRNAME})


# opencv_contrib-4.5.3-----------------------------------------------------------------------
set(OPENCV_CONTRIB_DIRNAME opencv_contrib-4.5.3)
# https://github.com/opencv/opencv_contrib
# https://atmarkit.itmedia.co.jp/ait/articles/1709/12/news135.html
if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${OPENCV_CONTRIB_DIRNAME}")
    download_and_extract_library(${OPENCV_CONTRIB_DIRNAME} .zip https://github.com/opencv/opencv_contrib/archive/refs/tags/4.5.3.zip)
endif()


###############################################################################
# 以下に追加したいサードパーティ製のライブラリを明記 External Project
###############################################################################
include(ExternalProject)


# opencv-4.5.3 -----------------------------------------------------------------------
# https://opencv.org/releases/
# https://github.com/opencv/opencv/tree/4.5.3

# OpenCV CMake Build Option List
# ----------------------------------------------------------------------
# Name                                     | Value @note default config IN MY WINDOWS OS
# ----------------------------------------------------------------------
# ANT_EXECUTABLE                           : ANT_EXECUTABLE-NOTFOUND
# BUILD_CUDA_STUBS                         : OFF 
# BUILD_DOCS                               : OFF
# BUILD_EXAMPLES                           : OFF
# BUILD_IPP_IW                             : ON
# BUILD_ITT                                : ON
# BUILD_JASPER                             : ON
# BUILD_JAVA                               : ON
# BUILD_JPEG                               : ON
# BUILD_LIST                               : ""
# BUILD_OPENEXR                            : ON
# BUILD_OPENJPEG                           : ON
# BUILD_PAKCAGE                            : ON
# BUILD_PERF_TESTS                         : ON
# BUILD_PNG                                : ON
# BUILD_PROTOBUF                           : ON
# BUILD_SHARED_LIBS                        : ON
# BUILD_TBB                                : OFF
# BUILD_TESTS                              : ON
# BUILD_TIFF                               : ON
# BUILD_USE_SYMLINKS                       : OFF
# BUILD_WEBP                               : ON
# BUILD_WITH_DEBUG_INFO                    : OFF
# BUILD_WITH_DYNAMIC_IPP                   : OFF
# BUILD_WITH_STATIC_CRT                    : ON
# BUILD_ZLIB                               : ON
# BUILD_opencv_apps                        : ON
# BUILD_opencv_calib3d                     : ON
# BUILD_opencv_core                        : ON
# BUILD_opencv_dnn                         : ON
# BUILD_opencv_features2d                  : ON
# BUILD_opencv_flann                       : ON
# BUILD_opencv_gapi                        : ON
# BUILD_opencv_highgui                     : ON
# BUILD_opencv_imgcodecs                   : ON
# BUILD_opencv_imgproc                     : ON
# BUILD_opencv_java_bindings_generator     : ON
# BUILD_opencv_js                          : OFF
# BUILD_opencv_js_bindings_generator       : OFF
# BUILD_opencv_ml                          : ON
# BUILD_opencvobjc_bindings_generator      : ON
# BUILD_opencv_objdetect                   : ON
# BUILD_opencv_photo                       : ON
# BUILD_opencv_python3                     : ON
# BUILD_opencv_python_bindigs_generator    : ON
# BUILD_opencv_python_tests                : ON
# BUILD_opencv_stitching                   : ON
# BUILD_opencv_ts                          : ON
# BUILD_opencv_video                       : ON
# BUILD_opencv_videoio                     : ON
# BUILD_opencv_world                       : OFF
# CLAMDBLAS_INCLUDE_DIR                    : CLAMDBLAS_INCLUDE_DIR-NOTFOUND
# CLAMDBLAS_ROOT_DIR                       : CLAMDBLAS_ROOT_DIR-NOTFOUND
# CLAMDFFT_INCLUDE_DIR                     : CLAMDFFT_INCLUDE_DIR-NOTFOUND
# CLAMMFFT_ROOT_DIR                        : CLAMDFFT_ROOT_DIR-NOTFOUND
# CMAKE_CONFIGURATION_TYPES                : Debug;Release
# CMAKE_INSTALL_PREFIX                     : user/path/to/out-of-build
# CPU_BASELINE                             : SSE3
# CPU_DISPATCH                             : SSE4_1;SSE4_2;AVX;FP16;AVX2;AVX512_SKX
# CV_DISABLE_OPTIMIZATION                  : OFF
# CV_ENABLE_INTRINSICS                     : ON
# CV_TRACE                                 : ON
# DC1394_INCLUDE                           : DC1394_INCLUDE-NOTFOUND
# DC1394_LIBRARY                           : DC1394_LIBRARY-NOTFOUND
# EIGEN_INCLUDE_PATH                       : EIGEN_INCLUDE_PATH-NOTFOUND
# ENABLE_BUILD_HARDENING                   : OFF
# ENABLE_BUILD_CCACHE                      : OFF
# ENABLE_CONFIG_VERIFICATION               : OFF
# ENABLE_FAST_MATH                         : OFF
# ENABLE_FLAKE8                            : OFF
# ENABLE_IMPL_COLLECTION                   : OFF
# ENABLE_INSTRUMENTATION                   : OFF
# ENABLE_LTO                               : OFF
# ENABLE_NOISY_WARNINGS                    : OFF
# ENABLE_PIC                               : ON
# ENABLE_PERCOMPILED_HEADERS               : ON
# ENABLE_PYLINT                            : OFF
# ENABLE_SOLUTION_FOLDERS                  : ON
# EXCUTABLE_OUTPUT_PATH                    : user/path/to/out-of-build/bin
# Eigen3_DIR                               : Eigen3_DIR-NOTFOUND
# GSTREAMER_app_LIBRARY                    : GSTREAMER_app_LIBRARY-NOTFOUND
# GSTREAMER_base_LIBRARY                   : GSTREAMER_base_LIBRARY-NOTFOUND
# GSTREAMER_glib_INCLUDE_DIR               : GSTREAMER_glib_INCLUDE_DIR-NOTFOUND
# GSTREAMER_glib_LIBRARY                   : GSTREAMER_glib_LIBRARY-NOTFOUND
# GSTREAMER_glibconfig_INCLUDE_DIR         : GSTREAMER_glibconfig_INCLUDE_DIR-NOTFOUND
# GSTREAMER_gobject_LIBRARY                : GSTREAMER_gobject_LIBRARY-NOTFOUND
# GSTREAMER_gst_INCLUDE_DIR                : GSTREAMER_gst_INCLUDE_DIR-NOTFOUND
# GSTREAMER_gstreamer_LIBRARY              : GSTREAMER_gstreamer_LIBRARY-NOTFOUND
# GSTREAMER_pbutils_LIBRARY                : GSTREAMER_pbutils_LIBRARY-NOTFOUND
# GSTREAMER_riff_LIBRARY                   : GSTREAMER_riff_LIBRARY-NOTFOUND
# GSTREAMER_video_LIBRARY                  : GSTREAMER_video_LIBRARY-NOTFOUND
# INSTALL_CREATE_DISTRIB                   : OFF
# INSTALL_C_EXAMPLES                       : OFF
# INSTALL_PDB                              : ON
# INSTALL_PDB_COMPONENT_EXECLUDE_FROM_ALL  : ON
# INSTALL_PYTHON_EXAMPLES                  : OFF
# INSTALL_TESTS                            : OFF
# LAPACK_CBLAS_H                           : ""
# LAPACK_IMPL                              : Unknown
# LAPACK_INCLUDE_DIR                       : ""
# LAPACK_LIBRARIES                         : ""
# MKL_ROOT_DIR                             : MKL_ROOT_DIR-NOTFOUND
# MKL_USE_SINGLE_DYNAMIC_LIBRARY           : OFF
# MKL_WITH_OPENMP                          : OFF
# MKL_WITH_TBB                             : OFF
# M_LIBRARY                                : M_LIBRARY-NOTFOUND
# OPENCL_FOUND                             : ON
# OPENCV_CMAKE_MACRO_WIN32_WINNT           : 0x0601
# OPENCV_CONFIG_FILE_INCLUDE_DIR           : user/path/to/out-of-build
# OPENCV_DISABLE_FILESYSTEM_SUPPORT        : OFF
# OPENCV_DNN_CUDA                          : OFF
# OPENCV_DNN_OPENCL                        : ON
# OPENCV_DNN_PERF_CAFFE                    : OFF
# OPENCV_DNN_PERF_CLCAFFE                  : OFF
# OPENCV_DOWNLOAD_PATH                     : user/path/to/download-path
# OPENCV_DUMP_HOOOKS_FLOW                  : OFF
# OPENCV_ENABLE_ALLOCATOR_STATS            : ON
# OPENCV_ENABLE_ATOMIC_LONG_LONG           : ON
# OPENCV_ENABLE_MEMALIGN                   : ON
# OPENCV_ENABLE_MEMORY_SANITIZER           : OFF
# OPENCV_ENABLE_NONFREE                    : OFF
# OPENCV_EXTRA_MODULES_PATH                : user/path/to/opencv_contrib-path
# OPENCV_FORCE_3RDPARTY_BUILD              : OFF
# OPENCV_FORCE_PYTHON_LIBS                 : OFF
# OPENCV_GENERATE_PKGCONFIG                : OFF
# OPENCV_GENERATE_SETUPVARS                : ON
# OPENCV_IPP_GAUSSIAN_BLUR                 : OFF
# OPENCV_JAVA_SOURCE_VERSION               : ""
# OPENCV_JAVA_TARGET_VERSION               : ""
# OPENCV_MATHJAX_RELPATH                   : https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.0
# OPENCV_MSVC_PARALLEL                     : ON
# OPENCV_PYTHON3_VERSION                   : OFF
# OPENCV_TIMESTAMP                         : user data-time
# OPENCV_WARNINGS_ARE_ERRORS               : OFF
# OBJ_DISABLE_TPSOT_FIX                    : OFF
# OPJ_USE_THREAD                           : ON
# OpenCV_HAL_DIR                           : OpenCV_HAL_DIR-NOTFOUND
# PARALLEL_ENABLE_PLUGINS                  : ON
# PROTOBUF_UPDATE_FILES                    : OFF
# PYTHON2_EXECUTABLE                       : ""
# PYTHON2_INCLUDE_DIR                      : """
# PYTHON2_INCLUDE_DIR2                     : ""
# PYTHON2_LIBRARY                          : ""
# PYTHON2_LIBRARY_DEBUG                    : ""
# PYTHON2_NUMPY_INCLUDE_DIRS               : ""
# PYTHON2_PACKAGES_PATH                    : ""
# PYTHON3_EXECUTABLE                       : user/path/to
# PYTHON3_INCLUDE_DIR                      : user/path/to
# PYTHON3_INCLUDE_DIR2                     : user/path/to
# PYTHON3_LIBRARY                          : user/path/to
# PYTHON3_LIBRARY_DEBUG                    : PYTHON_DEBUG_LIBRARY-NOTFOUND
# PYTHON3_NUMPY_INCLUDE_DIRS               : user/path/to
# PYTHON3_PACKAGES_PATH                    : user/path/to
# USE_WIN32_FILEIO                         : ON
# VTK_DIR                                  : VTK_DIR-NOTFOUND
# WITH_1394                                : ON
# WITH_ADE                                 : ON
# WITH_ARITH_DEC                           : ON
# WITH_ARITH_ENC                           : ON
# WITH_CLP                                 : OFF
# WITH_CUDA                                : OFF
# WITH_DIRECTX                             : ON
# WITH_DSHOW                               : ON
# WITH_EIGEN                               : ON
# WITH_FFMPEG                              : ON
# WITH_FREETYPE                            : OFF
# WITH_GDAL                                : OFF
# WITH_GDCM                                : OFF
# WITH_GSTREAMER                           : ON
# WITH_HALIDE                              : OFF
# WITH_HPX                                 : OFF
# WITH_IMGCODEC_HDR                        : ON
# WITH_IMGCODEC_PFM                        : ON
# WITH_IMGCODEC_PXM                        : ON
# WITH_IMGCODEC_SUNRASTER                  : ON
# WITH_INF_ENGINE                          : OFF
# WITH_IPP                                 : ON
# WITH_ITT                                 : ON
# WITH_JASPER                              : ON
# WITH_LAPACK                              : ON
# WITH_LIBREALSENSE                        : OFF
# WITH_MFX                                 : OFF
# WITH_MSMF                                : ON
# WITH_MSMF_DXVA                           : ON
# WITH_NGRAPH                              : OFF
# WITH_ONNX                                : OFF
# WITH_OPENCL                              : ON
# WITH_OPENCLAMDBLAS                       : ON
# WITH_OPENCLAMDFFT                        : ON
# WITH_OPENCL_D3D11_NV                     : ON
# WITH_OPENCL_SVM                          : OFF
# WITH_OPENEXR                             : ON
# WITH_OPENGL                              : OFF
# WITH_OPENJPEG                            : ON
# WITH_OPENMP                              : OFF
# WITH_OPENNI                              : OFF
# WITH_OPENNI2                             : OFF
# WITH_OPENVX                              : OFF
# WITH_PLAIDML                             : OFF
# WITH_PNG                                 : ON
# WITH_PROTOBUF                            : ON
# WITH_PVAPI                               : OFF
# WITH_QT                                  : OFF
# WITH_QUIRC                               : ON
# WITH_TBB                                 : OFF
# WITH_TIFF                                : ON
# WITH_UEYE                                : OFF
# WITH_VTK                                 : ON
# WITH_VULKAN                              : OFF
# WITH_WEBP                                : ON
# WITH_WIN32UI                             : ON
# WITH_XIMEA                               : OFF
# ccitt                                    : ON
# logluv                                   : ON
# lzw                                      : ON
# mdi                                      : ON
# next                                     : ON
# old-jpeg                                 : OFF
# packbits                                 : ON
# thunder                                  : ON

set(OPENCV_DIRNAME opencv-4.5.3)
message(STATUS "Download and extract library: ${OPENCV_DIRNAME}")

# Setting opencv Project
set(OPENCV_TARGET ${OPENCV_DIRNAME})
set(OPENCV_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${OPENCV_TARGET})
set(OPENCV_INSTALL ${OPENCV_PREFIX}/install)

ExternalProject_Add(
        # Taget Name
        ${OPENCV_TARGET}

    #################### No.0 Common Directory Path ####################    
    # 一時ファイルを保存するディレクトリパス
    TMP_DIR ${OPENCV_PREFIX}/tmp
    # 各ステップで生成されるタイムスタンプを保存するディレクトリパス
    STAMP_DIR ${OPENCV_PREFIX}/stamp
    # 各ステップでのLogファイルを保存するディレクトリパス
    LOG_DIR ${OPENCV_PREFIX}/log
    # 解凍前のダウンロード内容(zipなど)を保存するディレクトリパス(※ gitでは, SOURCE_DIRに直接保存される)
    DOWNLOAD_DIR ${OPENCV_PREFIX}/archive
    # (解凍後の)ダウンロード内容を保存するディレクトリパス
    SOURCE_DIR ${OPENCV_PREFIX}/src
    # ビルド後の成果物を保存するディレクトリパス
    BINARY_DIR ${OPENCV_PREFIX}/build
    # ビルド成果物のインストール先のディレクトリパス
    # (※ 実際にはmake installのインストール先に、このプロパティは使われない! make install DESTDIR=~/path/toで指定するかCMakeの-Dオプションで指定)
    INSTALL_DIR ${OPENCV_PREFIX}

    #################### No.1 Download Step ####################
    # URL Download
    URL https://github.com/opencv/opencv/archive/4.5.3.zip
    # Timeout
    TIMEOUT 10 # 10秒

    #################### No.2 Update/Patch Step ####################
    # ダウンロードした内容にパッチを当てるコマンド
    # UPDATE_COMMAND ""

    #################### No.3 Configure Step ####################
    # ジェネレータ
    # CMAKE_GENERATOR ""
    # cmake コマンドに渡す引数
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${OPENCV_INSTALL} 
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
            #-DCMAKE_COMPILER_IS_GNUCXX=${CMAKE_COMPILER_IS_GNUCXX}
            #-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            #-DCMAKE_C_FLAGS=${GL_BINDING_C_FLAGS}
            -DEIGEN_INCLUDE_PATH:PATH=${CMAKE_CURRENT_SOURCE_DIR}/${EIGEN_DIRNAME}
            -DOPENCV_EXTRA_MODULES_PATH:PATH=${CMAKE_CURRENT_SOURCE_DIR}/${OPENCV_CONTRIB_DIRNAME}/modules
            -DBUILD_opencv_world=ON
            -DBUILD_DOCS=ON
            -DBUILD_TESTS=OFF

    #################### No.4 Build Step ####################
    # ビルドコマンド
    #BUILD_COMMAND ""
    # Build In Source or Build Out Source
    BUILD_IN_SOURCE OFF
    # Build Always or Not
    BUILD_ALWAYS OFF

    #################### No.5 Install Step ####################
    # インストールコマンド
    #INSTALL_COMMAND ""
    #################### No.6 Test Step ####################
    #################### No.7 Output Logging ####################
    LOG_DOWNLOAD ON
    LOG_UPDATE ON
    LOG_PATCH ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
    LOG_TEST ON
    LOG_MERGED_STDOUTERR ON
    LOG_OUTPUT_ON_FAILURE ON
    #################### No.8 Terminal Access ####################
    #################### No.9 Target ####################
    #################### No.10 Miscellaneous ####################
)

ExternalProject_Get_Property(${OPENCV_TARGET} ${INSTALL_DIR})
set(OPENCV_INSTALL_DIR ${OPENCV_INSTALL})
message(STATUS "${OPENCV_DIRNAME}'s installed dir: ${OPENCV_INSTALL_DIR}")

# OpenCVのインクルードパス
set(OPENCV_INCLUDE_DIR ${OPENCV_INSTALL_DIR}/include)
message(STATUS "${OPENCV_DIRNAME}'s include dir: ${OPENCV_INCLUDE_DIR}")
include_directories(${OPENCV_INCLUDE_DIR})

# OpenCVのライブラリパス
set(OPENCV_LIBRARY_DIR ${OPENCV_INSTALL_DIR}/lib)
message(STATUS "${OPENCV_DIRNAME}'s lib dir: ${OPENCV_LIBRARY_DIR}")
link_directories(${OPENCV_LIBRARY_DIR})

# CMakeのターゲットに昇華
set(INTERNAL_OPENCV_TARGET "internal-${OPENCV_TARGET}" CACHE STRING "Internal-OpenCV Target") # キャッシュ変数はどこからでもアクセス可能
add_library(${INTERNAL_OPENCV_TARGET} SHARED IMPORTED GLOBAL) # 上位層のディレクトリから参照するにはGLOBALオプションが必要
add_dependencies(${INTERNAL_OPENCV_TARGET} ${OPENCV_TARGET})


# zlib-1.2.11 ------------------------------------------------------------------------
# http://www.zlib.net/
# https://github.com/madler/zlib
set(ZLIB_DIRNAME zlib-1.2.11)
message(STATUS "Download and extract library: ${ZLIB_DIRNAME}")

# Setting zlib Project
set(ZLIB_TARGET ${ZLIB_DIRNAME})
set(ZLIB_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${ZLIB_TARGET})
set(ZLIB_INSTALL ${ZLIB_PREFIX}/install)

ExternalProject_Add(
    # Taget Name
    ${ZLIB_TARGET}

    #################### No.0 Common Directory Path ####################
    # 一時ファイルを保存するディレクトリパス
    TMP_DIR ${ZLIB_PREFIX}/tmp
    # 各ステップで生成されるタイムスタンプを保存するディレクトリパス
    STAMP_DIR ${ZLIB_PREFIX}/stamp
    # 各ステップでのLogファイルを保存するディレクトリパス
    LOG_DIR ${ZLIB_PREFIX}/log
    # 解凍前のダウンロード内容(zipなど)を保存するディレクトリパス(※ gitでは, SOURCE_DIRに直接保存される)
    DOWNLOAD_DIR ${ZLIB_PREFIX}/archive
    # (解凍後の)ダウンロード内容を保存するディレクトリパス
    SOURCE_DIR ${ZLIB_PREFIX}/src
    # ビルド後の成果物を保存するディレクトリパス
    BINARY_DIR ${ZLIB_PREFIX}/build
    # ビルド成果物のインストール先のディレクトリパス
    # (※ 実際にはmake installのインストール先に、このプロパティは使われない! make install DESTDIR=~/path/toで指定するかCMakeの-Dオプションで指定)
    INSTALL_DIR ${ZLIB_PREFIX}

    #################### No.1 Download Step ####################
    # 原因不明で前回, git cloneを行っていないのに、すでにgit cloneされていると判断されて、git cloneが実行されない.
    # # Git Repository URL
    # GIT_REPOSITORY https://github.com/google/googletest.git
    # # Git branch-name, tag or commit's hash id
    # GIT_TAG release-1.10.0
    # # Git Remote Name (default: origin)
    # GIT_REMOTE_NAME origin
    # # Git Remote Clone Progress
    # GIT_REMOTE_PROGRESS ON

    # URL Download
    URL https://github.com/madler/zlib/archive/refs/tags/v1.2.11.zip
    # Timeout
    TIMEOUT 10 # 10秒

    #################### No.2 Update/Patch Step ####################
    # ダウンロードした内容にパッチを当てるコマンド
    # UPDATE_COMMAND ""

    #################### No.3 Configure Step ####################
    # ジェネレータ
    # CMAKE_GENERATOR ""
    # cmake コマンドに渡す引数
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${ZLIB_INSTALL} 
            -DBUILD_SHARED_LIBS=ON
            -DCMAKE_COMPILER_IS_GNUCXX=${CMAKE_COMPILER_IS_GNUCXX}
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_C_FLAGS=${GL_BINDING_C_FLAGS}
    
    #################### No.4 Build Step ####################
    # ビルドコマンド
    #BUILD_COMMAND ""
    # Build In Source or Build Out Source
    BUILD_IN_SOURCE OFF
    # Build Always or Not
    BUILD_ALWAYS OFF

    #################### No.5 Install Step ####################
    # インストールコマンド
    #INSTALL_COMMAND ""
    #################### No.6 Test Step ####################
    #################### No.7 Output Logging ####################
    LOG_DOWNLOAD ON
    LOG_UPDATE ON
    LOG_PATCH ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
    LOG_TEST ON
    LOG_MERGED_STDOUTERR ON
    LOG_OUTPUT_ON_FAILURE ON
    #################### No.8 Terminal Access ####################
    #################### No.9 Target ####################
    #################### No.10 Miscellaneous ####################
)

ExternalProject_Get_Property(${ZLIB_TARGET} ${INSTALL_DIR})
set(ZLIB_INSTALL_DIR ${ZLIB_INSTALL})
message(STATUS "${ZLIB_DIRNAME}'s installed dir: ${ZLIB_INSTALL_DIR}")

# Zlibのインクルードパス
set(ZLIB_INCLUDE_DIR ${ZLIB_INSTALL_DIR}/include)
message(STATUS "${ZLIB_DIRNAME}'s include dir: ${ZLIB_INCLUDE_DIR}")
include_directories(${ZLIB_INCLUDE_DIR})

# Zlibのライブラリパス
set(ZLIB_LIBRARY_DIR ${ZLIB_INSTALL_DIR}/lib)
message(STATUS "${ZLIB_DIRNAME}'s lib dir: ${ZLIB_LIBRARY_DIR}")
link_directories(${ZLIB_LIBRARY_DIR})

# CMakeのターゲットに昇華
set (INTERNAL_ZLIB_TARGET "internal-${ZLIB_TARGET}")
add_library(${INTERNAL_ZLIB_TARGET} SHARED IMPORTED GLOBAL) # 上位層のディレクトリから参照するにはGLOBALオプションが必要
add_dependencies(${INTERNAL_ZLIB_TARGET} ${ZLIB_TARGET})



# libpng-1.6.35 ----------------------------------------------------------
# http://www.libpng.org/pub/png/libpng.html
# https://github.com/glennrp/libpng
set(LIBPNG_DIRNAME libpng-1.6.35)
message(STATUS "Download and extract library: ${LIBPNG_DIRNAME}")

# Setting libpng Project
set(LIBPNG_TARGET ${LIBPNG_DIRNAME})
set(LIBPNG_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/${LIBPNG_TARGET})
set(LIBPNG_INSTALL ${LIBPNG_PREFIX}/install)

ExternalProject_Add(
    # Taget Name
    ${LIBPNG_TARGET}

    #################### No.0 Common Directory Path ####################
    # 一時ファイルを保存するディレクトリパス
    TMP_DIR ${LIBPNG_PREFIX}/tmp
    # 各ステップで生成されるタイムスタンプを保存するディレクトリパス
    STAMP_DIR ${LIBPNG_PREFIX}/stamp
    # 各ステップでのLogファイルを保存するディレクトリパス
    LOG_DIR ${LIBPNG_PREFIX}/log
    # 解凍前のダウンロード内容(zipなど)を保存するディレクトリパス(※ gitでは, SOURCE_DIRに直接保存される)
    DOWNLOAD_DIR ${LIBPNG_PREFIX}/archive
    # (解凍後の)ダウンロード内容を保存するディレクトリパス
    SOURCE_DIR ${LIBPNG_PREFIX}/src
    # ビルド後の成果物を保存するディレクトリパス
    BINARY_DIR ${LIBPNG_PREFIX}/build
    # ビルド成果物のインストール先のディレクトリパス
    # (※ 実際にはmake installのインストール先に、このプロパティは使われない! make install DESTDIR=~/path/toで指定するかCMakeの-Dオプションで指定)
    INSTALL_DIR ${LIBPNG_PREFIX}/install

     #################### No.1 Download Step ####################
    # 原因不明で前回, git cloneを行っていないのに、すでにgit cloneされていると判断されて、git cloneが実行されない.
    # # Git Repository URL
    # GIT_REPOSITORY [url]
    # # Git branch-name, tag or commit's hash id
    # GIT_TAG [tag]
    # # Git Remote Name (default: origin)
    # GIT_REMOTE_NAME origin
    # # Git Remote Clone Progress
    # GIT_REMOTE_PROGRESS ON

    # URL Download
    URL https://github.com/glennrp/libpng/archive/refs/tags/v1.6.35.zip
    # Timeout
    TIMEOUT 10 # 10秒

    #################### No.2 Update/Patch Step ####################
    # ダウンロードした内容にパッチを当てるコマンド
    # UPDATE_COMMAND ""

    #################### No.3 Configure Step ####################
    # 依存関係
    DEPENDS ${ZLIB_TARGET}

    # ジェネレータ
    # CMAKE_GENERATOR ""

    # cmake コマンドに渡す引数
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${LIBPNG_INSTALL} 
               -DPNG_DEBUG=OFF
               -DBUILD_SHARED_LIBS=OFF
               -DPNG_SHARED=ON
               -DPNG_STATIC=ON
               -DPNG_TEST=ON
               -DPNG_BUILD_ZLIB=ON # OFFの場合, find_packageする
            #    -DZLIB_INCLUDE_DIR:PATH=${ZLIB_INCLUDE_DIR}
            #    -DZLIB_LIBRARY_DEBUG:PATH=${ZLIB_LIBRARY_DIR}/zlibd.lib
            #    -DZLIB_LIBRARY_RELEASE:PATH=${ZLIB_LIBRARY_DIR}/zlib.lib
             
    
    #################### No.4 Build Step ####################
    # ビルドコマンド
    #BUILD_COMMAND ""
    # Build In Source or Build Out Source
    BUILD_IN_SOURCE OFF
    # Build Always or Not
    BUILD_ALWAYS OFF

    #################### No.5 Install Step ####################
    # インストールコマンド
    #INSTALL_COMMAND ""
    #################### No.6 Test Step ####################
    #################### No.7 Output Logging ####################
    LOG_DOWNLOAD ON
    LOG_UPDATE ON
    LOG_PATCH ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
    LOG_TEST ON
    LOG_MERGED_STDOUTERR ON
    LOG_OUTPUT_ON_FAILURE ON
    #################### No.8 Terminal Access ####################
    #################### No.9 Target ####################
    #################### No.10 Miscellaneous ####################
)


ExternalProject_Get_Property(${LIBPNG_TARGET} ${INSTALL_DIR})
set(LIBPNG_INSTALL_DIR ${LIBPNG_INSTALL})
message(STATUS "${LIBPNG_DIRNAME}'s installed dir: ${LIBPNG_INSTALL_DIR}")

# Libpngのインクルードパス
set(LIBPNG_INCLUDE_DIR ${LIBPNG_INSTALL_DIR}/include)
message(STATUS "${LIBPNG_DIRNAME}'s include dir: ${LIBPNG_INCLUDE_DIR}")
include_directories(${LIBPNG_INCLUDE_DIR})

# Libpngのライブラリパス
set(LIBPNG_LIBRARY_DIR ${LIBPNG_INSTALL_DIR}/lib)
message(STATUS "${LIBPNG_DIRNAME}'s lib dir: ${LIBPNG_LIBRARY_DIR}")
link_directories(${LIBPNG_LIBRARY_DIR})

# CMakeのターゲットに昇華
set (INTERNAL_LIBPNG_TARGET "internal-${LIBLIB_TARGET}")
add_library(${INTERNAL_LIBPNG_TARGET} SHARED IMPORTED GLOBAL) # 上位層のディレクトリから参照するにはGLOBALオプションが必要
add_dependencies(${INTERNAL_LIBPNG_TARGET} ${LIBPNG_TARGET})

# # Internalなlibpngのビルド時に使うIncludeとLibrary
# set_target_properties(${MyLibPng} PROPERTIES
#     IMPORTED_LOCATION ${ZLIB_LIBRARY_DIR}
#     IMPORTED_IMPLIB ${ZLIB_LIBRARY_DIR}
#     INTERFACE_INCLUDE_DIRECTORIES ${ZLIB_INCLUDE_DIR}
#     INTERFACE_LINK_LIBRARIES ${INTERNAL_ZLIB_TARGET}
# )