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
                   -DCMAKE_DEBUG_POSTFIX=d
                   -DCMAKE_MINSIZEREL_POSTFIX=_minsizerel
                   -DCMAKE_RELWITHDEBINFO_POSTFIX=_relwithdebinfo


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
    # 1. gtest[d].(lib, dll @MSVC or a, so @GNU)
    # 2. gtest_main[d].(lib, dll @MSVC or a, so @GNU)
    # 3. gmock[d].(lib, dll @MSVC or a, so @GNU)
    # 4. gmock_main[d].(lib, dll @MSVC or a, so @GNU)
    # ---------------------------------------------------------------------- #

    set(GOOGLETEST_LIB_TYPE "")
    if(${GTEST_SHARED_LIB})
        set(GOOGLETEST_LIB_TYPE SHARED)
    else()
        set(GOOGLETEST_LIB_TYPE STATIC)
    endif()

    message(STATUS "GOOGLETEST_LIB_TYPE: ${GOOGLETEST_LIB_TYPE}")

    # Generate IMPORTED_TARGET
    # GTest, GTestMain, GMock, GMockMain
    add_library(GTest::gtest ${GOOGLETEST_LIB_TYPE} IMPORTED GLOBAL)
    add_library(GTest::gmock ${GOOGLETEST_LIB_TYPE} IMPORTED GLOBAL)
    add_library(GTest::gtest_main ${GOOGLETEST_LIB_TYPE} IMPORTED GLOBAL)
    add_library(GTest::gmock_main ${GOOGLETEST_LIB_TYPE} IMPORTED GLOBAL)

    add_dependencies(GTest::gtest ${GTEST_TARGET})
    add_dependencies(GTest::gmock ${GTEST_TARGET})
    add_dependencies(GTest::gtest_main ${GTEST_TARGET})
    add_dependencies(GTest::gmock_main ${GTEST_TARGET})

    # message(STATUS "CMAKE_FIND_LIBRARY_PREFIXES: ${CMAKE_FIND_LIBRARY_PREFIXES}")
    # message(STATUS "CMAKE_FIND_LIBRARY SUFFIXES: ${CMAKE_FIND_LIBRARY_SUFFIXES}")

    if(MSVC)
        # GTest
        set_target_properties(GTest::gtest PROPERTIES
            IMPORTED_LOCATION_RELEASE ${INSTALL_DIR}/bin/gtest.dll
            IMPORTED_LOCATION_DEBUG ${INSTALL_DIR}/bin/gtestd.dll
            IMPORTED_LOCATION_RELWITHDEBINFO ${INSTALL_DIR}/bin/gtest_relwithdebinfo.dll
            IMPORTED_LOCATION_MINSIZEREL ${INSTALL_DIR}/bin/gtest_minsizerel.dll
            IMPORTED_IMPLIB_RELEASE ${INSTALL_DIR}/lib/gtest.lib
            IMPORTED_IMPLIB_DEBUG ${INSTALL_DIR}/lib/gtestd.lib
            IMPORTED_IMPLIB_RELWITHDEBINFO ${INSTALL_DIR}/lib/gtest_relwithdebinfo.lib
            IMPORTED_IMPLIB_MINSIZEREL ${INSTALL_DIR}/lib/gtest_minsizerel.lib
            INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
            INTERFACE_LINK_LIBRARIES Threads::Threads
        )

        # GTest_Main
        set_target_properties(GTest::gtest_main PROPERTIES
            IMPORTED_LOCATION_RELEASE ${INSTALL_DIR}/bin/gtest_main.dll
            IMPORTED_LOCATION_DEBUG ${INSTALL_DIR}/bin/gtest_maind.dll
            IMPORTED_LOCATION_RELWITHDEBINFO ${INSTALL_DIR}/bin/gtest_main_relwithdebinfo.dll
            IMPORTED_LOCATION_MINSIZEREL ${INSTALL_DIR}/bin/gtest_main_minsizerel.dll
            IMPORTED_IMPLIB_RELEASE ${INSTALL_DIR}/lib/gtest_main.lib
            IMPORTED_IMPLIB_DEBUG ${INSTALL_DIR}/lib/gtest_maind.lib
            IMPORTED_IMPLIB_RELWITHDEBINFO ${INSTALL_DIR}/lib/gtest_main_relwithdebinfo.lib
            IMPORTED_IMPLIB_MINSIZEREL ${INSTALL_DIR}/lib/gtest_main_minsizerel.lib
            INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
            INTERFACE_LINK_LIBRARIES Threads::Threads
        )

        # GMock
        set_target_properties(GTest::gmock PROPERTIES
            IMPORTED_LOCATION_RELEASE ${INSTALL_DIR}/bin/gmock.dll
            IMPORTED_LOCATION_DEBUG ${INSTALL_DIR}/bin/gmockd.dll
            IMPORTED_LOCATION_RELWITHDEBINFO ${INSTALL_DIR}/bin/gmock_relwithdebinfo.dll
            IMPORTED_LOCATION_MINSIZEREL ${INSTALL_DIR}/bin/gmock_minsizerel.dll
            IMPORTED_IMPLIB_RELEASE ${INSTALL_DIR}/lib/gmock.lib
            IMPORTED_IMPLIB_DEBUG ${INSTALL_DIR}/lib/gmockd.lib
            IMPORTED_IMPLIB_RELWITHDEBINFO ${INSTALL_DIR}/lib/gmock_relwithdebinfo.lib
            IMPORTED_IMPLIB_MINSIZEREL ${INSTALL_DIR}/lib/gmock_minsizerel.lib
            INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
            INTERFACE_LINK_LIBRARIES Threads::Threads
        )


        # GMock
        set_target_properties(GTest::gmock PROPERTIES
            IMPORTED_LOCATION_RELEASE ${INSTALL_DIR}/bin/gmock.dll
            IMPORTED_LOCATION_DEBUG ${INSTALL_DIR}/bin/gmockd.dll
            IMPORTED_LOCATION_RELWITHDEBINFO ${INSTALL_DIR}/bin/gmock_relwithdebinfo.dll
            IMPORTED_LOCATION_MINSIZEREL ${INSTALL_DIR}/bin/gmock_minsizerel.dll
            IMPORTED_IMPLIB_RELEASE ${INSTALL_DIR}/lib/gmock.lib
            IMPORTED_IMPLIB_DEBUG ${INSTALL_DIR}/lib/gmockd.lib
            IMPORTED_IMPLIB_RELWITHDEBINFO ${INSTALL_DIR}/lib/gmock_relwithdebinfo.lib
            IMPORTED_IMPLIB_MINSIZEREL ${INSTALL_DIR}/lib/gmock_minsizerel.lib
            INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
            INTERFACE_LINK_LIBRARIES Threads::Threads
        )

        # GMock_Main
        set_target_properties(GTest::gmock_main PROPERTIES
            IMPORTED_LOCATION_RELEASE ${INSTALL_DIR}/bin/gmock_main.dll
            IMPORTED_LOCATION_DEBUG ${INSTALL_DIR}/bin/gmock_maind.dll
            IMPORTED_LOCATION_RELWITHDEBINFO ${INSTALL_DIR}/bin/gmock_main_relwithdebinfo.dll
            IMPORTED_LOCATION_MINSIZEREL ${INSTALL_DIR}/bin/gmock_main_minsizerel.dll
            IMPORTED_IMPLIB_RELEASE ${INSTALL_DIR}/lib/gmock_main.lib
            IMPORTED_IMPLIB_DEBUG ${INSTALL_DIR}/lib/gmock_maind.lib
            IMPORTED_IMPLIB_RELWITHDEBINFO ${INSTALL_DIR}/lib/gmock_main_relwithdebinfo.lib
            IMPORTED_IMPLIB_MINSIZEREL ${INSTALL_DIR}/lib/gmock_main_minsizerel.lib
            INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
            INTERFACE_LINK_LIBRARIES Threads::Threads
        )

    else() # UNIX or APPLE

        # GTest
        set_target_properties(GTest::gtest PROPERTIES
            IMPORTED_LOCATION_RELEASE ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest.${CMAKE_FIND_LIBRARY_SUFFIXES}
            IMPORTED_LOCATION_DEBUG ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtestd.${CMAKE_FIND_LIBRARY_SUFFIXES}
            IMPORTED_LOCATION_RELWITHDEBINFO ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_relwithdebinfo.${CMAKE_FIND_LIBRARY_SUFFIXES}
            IMPORTED_LOCATION_MINSIZEREL ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_minsizerel.${CMAKE_FIND_LIBRARY_SUFFIXES}
            INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
            INTERFACE_LINK_LIBRARIES Threads::Threads
        )

        # GTest_Main
            set_target_properties(GTest::gtest_main PROPERTIES
            IMPORTED_LOCATION_RELEASE ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main.${CMAKE_FIND_LIBRARY_SUFFIXES}
            IMPORTED_LOCATION_DEBUG ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_maind.${CMAKE_FIND_LIBRARY_SUFFIXES}
            IMPORTED_LOCATION_RELWITHDEBINFO ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main_relwithdebinfo.${CMAKE_FIND_LIBRARY_SUFFIXES}
            IMPORTED_LOCATION_MINSIZEREL ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main_minsizerel.${CMAKE_FIND_LIBRARY_SUFFIXES}
            INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
            INTERFACE_LINK_LIBRARIES Threads::Threads
        )

        # GMock
        set_target_properties(GTest::gmock PROPERTIES
            IMPORTED_LOCATION_RELEASE ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gmock.${CMAKE_FIND_LIBRARY_SUFFIXES}
            IMPORTED_LOCATION_DEBUG ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gmockd.${CMAKE_FIND_LIBRARY_SUFFIXES}
            IMPORTED_LOCATION_RELWITHDEBINFO ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_relwithdebinfo.${CMAKE_FIND_LIBRARY_SUFFIXES}
            IMPORTED_LOCATION_MINSIZEREL ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_minsizerel.${CMAKE_FIND_LIBRARY_SUFFIXES}
            INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
            INTERFACE_LINK_LIBRARIES Threads::Threads
        )

        # GMock_Main
        set_target_properties(GTest::gmock_main PROPERTIES
            IMPORTED_LOCATION_RELEASE ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_main.${CMAKE_FIND_LIBRARY_SUFFIXES}
            IMPORTED_LOCATION_DEBUG ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_maind.${CMAKE_FIND_LIBRARY_SUFFIXES}
            IMPORTED_LOCATION_RELWITHDEBINFO ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_main_relwithdebinfo.${CMAKE_FIND_LIBRARY_SUFFIXES}
            IMPORTED_LOCATION_MINSIZEREL ${INSTALL_DIR}/lib/${CMAKE_FIND_LIBRARY_PREFIXES}gmock_main_minsizerel.${CMAKE_FIND_LIBRARY_SUFFIXES}
            INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include
            INTERFACE_LINK_LIBRARIES Threads::Threads
        )


    endif() 
endfunction()