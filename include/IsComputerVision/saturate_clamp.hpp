/**
 * @file saturate_clamp.hpp
 * @author your name (you@domain.com)
 * @brief OpenCVのsaturate_castをコピー
 * @version 0.1
 * @date 2021-07-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <cmath> // std::fmax, std::fmin, std::roundf, std::round etc..
#include <algorithm> // std::max, std::min
#include <limits.h>

namespace is
{
    namespace imgproc
    {
        using uchar = unsigned char;
        using ushort = unsigned short;
        using uint = unsigned int;
        using ulong = unsigned long;
        using longlong = long long;
        using ulonglong = unsigned long long;
        using longdouble = long double;

        // プライマリーテンプレート------------------------------------

        /*1byte*/
        // uchar
        template <typename T>
        static inline T saturate_clamp(uchar v) { return T(v); }
        // char
        template <typename T>
        static inline T saturate_clamp(char v) { return T(v); }

        /*2byte*/
        // ushort
        template <typename T>
        static inline T saturate_clamp(ushort v) { return T(v);  }
        // short
        template <typename T>
        static inline T saturate_clamp(short v) { return T(v); }

        /*4byte*/
        // uint
        template <typename T>
        static inline T saturate_clamp(uint v) { return T(v); }
        // int
        template <typename T>
        static inline T saturate_clamp(int v) { return T(v); }
        // float
        template <typename T>
        static inline T saturate_clamp(float v) { return T(v); }

        /*4byte or 8byte*/
        // ulong
        template <typename T>
        static inline T saturate_clamp(ulong v) { return T(v); }
        // long
        template <typename T>
        static inline T saturate_clamp(long v) { return T(v); }

        /*8byte*/
        // ulonglong
        template <typename T>
        static inline T saturate_clamp(ulonglong v) { return T(v); }
        // longlong
        template <typename T>
        static inline T saturate_clamp(longlong v) { return T(v); }
        // double
        template <typename T>
        static inline T saturate_clamp(double v) { return T(v); }

        /*16byte*/
        // long double
        template <typename T>
        static inline T saturate_clamp(longdouble v) { return T(v); }


        // 完全特殊化テンプレート------------------------------------

        // ----- uchar -----
        // uchar -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(uchar v) { return v; }
        // char -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(char v) { return (uchar)(std::max)((int)v, 0); }
        // ushort -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(ushort v) { return (uchar)((std::max)((uchar)v, (uchar)UCHAR_MAX)); }
        // int -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(int v) { return (uchar)((uint)v <= (uint)UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0); }
        // short -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(short v) { return saturate_clamp<uchar>((int)v); }
        // uint -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(uint v) { return (uchar)((std::min)(v, (uint)UCHAR_MAX)); }
        // ulong -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(ulong v) { return saturate_clamp<uchar>((uint)v); }
        // long -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(long v) { return saturate_clamp<uchar>((int)v); }
        // ulonglong -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(ulonglong v) { return (uchar)((std::min)(v, (ulonglong)UCHAR_MAX)); }
        // longlong -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(longlong v) { return (uchar)((ulonglong)v <= (ulonglong)UCHAR_MAX ? v : v > 0 ? UCHAR_MAX : 0); }
        // float -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(float v) { 
            int iv = (int)std::roundf(v);
            return saturate_clamp<uchar>(iv);
        }
        // double -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(double v) {
            int iv = (int)std::round(v);
            return saturate_clamp<uchar>(iv);
        }
        // longdouble -> uchar
        template <>
        inline uchar saturate_clamp<uchar>(longdouble v) {
            return saturate_clamp<uchar>((double)v);
        }


        // ----- char -----
        // char -> char
        template <>
        inline char saturate_clamp<char>(char v) { return v; }
        // uchar -> char
        template <>
        inline char saturate_clamp<char>(uchar v) { return (char)((std::min)((int)v, SCHAR_MAX)); }
        // ushort -> char
        template <>
        inline char saturate_clamp<char>(ushort v) { return (char)((std::min)((uint)v, (uint)SCHAR_MAX)); }
        // int -> char
        template <>
        inline char saturate_clamp<char>(int v) { return (char)((uint)(v - SCHAR_MIN) <= (uint)UCHAR_MAX ? v : v > 0 ? SCHAR_MAX : SCHAR_MIN); }
        // short -> char
        template <>
        inline char saturate_clamp<char>(short v) { return saturate_clamp<char>((int)v); }
        // uint -> char
        template <>
        inline char saturate_clamp<char>(uint v) { return (char)((std::min)(v, (uint)SCHAR_MAX)); }
        // long -> char
        template <>
        inline char saturate_clamp<char>(long v) { return saturate_clamp<char>((int)v); }
        // ulong -> char
        template <>
        inline char saturate_clamp<char>(ulong v) { return saturate_clamp<char>((uint)v); }
        // longlong -> char
        template <>
        inline char saturate_clamp<char>(longlong v) { return (char)((ulonglong)(v - SCHAR_MIN) <= (ulonglong)UCHAR_MAX ? v : v > 0 ? SCHAR_MAX : SCHAR_MIN); }
        // ulonglong -> char
        template <>
        inline char saturate_clamp<char>(ulonglong v) { return (char)((std::min)(v, (ulonglong)SCHAR_MAX)); }
        // float -> char
        template <>
        inline char saturate_clamp<char>(float v) {
            int iv = (int)std::roundf(v);
            return saturate_clamp<char>(iv);
        }
        // double -> char
        template <>
        inline char saturate_clamp<char>(double v) {
            int iv = (int)std::round(v);
            return saturate_clamp<char>(iv);
        }
        // longdouble -> char
        template <>
        inline char saturate_clamp<char>(longdouble v) {
            return saturate_clamp<char>((double)v);
        }


        // ----- ushort -----
        // char -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(char v) { return (ushort)((std::max)((int)v, 0)); }
        // uchar -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(uchar v) { return (ushort)(v); }
        // short -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(short v) { return (ushort)((std::max)((int)v, 0)); }
        // ushort -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(ushort v) { return v; }
        // int -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(int v) { return (ushort)((uint)v <= (uint)USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0); }
        // uint -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(uint v) { return (ushort)((std::min)(v, (uint)USHRT_MAX)); }
        // long -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(long v) { return saturate_clamp<ushort>((int)v); }
        // ulong -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(ulong v) { return saturate_clamp<ushort>((uint)v); }
        // longlong -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(longlong v) { return (ushort)((ulonglong)v <= (ulonglong)USHRT_MAX ? v : v > 0 ? USHRT_MAX : 0); }
        // ulonglong -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(ulonglong v) { return (ushort)((std::min)(v, (ulonglong)USHRT_MAX)); }
        // float -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(float v) {
            int iv = (int)std::roundf(v);
            return saturate_clamp<ushort>(iv);
        }
        // double -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(double v) {
            int iv = (int)std::round(v);
            return saturate_clamp<ushort>(iv);
        }
        // longdouble -> ushort
        template <>
        inline ushort saturate_clamp<ushort>(longdouble v) {
            return saturate_clamp<ushort>((double)v);
        }


        // ----- short -----
        // char -> short
        template <>
        inline short saturate_clamp<short>(char v) { return (short)v; }
        // uchar -> short
        template <>
        inline short saturate_clamp<short>(uchar v) { return (short)v; }
        // short -> short
        template <>
        inline short saturate_clamp<short>(short v) { return v; }
        // ushort -> short
        template <>
        inline short saturate_clamp<short>(ushort v) { return (short)((std::min)((int)v, SHRT_MAX)); }
        // int -> short
        template <>
        inline short saturate_clamp<short>(int v) { return (short)((uint)(v - SHRT_MIN) <= (uint)SHRT_MAX ? v : v > 0 ? SHRT_MAX : SHRT_MIN); }
        // uint -> short
        template <>
        inline short saturate_clamp<short>(uint v) { return (short)((std::min)(v, (uint)SHRT_MAX)); }
        // long -> short
        template <>
        inline short saturate_clamp<short>(long v) { return saturate_clamp<short>((int)v); }
        // ulong -> short
        template <>
        inline short saturate_clamp<short>(ulong v) { return saturate_clamp<short>((uint)v); }
        // longlong -> short
        template <>
        inline short saturate_clamp<short>(longlong v) { return (short)((ulonglong)(v - SHRT_MIN) <= (ulonglong)SHRT_MAX ? v : v > 0 ? SHRT_MAX : SHRT_MIN); }
        // ulonglong -> short
        template <>
        inline short saturate_clamp<short>(ulonglong v) { return (short)((std::min)(v, (ulonglong)SHRT_MAX)); }
        // float -> short
        template <>
        inline short saturate_clamp<short>(float v) {
            int iv = (int)std::roundf(v);
            return saturate_clamp<short>(iv);
        }
        // double -> short
        template <>
        inline short saturate_clamp<short>(double v) {
            int iv = (int)std::round(v);
            return saturate_clamp<short>(iv);
        }
        // longdouble -> short
        template <>
        inline short saturate_clamp<short>(longdouble v) {
            return saturate_clamp<short>((double)v);
        }


        // ----- uint -----
        // char -> uint
        template <>
        inline uint saturate_clamp<uint>(char v) { return (uint)((std::max)(v, (char)0)); }
        // uchar -> uint
        template <>
        inline uint saturate_clamp<uint>(uchar v) { return (uint)v; }
        // short -> uint
        template <>
        inline uint saturate_clamp<uint>(short v) { return (uint)((std::max)(v, (short)0)); }
        // ushort -> uint
        template <>
        inline uint saturate_clamp<uint>(ushort v) { return (uint)v; }
        // int -> uint
        template <>
        inline uint saturate_clamp<uint>(int v) { return (uint)((std::max)(v, (int)0)); }
        // uint -> uint
        template <>
        inline uint saturate_clamp<uint>(uint v) { return v; }
        // long -> uint
        template <>
        inline uint saturate_clamp<uint>(long v) { return saturate_clamp<uint>((int)v); }
        // ulong -> uint
        template <>
        inline uint saturate_clamp<uint>(ulong v) { return (uint)v; }
        // longlong -> uint
        template <>
        inline uint saturate_clamp<uint>(longlong v) { return (uint)((ulonglong)v <= (ulonglong)UINT_MAX ? v : v > 0 ? UINT_MAX : 0); }
        // ulonglong -> uint
        template <>
        inline uint saturate_clamp<uint>(ulonglong v) { return (uint)((std::min)(v, (ulonglong)UINT_MAX)); }
        // float -> uint
        template <>
        inline uint saturate_clamp<uint>(float v) {
            int iv = (int)std::roundf(v);
            return saturate_clamp<uint>(iv);
        }
        // double -> uint
        template <>
        inline uint saturate_clamp<uint>(double v) {
            int iv = (int)std::round(v);
            return saturate_clamp<uint>(iv);
        }
        // longdouble -> uint
        template <>
        inline uint saturate_clamp<uint>(longdouble v) {
            return saturate_clamp<uint>((double)v);
        }


        // ----- int -----
        // char -> int
        template <>
        inline int saturate_clamp<int>(char v) { return (int)v; }
        // uchar -> int
        template <>
        inline int saturate_clamp<int>(uchar v) { return (int)v; }
        // short -> int
        template <>
        inline int saturate_clamp<int>(short v) { return (int)v; }
        // ushort -> int
        template <>
        inline int saturate_clamp<int>(ushort v) { return (int)v; }
        // int -> int
        template <>
        inline int saturate_clamp<int>(int v) { return (int)v; }
        // uint -> int
        template <>
        inline int saturate_clamp<int>(uint v) { return (int)((std::min)(v, (uint)INT_MAX)); }
        // long -> int
        template <>
        inline int saturate_clamp<int>(long v) { return (int)v; }
        // ulong -> int
        template <>
        inline int saturate_clamp<int>(ulong v) { return saturate_clamp<int>((uint)v); }
        // longlong -> int
        template <>
        inline int saturate_clamp<int>(longlong v) { return saturate_clamp<int>((ulonglong)(v - INT_MIN) <= (ulonglong)UINT_MAX ? v : v > 0 ? INT_MAX : INT_MIN); }
        // ulonglong -> int
        template <>
        inline int saturate_clamp<int>(ulonglong v) { return (int)((std::min)(v, (ulonglong)INT_MAX)); }
        // float -> int
        template <>
        inline int saturate_clamp<int>(float v) {
            int iv = (int)std::roundf(v);
            return iv;
        }
        // double -> int
        template <>
        inline int saturate_clamp<int>(double v) {
            int iv = (int)std::round(v);
            return iv;
        }
        // longdouble -> int
        template <>
        inline int saturate_clamp<int>(longdouble v) {
            return saturate_clamp<int>((double)v);
        }


        // ----- ulong -----
        // char -> ulong
        template <>
        inline ulong saturate_clamp<ulong>(char v) { return (ulong)saturate_clamp<uint>(v); }
        // uchar -> ulong
        template <>
        inline ulong saturate_clamp<ulong>(uchar v) { return (ulong)saturate_clamp<uint>(v); }
        // short -> ulong
        template <>
        inline ulong saturate_clamp<ulong>(short v) { return (ulong)saturate_clamp<uint>(v); }
        // ushort -> ulong
        template <>
        inline ulong saturate_clamp<ulong>(ushort v) { return (ulong)saturate_clamp<uint>(v); }
        // int -> ulong
        template <>
        inline ulong saturate_clamp<ulong>(int v) { return (ulong)saturate_clamp<uint>(v); }
        // uint -> ulong
        template <>
        inline ulong saturate_clamp<ulong>(uint v) { return (ulong)v; }
        // long -> ulong
        template <>
        inline ulong saturate_clamp<ulong>(long v) { return (ulong)saturate_clamp<uint>(v); }
        // ulong -> ulong
        template <>
        inline ulong saturate_clamp<ulong>(ulong v) { return v; }
        // longlong -> ulong
        template <>
        inline ulong saturate_clamp<ulong>(longlong v) { return (ulong)saturate_clamp<uint>(v); }
        // float -> ulong
        template <>
        inline ulong saturate_clamp<ulong>(float v) { return (ulong)saturate_clamp<uint>(v); }
        // double -> ulong
        template <>
        inline ulong saturate_clamp<ulong>(double v) { return (ulong)saturate_clamp<uint>(v); }
        // longdouble -> ulong
        template <>
        inline ulong saturate_clamp<ulong>(longdouble v) { return saturate_clamp<ulong>((double)v); }


        // ----- long -----
        // char -> long
        template <>
        inline long saturate_clamp<long>(char v) { return (long)saturate_clamp<int>(v); }
        // uchar -> long
        template <>
        inline long saturate_clamp<long>(uchar v) { return (long)saturate_clamp<int>(v); }
        // short -> long
        template <>
        inline long saturate_clamp<long>(short v) { return (long)saturate_clamp<int>(v); }
        // ushort -> long
        template <>
        inline long saturate_clamp<long>(ushort v) { return (long)saturate_clamp<int>(v); }
        // int -> long
        template <>
        inline long saturate_clamp<long>(int v) { return (long)v; }
        // uint -> long
        template <>
        inline long saturate_clamp<long>(uint v) { return (long)saturate_clamp<int>(v); }
        // long -> long
        template <>
        inline long saturate_clamp<long>(long v) { return v; }
        // ulong -> long
        template <>
        inline long saturate_clamp<long>(ulong v) { return (long)saturate_clamp<int>(v); }
        // longlong -> long
        template <>
        inline long saturate_clamp<long>(longlong v) { return (long)saturate_clamp<int>(v); }
        // ulonglong -> long
        template <>
        inline long saturate_clamp<long>(ulonglong v) { return (long)saturate_clamp<int>(v); }
        // float -> long
        template <>
        inline long saturate_clamp<long>(float v) { return (long)saturate_clamp<int>(v); }
        // double -> long
        template <>
        inline long saturate_clamp<long>(double v) { return (long)saturate_clamp<int>(v); }


        // ----- ulonglong -----
        // char -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(char v) { return (ulonglong)((std::max)(v, (char)v)); }
        // uchar -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(uchar v) { return (ulonglong)v; }
        // short -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(short v) { return (ulonglong)((std::max)(v, (short)0)); }
        // ushort -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(ushort v) { return (ulonglong)v; }
        // int -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(int v) { return (ulonglong)((std::max)(v, (int)0)); }
        // uint -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(uint v) { return (ulonglong)v; }
        // long -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(long v) { return saturate_clamp<ulonglong>((int)v); }
        // ulong -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(ulong v) { return saturate_clamp<ulonglong>((uint)v); }
        // longlong -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(longlong v) { return (ulonglong)((std::max)(v, (longlong)0)); }
        // ulonglong -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(ulonglong v) { return v; }
        // float -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(float v) {
            longlong llv = (longlong)std::roundf(v);
            return saturate_clamp<ulonglong>(llv);
        }
        // double -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(double v) {
            longlong llv = (longlong)std::round(v);
            return saturate_clamp<ulonglong>(llv);
        }
        // longdouble -> ulonglong
        template <>
        inline ulonglong saturate_clamp<ulonglong>(longdouble v) {
            return saturate_clamp<ulonglong>((double)v);
        }


        // ----- longlong -----
        // char -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(char v) { return (longlong)v; }
        // uchar -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(uchar v) { return (longlong)v; }
        // short -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(short v) { return (longlong)v; }
        // ushort -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(ushort v) { return (longlong)v; }
        // int -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(int v) { return (longlong)v; }
        // uint -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(uint v) { return (longlong)v; }
        // long -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(long v) { return (longlong)v; }
        // ulong -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(ulong v) { return (longlong)v; }
        // longlong -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(longlong v) { return v; }
    #ifdef WIN32
        // ulonglong -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(ulonglong v) { return (longlong)((std::min)(v, (ulonglong)LLONG_MAX)); }
    #else
        // ulonglong -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(ulonglong v) { return (longlong)((std::min)(v, (ulonglong)LONG_LONG_MAX)); }
    #endif
        // float -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(float v) { return (longlong)(std::roundf(v)); }
        // double -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(double v) { return (longlong)(std::round(v)); }
        // longdouble -> longlong
        template <>
        inline longlong saturate_clamp<longlong>(longdouble v) { return saturate_clamp<longlong>((double)v); }


        // ----- float -----
        // char -> float
        template <>
        inline float saturate_clamp<float>(char v) { return (float)v; }
        // uchar -> float
        template <>
        inline float saturate_clamp<float>(uchar v) { return (float)v; }
        // short -> float
        template <>
        inline float saturate_clamp<float>(short v) { return (float)v; }
        // ushort -> float
        template <>
        inline float saturate_clamp<float>(ushort v) { return (float)v; }
        // int -> float
        template <>
        inline float saturate_clamp<float>(int v) { return (float)v; }
        // uint -> float
        template <>
        inline float saturate_clamp<float>(uint v) { return (float)v; }
        // long -> float
        template <>
        inline float saturate_clamp<float>(long v) { return (float)v; }
        // ulong -> float
        template <>
        inline float saturate_clamp<float>(ulong v) { return (float)v; }
        // longlong -> float
        template <>
        inline float saturate_clamp<float>(longlong v) { return (float)v; }
        // ulonglong -> float
        template <>
        inline float saturate_clamp<float>(ulonglong v) { return (float)v; }
        // float -> float
        template <>
        inline float saturate_clamp<float>(float v) { return v; }
        // double -> float
        template <>
        inline float saturate_clamp<float>(double v) { return (float)v; }


        // ----- double -----
        // char -> double
        template <>
        inline double saturate_clamp<double>(char v) { return (double)v; }
        // uchar -> double
        template <>
        inline double saturate_clamp<double>(uchar v) { return (double)v; }
        // short -> double
        template <>
        inline double saturate_clamp<double>(short v) { return (double)v; }
        // ushort -> double
        template <>
        inline double saturate_clamp<double>(ushort v) { return (double)v; }
        // int -> double
        template <>
        inline double saturate_clamp<double>(int v) { return (double)v; }
        // uint -> double
        template <>
        inline double saturate_clamp<double>(uint v) { return (double)v; }
        // long -> double
        template <>
        inline double saturate_clamp<double>(long v) { return (double)v; }
        // ulong -> double
        template <>
        inline double saturate_clamp<double>(ulong v) { return (double)v; }
        // longlong -> double
        template <>
        inline double saturate_clamp<double>(longlong v) { return (double)v; }
        // ulonglong -> double
        template <>
        inline double saturate_clamp<double>(ulonglong v) { return (double)v; }
        // float -> double
        template <>
        inline double saturate_clamp<double>(float v) { return (double)v; }
        // double -> double
        template <>
        inline double saturate_clamp<double>(double v) { return v; }
        // longdougle -> double
        template <>
        inline double saturate_clamp<double>(longdouble v) { return (double)v; }


        // ----- longdouble -----
        // char -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(char v) { return (longdouble)v; }
        // uchar -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(uchar v) { return (longdouble)v; }
        // short -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(short v) { return (longdouble)v; }
        // ushort -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(ushort v) { return (longdouble)v; }
        // int -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(int v) { return (longdouble)v; }
        // uint -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(uint v) { return (longdouble)v; }
        // long -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(long v) { return (longdouble)v; }
        // ulong -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(ulong v) { return (longdouble)v; }
        // longlong -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(longlong v) { return (longdouble)v; }
        // ulonglong -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(ulonglong v) { return (longdouble)v; }
        // float -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(float v) { return (longdouble)v; }
        // double -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(double v) { return (longdouble)v; }
        // longdouble -> longdouble
        template <>
        inline longdouble saturate_clamp<longdouble>(longdouble v) { return v; }
    }
}