# Download and Build GoogleTest with CMake ExternalProject()

cmake_minimum_required(VERSION 3.14.6)

function(BuildGoogleTest GTEST_TARGET GTEST_VERSION GTEST_SHARED_LIB)

    message(STATUS "Download and Build GoogleTest with CMake ExternalProject()")

    set(GTEST_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external_project/${GTEST_TARGET})
    set(GTEST_INSTALL ${GTEST_PREFIX}/install)

    message(STATUS "GoggleTest Version: ${GTEST_VERSION}")
    message(STATUS "GoogleTest CMake Target: `${GTEST_TARGET}`")
    message(STATUS "CMake ExternalProject Prefix Path: ${GTEST_PREFIX}") # mockがサポートされたのは1.8.0から
    message(STATUS "GoogleTest install dir: ${GTEST_INSTALL}")
    message(STATUS "GoogleTest shared_lib: ${GTEST_SHARED_LIB}")

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
        URL https://github.com/google/googletest/archive/release-${GTEST_VERSION}.tar.gz

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
                   -DCMAKE_RELEASE_POSTFIX=${CMAKE_RELEASE_POSTFIX}
                   -DCMAKE_DEBUG_POSTFIX=${CMAKE_DEBUG_POSTFIX}
                   -DCMAKE_MINSIZEREL_POSTFIX=${CMAKE_MINSIZEREL_POSTFIX}
                   -DCMAKE_RELWITHDEBINFO_POSTFIX=${CMAKE_RELWITHDEBINFO_POSTFIX}


        #################### No.4 Build Step ####################
        # Build allways or not
        BUILD_ALWAYS ON
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
    # 1. gtest.(lib, dll @MSVC or a, so @GNU)
    # 2. gtest_main.(lib, dll @MSVC or a, so @GNU)
    # 3. gmock.(lib, dll @MSVC or a, so @GNU)
    # 4. gmock_main.(lib, dll @MSVC or a, so @GNU)
    # ---------------------------------------------------------------------- #

    set(GT_LIB_TYPE "")
    set(GT_LIB_PREFIX "")
    set(GT_LIB_EXT "")
    set(GT_LIB_DIR "")
    if(${GTEST_SHARED_LIB})
        set(GT_LIB_TYPE SHARED)
        if(MSVC)
            set(GT_LIB_DIR "bin")
            set(GT_LIB_PREFIX "")
            set(GT_LIB_EXT "dll")
        elseif(APPLE)
            set(GT_LIB_DIR "lib")
            set(GT_LIB_PREFIX "lib")
            set(GT_LIB_EXT "dylib")
        else()
            set(GT_LIB_DIR "lib")
            set(GT_LIB_PREFIX "lib")
            set(GT_LIB_EXT "so")
        endif()
    else()
        set(GT_LIB_TYPE STATIC)
        if(MSVC)
            set(GT_LIB_DIR "bin")
            set(GT_LIB_PREFIX "")
            set(GT_LIB_EXT "lib")
        elseif(APPLE)
            set(GT_LIB_DIR "lib")
            set(GT_LIB_PREFIX "lib")
            set(GT_LIB_EXT "a")
        else()
            set(GT_LIB_DIR "lib")
            set(GT_LIB_PREFIX "lib")
            set(GT_LIB_EXT "a")
        endif()
    endif()

    message(STATUS "GoogleTest library type: ${GT_LIB_TYPE}")

    # GTest, GTestMain, GMock, GMockMain
    foreach(GT_TARGET IN ITEMS gtest gmock gtest_main gmock_main)
        # Generate IMPORTED_TARGET
        add_library(GTest::${GT_TARGET} ${GT_LIB_TYPE} IMPORTED GLOBAL)

        # Dependency for GTEST_TARGET
        add_dependencies(GTest::${GT_TARGET} ${GTEST_TARGET})

        # Target Property
        set(GT_INSTALL_LIB_DIR ${INSTALL_DIR}/${GT_LIB_DIR})
        set_target_properties(GTest::${GT_TARGET} PROPERTIES
            IMPORTED_LOCATION_RELEASE ${GT_INSTALL_LIB_DIR}/${GT_LIB_PREFIX}${GT_TARGET}${CMAKE_REALSE_POSTFIX}.${GT_LIB_EXT}
            IMPORTED_LOCATION_DEBUG ${GT_INSTALL_LIB_DIR}/${GT_LIB_PREFIX}${GT_TARGET}${CMAKE_DEBUG_POSTFIX}.${GT_LIB_EXT}
            MPORTED_LOCATION_RELWITHDEBINFO ${GT_INSTALL_LIB_DIR}/${GT_LIB_PREFIX}${GT_TARGET}${CMAKE_RELWITHDEBINFO_POSTFIX}.${GT_LIB_EXT}
            IMPORTED_LOCATION_MINSIZEREL ${GT_INSTALL_LIB_DIR}/${GT_LIB_PREFIX}${GT_TARGET}${CMAKE_MINSIZEREL_POSTFIX}.${GT_LIB_EXT}
            INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
            INTERFACE_LINK_LIBRARIES Threads::Threads
        )

        # Import lib
        if(MSVC)
            set_target_properties(GTest::${GT_TARGET} PROPERTIES
                IMPORTED_IMPLIB_RELEASE ${GT_INSTALL_LIB_DIR}/${GT_LIB_PREFIX}${GT_TARGET}${CMAKE_REALSE_POSTFIX}.lib
                IMPORTED_IMPLIB_DEBUG ${GT_INSTALL_LIB_DIR}/${GT_LIB_PREFIX}${GT_TARGET}${CMAKE_DEBUG_POSTFIX}.lib
                IMPORTED_IMPLIB_RELWITHDEBINFO ${GT_INSTALL_LIB_DIR}/${GT_LIB_PREFIX}${GT_TARGET}${CMAKE_RELWITHDEBINFO_POSTFIX}.lib
                IMPORTED_IMPLIB_MINSIZEREL ${GT_INSTALL_LIB_DIR}/${GT_LIB_PREFIX}${GT_TARGET}${CMAKE_MINSIZEREL_POSTFIX}.lib
            )
        endif()

    endforeach()
    
endfunction()