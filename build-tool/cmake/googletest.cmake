# Download and Build GoogleTest with CMake ExternalProject()

cmake_minimum_required(VERSION 3.14.6)

function(BuildGoogleTest GTEST_TARGET GTEST_SHARED_LIB GTEST_BUILD_ALWAYS)

    message(STATUS "Download and Build GoogleTest with CMake ExternalProject()")

    set(GTEST_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external_project/${GTEST_TARGET})
    set(GTEST_INSTALL ${GTEST_PREFIX}/install)

    message(STATUS "GoogleTest CMake Target: `${GTEST_TARGET}`")
    message(STATUS "CMake ExternalProject Prefix Path: ${GTEST_PREFIX}") # mockがサポートされたのは1.8.0から
    message(STATUS "GoogleTest install dir: ${GTEST_INSTALL}")
    message(STATUS "GoogleTest shared_lib: ${GTEST_SHARED_LIB}")
    message(STATUS "GoogleTest build always: ${GTEST_BUILD_ALWAYS}")

    # GoogleTestのコンパイルにはGCCが必要になるので, macOSにおけるパスを指定する.
    set(MACOS_GCC_COMPILER_OPTION "")
    set(MACOS_GCC_C_PATH "/usr/local/bin/gcc-11")
    set(MACOS_GCC_CXX_PATH "/usr/local/bin/g++-11")
    set(MACOS_GCC_C_COMPILER "")
    set(MACOS_GCC_CXX_COMPILER "")
    if(APPLE)
        if (NOT EXISTS ${MACOS_GCC_C_PATH} OR NOT EXISTS ${MACOS_GCC_CXX_PATH})
            message(FATAL_ERROR "Failed to find GCC compiler. Please install GCC compiler on macOS.")
        endif()
        message(STATUS "Found GCC compiler on macOS.")
        set(MACOS_GCC_C_COMPILER "-DCMAKE_C_COMPILER=${MACOS_GCC_C_PATH}")
        set(MACOS_GCC_CXX_COMPILER "-DCMAKE_CXX_COMPILER=${MACOS_GCC_CXX_PATH}")
    endif()

    # Enable ExternalProject
    include(ExternalProject)
    ExternalProject_Add(
        # ----------------- Target Name -------------------
        ${GTEST_TARGET}

        #################### No.0 Common Directory Path ####################
        # 一時ファイルを保存するディレクトリパス
        TMP_DIR ${GTEST_PREFIX}/tmp
        # 各ステップで生成されるタイムスタンプを保存するディレクトリパス
        STAMP_DIR ${GTEST_PREFIX}/stamp
        # 各ステップでのLogファイルを保存するディレクトリパス
        LOG_DIR ${GTEST_PREFIX}/log
        # 解凍前のダウンロード内容(zipなど)を保存するディレクトリパス(※ gitでは, SOURCE_DIRに直接保存される)
        DOWNLOAD_DIR ${GTEST_PREFIX}/archive
        # (解凍後の)ダウンロード内容を保存するディレクトリパス
        SOURCE_DIR ${GTEST_PREFIX}/src
        # ビルド後の成果物を保存するディレクトリパス
        BINARY_DIR ${GTEST_PREFIX}/build
        # ビルド成果物のインストール先のディレクトリパス
        # (※ 実際にはmake installのインストール先に、このプロパティは使われない! make install DESTDIR=~/path/toで指定するかCMakeの-Dオプションで指定)
        INSTALL_DIR ${GTEST_INSTALL} #${GTEST_PREFIX}/install 


        #################### No.1 Download Step ####################
        # ---------- Git Clone Version ----------
        # Git Repository URL
        # GIT_REPOSITORY {{HTTP URL}}
        # Git branch-name, tag or commit's hash id
        # GIT_TAG {{TAG NAME}}
        # Git Remote Name (default: origin)
        # GIT_REMOTE_NAME origin # maybe `origin` or `main`
        # Git Remote Clone Progress
        # GIT_REMOTE_PROGRESS ON

        # ---------- Download Version ----------
        # Download archive source url
        URL https://github.com/google/googletest/archive/release-1.10.0.tar.gz

        # ---------- Other Setting ----------
        # Timeout
        TIMEOUT 10 # 10秒
        

        #################### No.2 Update/Patch Step ####################
        # Update if necessary.
        UPDATE_COMMAND ""
        # Patch if necessary.
        PATCH_COMMAND ""


        #################### No.3 Configure Step ####################
        # CMakeビルド時に渡す引数
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${GTEST_INSTALL} 
                   -DBUILD_SHARED_LIBS=${GTEST_SHARED_LIB} 
                   -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} # CMakeのビルドタイプに合わせる
                   ${MACOS_GCC_C_COMPILER} # Mac OS用 GCCコンパイラへのパス
                   ${MACOS_GCC_CXX_COMPILER}


        #################### No.4 Build Step ####################
        # Build allways or not
        BUILD_ALWAYS ${GTEST_BUILD_ALWAYS}
        # Build In Source or not
        BUILD_IN_SOURCE OFF


        #################### No.5 Install Step ####################
        # Custom override install
        # INSTALL_COMMAND <cmd>


        #################### No.6 Test Step ####################
        # メインビルドのAllターゲットからGoogleTestのテストステップを除外する
        TEST_EXCLUDE_FROM_MAIN ON



        #################### No.7 Output Logging ####################
        # Download stepの出力をファイルに記録する
        LOG_DOWNLOAD ON
        # Update stepの出力をファイルに記録する
        LOG_UPDATE ON
        # Patch stepの出力をファイルに記録する
        LOG_PATCH ON
        # Configure stepの出力をファイルに記録する
        LOG_CONFIGURE ON
        # Build stepの出力をファイルに記録する
        LOG_BUILD ON
        # Install stepの出力をファイルに記録する
        LOG_INSTALL ON
        # Test stepの出力をファイルに記録する
        LOG_TEST ON
        # 出力ファイルでstdoutとstderrをマージするか否か
        LOG_MERGED_STDOUTERR OFF
        # ファイルへのログ記録が有効になっているステップでエラーが発生した場合、そのステップの出力をコンソール出力する
        LOG_OUTPUT_ON_FAILURE ON


        #################### No.8 Terminal Access ####################
        # 各ステップにおける端末(コンソール)へのアクセス許可を行う

        #################### No.9 Target ####################
        # ターゲットに関するオプション

        #################### No.10 Miscellaneous ####################
        # その他
    )

    # ExternalProjectターゲットのプロパティを取得(今回はインストール先のパスを取得する)
    ExternalProject_Get_Property(${GTEST_TARGET} INSTALL_DIR)

    # pthread for googletest
    find_package(Threads REQUIRED)


    if (NOT EXISTS ${INSTALL_DIR}/include)
        message(STATUS "Return googletest.cmake because not make install/include yet.")
        message(STATUS "Please build googletest before test targets.")
        return()
    endif()

    # ---------------------------------------------------------------------- #
    # ビルド成果物である下記をそれぞれCMakeにおけるターゲットに昇華させる
    # IMPORTED TARGETの生成
    # https://qiita.com/osamu0329/items/3e9868e83d3745b8c8b6
    # 1. gtest[d].(lib, dll @MSVC or a, so @GNU)
    # 2. gtest_main[d].(lib, dll @MSVC or a, so @GNU)
    # 3. gmock[d].(lib, dll @MSVC or a, so @GNU)
    # 4. gmock_main[d].(lib, dll @MSVC or a, so @GNU)
    # ---------------------------------------------------------------------- #
    set(IMPORTED_LIB_EXT "")

    if(${GTEST_SHARED_LIB})
        # GTest, GTestMain, GMock, GMockMain with shared
        add_library(GTest::gtest SHARED IMPORTED GLOBAL)
        add_library(GTest::gmock SHARED IMPORTED GLOBAL)
        add_library(GTest::gtest_main SHARED IMPORTED GLOBAL)
        add_library(GTest::gmock_main SHARED IMPORTED GLOBAL)
        add_dependencies(GTest::gtest ${GTEST_TARGET})
        add_dependencies(GTest::gmock ${GTEST_TARGET})
        add_dependencies(GTest::gtest_main ${GTEST_TARGET})
        add_dependencies(GTest::gmock_main ${GTEST_TARGET})

        # OS別の動的ライブラリの拡張子
        if(MSVC)
            set(IMPORTED_LIB_EXT dll)
        elseif(APPLE)
            set(IMPORTED_LIB_EXT dylib)
        else()
            set(IMPORTED_LIB_EXT so)
        endif()
    else()
        # GTest, GTestMain, GMock, GMockMain with static
        add_library(GTest::gtest STATIC IMPORTED GLOBAL)
        add_library(GTest::gmock STATIC IMPORTED GLOBAL)
        add_library(GTest::gtest_main STATIC IMPORTED GLOBAL)
        add_library(GTest::gmock_main STATIC IMPORTED GLOBAL)
        add_dependencies(GTest::gtest ${GTEST_TARGET})
        add_dependencies(GTest::gmock ${GTEST_TARGET})
        add_dependencies(GTest::gtest_main ${GTEST_TARGET})
        add_dependencies(GTest::gmock_main ${GTEST_TARGET})

        # OS別の静的ライブラリの拡張子
        if(MSVC)
            set(IMPORTED_LIB_EXT lib)
        elseif(APPLE)
            set(IMPORTED_LIB_EXT a)
        else()
            set(IMPORTED_LIB_EXT a)
        endif()
    endif()
    
    message(STATUS "Extention of GoogleTest's library is `${IMPORTED_LIB_EXT}`.")
    

    set(LIB_GTEST_NAME "")
    set(LIB_GMOCK_NAME "")
    set(LIB_GTEST_MAIN_NAME "")
    set(LIB_GMOCK_MAIN_NAME "")
    if(MSVC)
        set(LIB_GTEST_NAME "gtest")
        set(LIB_GMOCK_NAME "gmock")
        set(LIB_GTEST_MAIN_NAME "gtest_main")
        set(LIB_GMOCK_MAIN_NAME "gmock_main")
    else()
        set(LIB_GTEST_NAME "libgtest")
        set(LIB_GMOCK_NAME "libgmock")
        set(LIB_GTEST_MAIN_NAME "libgtest_main")
        set(LIB_GMOCK_MAIN_NAME "libgmock_main")
    endif()

    if((${CMAKE_BUILD_TYPE} STREQUAL Debug) OR (${CMAKE_BUILD_TYPE} STREQUAL RelWithDebgInfo))
            # GTest
            set_target_properties(GTest::gtest PROPERTIES
                IMPORTED_LOCATION ${INSTALL_DIR}/lib/${LIB_GTEST_NAME}d.${IMPORTED_LIB_EXT}
                INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
                INTERFACE_LINK_LIBRARIES Threads::Threads
            )

            # GMock
            set_target_properties(GTest::gmock PROPERTIES
                IMPORTED_LOCATION ${INSTALL_DIR}/lib/${LIB_GMOCK_NAME}d.${IMPORTED_LIB_EXT}
                INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
                INTERFACE_LINK_LIBRARIES Threads::Threads
            )

            # GTestMain
            set_target_properties(GTest::gtest_main PROPERTIES
                IMPORTED_LOCATION ${INSTALL_DIR}/lib/${LIB_GTEST_MAIN_NAME}d.${IMPORTED_LIB_EXT}
                INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
                INTERFACE_LINK_LIBRARIES Threads::Threads
            )

            # GMockMain
            set_target_properties(GTest::gmock_main PROPERTIES
                IMPORTED_LOCATION ${INSTALL_DIR}/lib/${LIB_GMOCK_MAIN_NAME}d.${IMPORTED_LIB_EXT}
                INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
                INTERFACE_LINK_LIBRARIES Threads::Threads
            )
    else()
            # GTest
            set_target_properties(GTest::gtest PROPERTIES
                IMPORTED_LOCATION ${INSTALL_DIR}/lib/${LIB_GTEST_NAME}.${IMPORTED_LIB_EXT}
                INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
                INTERFACE_LINK_LIBRARIES Threads::Threads
            )

            # GMock
            set_target_properties(GTest::gmock PROPERTIES
                IMPORTED_LOCATION ${INSTALL_DIR}/lib/${LIB_GMOCK_NAME}.${IMPORTED_LIB_EXT}
                INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
                INTERFACE_LINK_LIBRARIES Threads::Threads
            )

            # GTestMain
            set_target_properties(GTest::gtest_main PROPERTIES
                IMPORTED_LOCATION ${INSTALL_DIR}/lib/${LIB_GTEST_MAIN_NAME}.${IMPORTED_LIB_EXT}
                INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
                INTERFACE_LINK_LIBRARIES Threads::Threads
            )

            # GMockMain
            set_target_properties(GTest::gmock_main PROPERTIES
                IMPORTED_LOCATION ${INSTALL_DIR}/lib/${LIB_GMOCK_MAIN_NAME}.${IMPORTED_LIB_EXT}
                INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
                INTERFACE_LINK_LIBRARIES Threads::Threads
            )
    endif() 

endfunction()