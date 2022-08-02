#include <IsNdArray/half.hpp>


namespace is
{
    namespace nbla
    {
        // Constructor
        Half::Half() {}
    #define CST1(TYPE)                                                                \
        Half::Half(const TYPE& rhs) { bits = float2halfbits(rhs); } // ここで, e.g double -> float -> half

        CST1(bool)
        CST1(unsigned char)
        CST1(char)
        CST1(unsigned short)
        CST1(short)
        CST1(unsigned int)
        CST1(int)
        CST1(unsigned long)
        CST1(long)
        CST1(unsigned long long)
        CST1(long long)
        CST1(double)
        CST1(long double)

    #undef CST1
        Half::Half(const float& rhs) { bits = float2halfbits(rhs); }
        Half::Half(const Half& rhs) { bits = rhs.bits; }
    

        // Conversion to scalar
        Half::operator float() const { return halfbits2float(bits); }
    #define CST2(TYPE)                                                                \
        Half::operator TYPE() const { return (TYPE)((float)(*this)); }

        CST2(bool)
        CST2(unsigned char)
        CST2(char)
        CST2(unsigned short)
        CST2(short)
        CST2(unsigned int)
        CST2(int)
        CST2(unsigned long)
        CST2(long)
        CST2(unsigned long long)
        CST2(long long)
        CST2(double)
        CST2(long double)

    #undef CST2


        // Assignment Operators
        Half& Half::operator=(const Half& rhs) 
        {
            bits = rhs.bits;
            return *this;
        }
    #define DUMMY_OP(OP)                                                             \
        Half& Half::operator OP(const Half& rhs)                                     \
        {                                                                            \
            float tmp = (float)(*this);                                              \
            tmp OP(float) rhs;                                                       \
            *this = tmp;                                                             \
            return *this;                                                            \
        }

        DUMMY_OP(+=)
        DUMMY_OP(-=)
        DUMMY_OP(*=)
        DUMMY_OP(/=)

    #undef DUMMY_OP


        // Arithmetic operators
        Half Half::operator+() const { return *this; }
        Half Half::operator-() const 
        {
            Half ret(*this);
            ret *= -1;
            return ret;
        }
    #define AOP(OP, TYPE)                                                            \
        Half Half::operator OP(TYPE o) const { return Half((float)(*this)OP o); }  
    #define AOPR(OP, TYPE)                                                           \
        TYPE Half::operator OP(TYPE o) const { return (float)(*this)OP o; }  
    #define AOP_TYPE(OP)                                                             \
        AOP(OP, bool)                                                                \
        AOP(OP, unsigned char)                                                       \
        AOP(OP, char)                                                                \
        AOP(OP, unsigned short)                                                      \
        AOP(OP, short)                                                               \
        AOP(OP, unsigned int)                                                        \
        AOP(OP, int)                                                                 \
        AOP(OP, unsigned long)                                                       \
        AOP(OP, long)                                                                \
        AOP(OP, unsigned long long)                                                  \
        AOP(OP, long long)                                                           \
        AOPR(OP, float)                                                              \
        AOPR(OP, double)                                                             \
        AOPR(OP, long double)                                                        \
        Half Half::operator OP(Half o) const                                         \
        {                                                                            \
            return Half((float)(*this)OP(float) o);                                  \
        }

        AOP_TYPE(+)
        AOP_TYPE(-)
        AOP_TYPE(*)
        AOP_TYPE(/)

    #undef AOP_TYPE
    #undef AOP
    #undef AOPR


    #define AOP(OP, TYPE)                                                            \
        Half operator OP(const TYPE& lhs, const Half& rhs)                           \
        {                                                                            \
            return Half(lhs OP(float) rhs);                                          \
        }  
    #define AOPR(OP, TYPE)                                                           \
        TYPE operator OP(const TYPE& lhs, const Half& rhs)                           \
        {                                                                            \
            return lhs OP(float) rhs;                                                \
        }
    #define AOP_TYPE(OP)                                                             \
        AOP(OP, bool)                                                                \
        AOP(OP, unsigned char)                                                       \
        AOP(OP, char)                                                                \
        AOP(OP, unsigned short)                                                      \
        AOP(OP, short)                                                               \
        AOP(OP, unsigned int)                                                        \
        AOP(OP, int)                                                                 \
        AOP(OP, unsigned long)                                                       \
        AOP(OP, long)                                                                \
        AOP(OP, unsigned long long)                                                  \
        AOP(OP, long long)                                                           \
        AOPR(OP, float)                                                              \
        AOPR(OP, double)                                                             \
        AOPR(OP, long double)

        AOP_TYPE(+)
        AOP_TYPE(-)
        AOP_TYPE(*)
        AOP_TYPE(/)

    #undef AOP_TYPE
    #undef AOP
    #undef AOPR


        // Relational operators
    #define ROP_TYPE(OP, TYPE) bool operator OP(const Half& lhs, const TYPE& rhs)
    #define IROP_TYPE(OP, TYPE) bool operator OP(const TYPE& lhs, const Half& rhs)
    #define ROP(TYPE)                                                                \
        ROP_TYPE(<, TYPE) { return (float)lhs < rhs; }                               \
        ROP_TYPE(>, TYPE) { return rhs < lhs; }                                      \
        ROP_TYPE(<=, TYPE) { return !(lhs > rhs); }                                  \
        ROP_TYPE(>=, TYPE) { return !(lhs < rhs); }                                  \
        ROP_TYPE(==, TYPE) { return (float)lhs == rhs; }                             \
        ROP_TYPE(!=, TYPE) { return !(lhs == rhs); }                                 \
        IROP_TYPE(<, TYPE) { return lhs < (float)rhs; }                              \
        IROP_TYPE(>, TYPE) { return rhs < lhs; }                                     \
        IROP_TYPE(<=, TYPE) { return !(lhs > rhs); }                                 \
        IROP_TYPE(>=, TYPE) { return !(lhs < rhs); }

        ROP(unsigned char)
        ROP(char)
        ROP(unsigned short)
        ROP(short)
        ROP(unsigned int)
        ROP(int)
        ROP(unsigned long)
        ROP(long)
        ROP(unsigned long long)
        ROP(long long)
        ROP(float)
        ROP(double)
        ROP(bool)
        ROP(long double)
        ROP_TYPE(<, Half) { return (float)lhs < (float)rhs; }
        ROP_TYPE(>, Half) { return rhs < lhs; }
        ROP_TYPE(<=, Half) { return !(lhs > rhs); }
        ROP_TYPE(>=, Half) { return !(lhs < rhs); }
        ROP_TYPE(==, Half) { return lhs.bits == rhs.bits; }
        ROP_TYPE(!=, Half) { return !(lhs == rhs); }

#undef ROP
    }
}


namespace std
{
    using namespace is::nbla;
#define MATHF(FUNC)                                                                 \
    Half FUNC(const Half& h) { return Half(std::FUNC((float)h)); }

    MATHF(exp)
    MATHF(log)
    MATHF(log2)
    MATHF(log1p)
    MATHF(sqrt)
    MATHF(sin)
    MATHF(cos)
    MATHF(tan)
    MATHF(sinh)
    MATHF(cosh)
    MATHF(tanh)
    MATHF(asin)
    MATHF(acos)
    MATHF(atan)
    MATHF(asinh)
    MATHF(acosh)
    MATHF(atanh)
    MATHF(fabs)
    MATHF(abs)
    MATHF(round)
    MATHF(isnan)
    MATHF(isinf)

    Half pow(const Half& a, const Half& b) { return std::pow((float)a, (float)b); }
    Half pow(const Half& a, const int& b) { return std::pow((float)a, (float)b); }
    Half max(const Half& a, const int& b) { return std::max((float)a, (float)b); }

    Half atan2(const Half& a, const Half& b) 
    {
        return std::atan2((float)a, (float)b);
    }

    Half ldexp(const Half& a, const int& b) { return std::ldexp((float)a, b); }

    MATHF(floor)
    MATHF(ceil)

#undef MATHF
}