/**
 * @file defs.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#if defined(_MSC_VER)
    // __func__の定義
    #define __func__ __FUNCTION__
#endif

// dll/lib
#if defined(_MSC_VER) && !defined(__CUDACC__)
    #if defined(IsCommon_EXPORTS) || defined(IsCommon_dbg_EXPORTS)
        #define IS_COMMON_API __declspec(dllexport)
    #else
        #define IS_COMMON_API __declspec(dllimport)
    #endif
#else
    #define IS_COMMON_API
#endif

#if defined(_MSC_VER) && _MSC_VER < 1925 // >= Visual Studio 2019 16.5.1
    // _MSVC_TRADITIONAL 従来:1, 準拠:0
    #if defined(_MSVC_TRADITIONAL) && _MSVC_TRADITIONAL // 従来型プリプロセッサ
        // マクロ内の可変長マクロの展開を補助するマクロを定義
        #define IS_MACRO_VA_EXPAND(...) __VA_ARGS__
    #endif
#endif

// コピーコンストラクタとコピー代入演算子の外部公開を禁止する
#define IS_DISABLE_COPY_AND_ASSIGN(class_name)         \
private:                                               \
    class_name(const class_name &) = delete;           \
    class_name &operator=(const class_name &) = delete;

// <= C++17 or C++20
#if (__cplusplus < 201704L)
    #define IS_INVOKE_RESULT_TYPE(Func, Args) typename std::result_of<Func(Args...)>::type
#else
    #define IS_INVOKE_RESULT_TYPE(Func, Args) typename std::invoke_result<Func, Args...>::type
#endif


// プリミティブ型のエイリアス
using byte = char;
using ubyte = unsigned char;
using int16 = short;
using uint16 = unsigned short;
using int32 = int;
using uint32 = unsigned int;

#if defined(_WIN32)
    using int64 = long long;
    using uint64 = unsigned long long;
#else
    using int64 = long;
    using uint64 = unsigned long;
#endif
