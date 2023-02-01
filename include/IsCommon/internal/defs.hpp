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

#ifdef _MSC_VER
    // https://qiita.com/Chironian/items/462a3bdf271d5f0b00b6#%EF%BC%92%EF%BC%93c4251%E8%AD%A6%E5%91%8A%E3%81%8C%E5%87%BA%E3%82%8B
    #pragma warning(disable : 4251)
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

// C++11 gives alignas as standard
// http://en.cppreference.com/w/cpp/language/alignas
#define IS_ALIGN(N) alignas(N)

// Memory
#include <cstring>
#define IS_ZERO_MEMORY(ptr, bytesize) std::memset((void *)ptr, 0, bytesize)

// プリミティブ型のエイリアス
using uchar = unsigned char;
using byte = char;
using ubyte = unsigned char;
using ushort = unsigned short;
using ulong = unsigned long;
using longlong = long long;
using ulonglong = unsigned long long;

using int16 = short;
using uint16 = unsigned short;

using uint = unsigned int;


#if defined(_WIN64) && defined(_WIN32) // 32bit環境
    using int32 = long;
    using uint32 = ulong;
    using int64 = longlong;
    using uint64 = ulonglong;
#else
    #if defined(__linux__) // Linux
        using int32 = int;
        using uint32 = unsigned int;
        using int64 = long;
        using uint64 = ulong;
    #elseif defined(__MACH__)  // macOS
        using int32 = int;
        using uint32 = unsigned int;
        using int64 = longlong;
        using uint64 = ulonglong;
    #else
        using int32 = int;
        using uint32 = unsigned int;
        using int64 = longlong;
        using uint64 = ulonglong;
    #endif
#endif

using longdouble = long double;