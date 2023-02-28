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


/**
 * @brief Check valid compiliers
 * [Valid] MSVC, Clang, AppleClang, GNU
 * [Invalid] mingw32, mingw64, cygwin32, cygwin64
 */
#if defined(__MINGW32__) || defined(__MINGW64__) || defined(__CYGWIN__)
#error Invalid compiler, MINGW32, MINGW64, CYGWIN.
#endif


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


/**
 * @brief Change behavier of preprocessor at MSVC.
 * 
 */
#if defined(_MSC_VER) && _MSC_VER < 1925 // >= Visual Studio 2019 16.5.1
    // _MSVC_TRADITIONAL conventional: 1, standard: 0
    #if defined(_MSVC_TRADITIONAL) && _MSVC_TRADITIONAL // Conventional preprocessor of MSVC
        // Unpack VA macro support in macro.
        #define IS_MACRO_VA_EXPAND(...) __VA_ARGS__
    #endif
#endif


// Disable copy constructor and copy assign
#define IS_DISABLE_COPY_AND_ASSIGN(class_name)         \
private:                                               \
    class_name(const class_name &) = delete;           \
    class_name &operator=(const class_name &) = delete;

// Disable move constructor and move assign
#define IS_DISABLE_MOVE_AND_ASSIGN(class_name)     \
    class_name(const class_name &&) = delete;      \
    class_name &operator=(class_name &&) = delete;

// Disable copy, move constructors and copy, move assigns
#define IS_DISABLE_COPY_MOVE_AND_ASSIGN(class_name)    \
    IS_DISABLE_COPY_AND_ASSIGN(class_name)             \
    IS_DISABLE_MOVE_AND_ASSIGN(class_name)

// Disable implicit constructors
#define IS_DISABLE_IMPLICIT_CONSTRUCTORS(class_name)   \
    class_name() = delete;                             \
    IS_DISABLE_COPY_MOVE_AND_ASSIGN(class_name)

#if (__cplusplus < 201704L) // <= C++17
    #define IS_INVOKE_RESULT_TYPE(Func, Args) typename std::result_of<Func(Args...)>::type
#else // C++20
    #define IS_INVOKE_RESULT_TYPE(Func, Args) typename std::invoke_result<Func, Args...>::type
#endif

// C++11 gives alignas as standard for struct.
// http://en.cppreference.com/w/cpp/language/alignas
#define IS_ALIGN(N) std::alignas(N)

// Zero memory
#include <cstring>
#define IS_ZERO_MEMORY(ptr, bytesize) std::memset((void *)ptr, 0, bytesize)

/**
 * @brief Alias for primitive type
 * 
 */
using uchar = unsigned char;
using byte = char;
using ubyte = unsigned char;
using ushort = unsigned short;
using uint = unsigned int;
using ulong = unsigned long; // If windows 32bit env, then it have 4byte. In case of Unix, 8byte.
using longlong = long long;  // Both windows and unix have 8byte.
using ulonglong = unsigned long long;
using int16 = short;
using uint16 = unsigned short;
using int32 = int;
using uint32 = unsigned int;
using int64 = long long;
using uint64 = unsigned long long;
using longdouble = long double;

/**
 * @brief Primitive type range.
 * 
 */
#include <limits>
constexpr uchar IS_UCHAR_MAX = std::numeric_limits<uchar>::max();
constexpr uchar IS_UCHAR_MIN = std::numeric_limits<uchar>::min();
constexpr byte IS_BYTE_MAN = std::numeric_limits<byte>::max();
constexpr byte IS_BYTE_MIN = std::numeric_limits<byte>::min();
constexpr ubyte IS_UBYTE_MAX = std::numeric_limits<ubyte>::max();
constexpr ubyte IS_UBYTE_MIN = std::numeric_limits<ubyte>::min();
constexpr short IS_SHORT_MAX = std::numeric_limits<short>::max();
constexpr short IS_SHORT_MIN = std::numeric_limits<short>::min();
constexpr ushort IS_USHORT_MAX = std::numeric_limits<ushort>::max();
constexpr ushort IS_USHORT_MIN = std::numeric_limits<ushort>::min();
constexpr int IS_INT_MAX = std::numeric_limits<int>::max();
constexpr int IS_INT_MIN = std::numeric_limits<int>::min();
constexpr uint IS_UINT_MAX = std::numeric_limits<uint>::max();
constexpr uint IS_UINT_MIN = std::numeric_limits<uint>::min();
constexpr long IS_LONG_MAX = std::numeric_limits<long>::max();
constexpr long IS_LONG_MIN = std::numeric_limits<long>::min();
constexpr ulong IS_ULONG_MAX = std::numeric_limits<ulong>::max();
constexpr ulong IS_ULONG_MIN = std::numeric_limits<ulong>::min();
constexpr longlong IS_LONGLONG_MAX = std::numeric_limits<longlong>::max();
constexpr longlong IS_LONGLONG_MIN = std::numeric_limits<longlong>::min();
constexpr ulonglong IS_ULONGLONG_MAX = std::numeric_limits<ulonglong>::max();
constexpr ulonglong IS_ULONGLONG_MIN = std::numeric_limits<ulonglong>::min();

constexpr float IS_FLOAT_MAX = std::numeric_limits<float>::max();
constexpr float IS_FLOAT_MIN = std::numeric_limits<float>::min();
constexpr float IS_FLOAT_NAN = std::numeric_limits<float>::quiet_NaN();
constexpr float IS_FLOAT_INF = std::numeric_limits<float>::infinity();

constexpr double IS_DOUBLE_MAX = std::numeric_limits<double>::max();
constexpr double IS_DOUBLE_MIN = std::numeric_limits<double>::min();
constexpr double IS_DOUBLE_NAN = std::numeric_limits<double>::quiet_NaN();
constexpr double IS_DOUBLE_INF = std::numeric_limits<double>::infinity();

constexpr longdouble IS_LONGDOUBLE_MAX = std::numeric_limits<longdouble>::max();
constexpr longdouble IS_LONGDOUBLE_MIN = std::numeric_limits<longdouble>::min();
constexpr longdouble IS_LONGDOUBLE_NAN = std::numeric_limits<longdouble>::quiet_NaN();
constexpr longdouble IS_LONGDOUBLE_INF = std::numeric_limits<longdouble>::infinity();

/**
 * @brief Platform identifier
 * Windows 64bit _WIN64
 * Windows 32bit _WIN32
 * Linux         __linux__
 * macOS         __MACH__
 */