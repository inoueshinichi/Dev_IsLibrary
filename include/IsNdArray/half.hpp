#pragma once

#include <IsNdArray/IsNdArray.hpp>
#include <IsNdArray/dtypes.hpp>
#include <IsNdArray/exception.hpp>

#include <IsNdArray/float_bits.hpp>

#include <cmath>
#include <limits>


namespace is {
    namespace nbla {
        inline uint16_t float2halfbits(float fvalue) {
            // unionでビット単位で共有
            union {
                uint32_t bits;
                float value_;
            };

            value_ = fvalue;
            return float_bits<float>::downconvert_to<Half>(bits);
        }


        inline float halfbits2float(uint16_t hbits) {
            union {
                uint32_t fbits;
                float fvalue;
            };

            fbits = float_bits<Half>::upconvert_to<float>(hbits);
            return fvalue;
        }


        /**
         * @brief Half (half floating point)クラス
         * float 4bytes
         * half  2bytes
         * 
         */
        struct NBLA_ALIGN(2) Half {
            uint16_t bits;

            // Constructors
            Half();
        #define CST(type) NBLA_API Half(const type& rhs)
            CST(bool);
            CST(unsigned char);
            CST(char);
            CST(unsigned short);
            CST(short);
            CST(unsigned int);
            CST(int);
            CST(unsigned long);
            CST(long);
            CST(unsigned long long);
            CST(long long);
            CST(float);
            CST(double);
            CST(long double);
            CST(Half);
         #undef CST

            // Assignment Operators
            NBLA_API Half& operator+=(const Half& rhs);
            NBLA_API Half& operator-=(const Half& rhs);
            NBLA_API Half& operator*=(const Half& rhs);
            NBLA_API Half& operator/=(const Half& rhs);
            NBLA_API Half& operator=(const Half& rhs);
        #define CST(type) NBLA_API operator type() const
            CST(bool);
            CST(unsigned char);
            CST(char);
            CST(unsigned short);
            CST(short);
            CST(unsigned int);
            CST(int);
            CST(unsigned long);
            CST(long);
            CST(unsigned long long);
            CST(long long);
            CST(float);
            CST(double);
            CST(long double);
        #undef CTS

            // Arithmetic operators
            NBLA_API Half operator+() const;
            NBLA_API Half operator-() const;
        #define AOP(op, type) NBLA_API Half operator op(type o) const
        #define AOPR(op, type) NBLA_API type operator op(type o) const
        #define AOP_TYPE(op)                                           \
            AOP(op, bool);                                             \
            AOP(op, unsigned char);                                    \
            AOP(op, char);                                             \
            AOP(op, unsigned short);                                   \
            AOP(op, short);                                            \
            AOP(op, unsigned int);                                     \
            AOP(op, int);                                              \
            AOP(op, unsigned long);                                    \
            AOP(op, long);                                             \
            AOP(op, unsigned long long);                               \
            AOP(op, long long);                                        \
            AOPR(op, float);                                           \
            AOPR(op, double);                                          \
            AOPR(op, long double);                                     \
            AOP(op, Half)

            AOP_TYPE(+);
            AOP_TYPE(-);
            AOP_TYPE(*);
            AOP_TYPE(/);

        #undef AOP_TYPE
        #undef AOP
        #undef AOPR
        };


        // Inverse arithmetic operators
        #define AOP(op, type)                                           \
            NBLA_API Half operator op(const type &lhs, const Half &rhs)
        #define AOPR(op, type)                                          \
            NBLA_API type operator op(const type &lhs, const Half &rhs)
        #define AOP_TYPE(op)                                            \
            AOP(op, bool);                                              \
            AOP(op, unsigned char);                                     \
            AOP(op, char);                                              \
            AOP(op, unsigned short);                                    \
            AOP(op, short);                                             \
            AOP(op, unsigned int);                                      \
            AOP(op, int);                                               \
            AOP(op, unsigned long);                                     \
            AOP(op, long);                                              \
            AOP(op, unsigned long long);                                \
            AOP(op, long long);                                         \
            AOPR(op, float);                                            \
            AOPR(op, double);                                           \
            AOPR(op, long double)

            AOP_TYPE(+);
            AOP_TYPE(-);
            AOP_TYPE(*);
            AOP_TYPE(/);

        #undef AOP_TYPE
        #undef AOP
        #undef AOPR


        // Relational operators
        #define ROP_TYPE(op, type)                                  \
        NBLA_API bool operator op(const Half &lhs, const type &rhs)
        #define IROP_TYPE(op, type)                                 \
        NBLA_API bool operator op(const type &lhs, const Half &rhs)
        #define ROP(type)                                           \
            ROP_TYPE(<, type);                                      \
            ROP_TYPE(>, type);                                      \
            ROP_TYPE(<=, type);                                     \
            ROP_TYPE(>=, type);                                     \
            ROP_TYPE(==, type);                                     \
            ROP_TYPE(!=, type);                                     \
            IROP_TYPE(<, type);                                     \
            IROP_TYPE(>, type);                                     \
            IROP_TYPE(<=, type);                                    \
            IROP_TYPE(>=, type)

            ROP(bool);
            ROP(unsigned char);
            ROP(char);
            ROP(unsigned short);
            ROP(short);
            ROP(unsigned int);
            ROP(int);
            ROP(unsigned long);
            ROP(long);
            ROP(unsigned long long);
            ROP(long long);
            ROP(float);
            ROP(double);
            ROP(long double);
            ROP_TYPE(<, Half);
            ROP_TYPE(>, Half);
            ROP_TYPE(<=, Half);
            ROP_TYPE(>=, Half);
            ROP_TYPE(==, Half);
            ROP_TYPE(!=, Half);

        #undef ROP_TYPE
        #undef IROP_TYPE
        #undef ROP

        template <typename T> struct force_float { using type = T; };
        template <> struct force_float<Half> { using type = float; };
    }
}


// cmath function

namespace std {
    using namespace is::nbla;
#define MATHF(func) NBLA_API Half func(const Half& h)
    MATHF(exp);
    MATHF(log);
    MATHF(log2);
    MATHF(log1p);
    MATHF(sqrt);
    MATHF(sin);
    MATHF(cos);
    MATHF(tan);
    MATHF(sinh);
    MATHF(cosh);
    MATHF(tanh);
    MATHF(asin);
    MATHF(acos);
    MATHF(atan);
    MATHF(asinh);
    MATHF(acosh);
    MATHF(atanh);
    MATHF(fabs);
    MATHF(abs);
    MATHF(floor);
    MATHF(ceil);
    MATHF(round);
    MATHF(isnan);
    MATHF(isinf);
    NBLA_API Half pow(const Half& a, const Half& b);
    NBLA_API Half pow(const Half& a, const int& b);
    NBLA_API Half max(const Half& a, const int& b);
    NBLA_API Half atan2(const Half& a, const Half& b);
    NBLA_API Half ldexp(const Half& a, const int& b);
    #undef MATHF

    template <> 
    class numeric_limits<Half> {
    public:
        inline static Half min() { return 6.10352e-5; }
        inline static Half max() { return 3.2768e+4; }
        static constexpr bool is_integer = false;
        static constexpr bool is_signed = true;
    };
}