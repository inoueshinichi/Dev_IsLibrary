#pragma once

#ifdef _MSC_VER
    // https://qiita.com/Chironian/items/462a3bdf271d5f0b00b6#%EF%BC%92%EF%BC%93c4251%E8%AD%A6%E5%91%8A%E3%81%8C%E5%87%BA%E3%82%8B
    #pragma warning(disable:4251)

    #define NOMAXMIN // workaround for windows max-min macro
#endif

#if defined(_MSC_VER) && !defined(__CUDACC__)
    #if defined(IsNdArray_EXPORTS) || defined(IsNdArray_dbg_EXPORTS)
        #define NBLA_API __declspec(dllexport)
    #else
        #define NBLA_API __declspec(dllimport)
    #endif
#else
    #define NBLA_API
#endif

// C++11 gives alignas as standard
// http://en.cppreference.com/w/cpp/language/alignas
#define NBLA_ALIGN(N) alignas(N)

// クラス型を取得するためのヘルパーマクロ
// これを使用するために，このマクロの前に<type_traits>をインクルードしなければならない.
#define NBLA_THIS_TYPE std::remove_pointer_t<decltype(this)>