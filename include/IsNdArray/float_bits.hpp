#pragma once

#include <cstdint>
#include <cstdio>
#include <type_traits>

namespace is
{
    namespace nbla
    {
        // 前方宣言
        struct Half;

        // プライマリーテンプレート
        template <typename T>
        struct nbits {};

        // 完全特殊化
        template <>
        struct nbits<Half>
        {
            static constexpr int n = 16; // 16bit
            static constexpr int nexp = 5; // 2の5乗
            static constexpr int nsig = 10;
            using bit_t = uint16_t;
        };

        template <>
        struct nbits<float>
        {
            static constexpr int n = 32; // 32bit
            static constexpr int nexp = 8; // 2の8乗
            static constexpr int nsig = 23;
            using bit_t = uint32_t;
        };

        template <>
        struct nbits<double>
        {
            static constexpr int n = 64; // 64bit
            static constexpr int nexp = 11; // 2の11乗
            static constexpr int nsig = 52;
            using bit_t = uint64_t;
        };



        /**
         * @brief float_bitsクラス
         * 
         */
        template <typename T>
        struct float_bits : public nbits<T>
        {
            using bit_t = typename nbits<T>::bit_t;
            using sbit_t = typename std::make_signed<typename nbits<T>::bit_t>::type;
            
            // 半浮動小数点数の定義
            static constexpr bit_t sign_mask = (bit_t)1 << (nbits<T>::n - 1);
            static constexpr bit_t exp_mask = ((bit_t)1 << nbits<T>::nexp) - 1;
            static constexpr bit_t inf_bits = exp_mask << nbits<T>::nsig;
            static constexpr bit_t sig_mask = ((bit_t)1 << nbits<T>::nsig) - 1;
            static constexpr sbit_t exp_bias = ((bit_t)1 << (nbits<T>::nexp - 1)) - 1;
            static constexpr sbit_t exp_max = exp_bias;
            static constexpr sbit_t exp_min = -exp_bias + 1;
            static constexpr sbit_t implied_bit = sig_mask + 1;

            template <typename U>
            static constexpr bit_t diff_sig()
            {
                static_assert(nbits<T>::n > nbits<U>::n, "T must be larger than U.");
                return nbits<T>::nsig - nbits<U>::nsig;
            }

            // 仮数
            static bit_t get_significand_bits(bit_t bits)
            {
                return bits & sig_mask;
            }

            // 符号付き指数
            static sbit_t get_signed_exponent(bit_t bits)
            {
                return (sbit_t)((bits >> nbits<T>::nsig) & exp_mask) - exp_bias;
            }


            // 符号ビットを取得
            template <typename U>
            static typename nbits<U>::bit_t get_sign_bit_of(bit_t bits)
            {
                static_assert(nbits<T>::n > nbits<U>::n, "T must be larger than U.");
                return (bits >> (nbits<T>::n - nbits<U>::n)) & float_bits<U>::sign_mask;
            }


             // 符号ビットを取得
             template <typename U>
             static bit_t get_sign_bit_from(typename nbits<U>::bit_t bits)
             {
                 static_assert(nbits<T>::n > nbits<U>::n, "T must be larger than U.");
                 return ((bit_t)(bits) << (nbits<T>::n - nbits<U>::n)) & float_bits<T>::sign_mask;
             }


             template <typename U>
             static typename float_bits<U>::bit_t signed_exp_to_bits(sbit_t signed_exp)
             {
                 return (signed_exp + float_bits<U>::exp_bias) << nbits<U>::nsig;
             }


             template <typename U>
             static void round_ties_to_even(bit_t& sig_bits)
             {
                 // Maks the last bit of U and the following bits.
                 constexpr bit_t mask = (1 << (diff_sig<U>() + 1)) - 1;
                
                // Tied value (goes to even.)
                constexpr bit_t tie = 1 << (diff_sig<U>() - 1);

                // tie 010...0 goes to 000...0, another tie 110...0 goes to (+1)000...0
                if ((sig_bits & mask) != tie)
                {
                    sig_bits += tie;
                }
             }


              /** Downconvert T-type bits into U-type bits
                 @code{.cpp}
                union {
                    float fval;
                    uint32_t fbits;
                };
                float_value = 1;
                uint16_t half_bits = float_bits<float>::downconvert_to<Half>(float_bits);
                @endcode
            */
           template <typename U>
           static typename nbits<U>::bit_t downconvert_to(bit_t bits)
           {
               static_assert(nbits<T>::n > nbits<U>::n,
                             "Downconverting to bigger type is prohibited.");

                using tbit_t = typename nbits<U>::bit_t;

                // 1. Parse the float bits to sign, exponent and significand.
                tbit_t tsign_bit = get_sign_bit_of<U>(bits);    // 符号
                sbit_t signed_exp = get_signed_exponent(bits);  // 指数
                bit_t sig_bits = get_significand_bits(bits);    // 仮数

                // 2. T value is Nan / inf.
                if (signed_exp == exp_max + 1)
                {
                    if (sig_bits == 0)
                    {
                        // inf
                        return tsign_bit | float_bits<U>::inf_bits;
                    }
                    else
                    {
                        // Nan: Propagate signaling Nan.
                        tbit_t tsig_bits = sig_bits >> diff_sig<U>();
                        if (tsig_bits == 0)
                        {
                            tsig_bits += 1; // avoid inf
                        }
                        return tsign_bit | float_bits<U>::inf_bits | tsig_bits;
                    }
                }

                // 3. If exponent overflows in target type, convert to signed inf.
                if (signed_exp > float_bits<U>::exp_max)
                {
                    return tsign_bit | float_bits<U>::inf_bits;
                }

                // 4. If exponent is lower than subnormal of U, return signed 0.
                if (signed_exp < (float_bits<U>::exp_min - nbits<U>::nsig))
                {
                    return tsign_bit;
                }

                // 5. If exponent is in a range of exponent of U, convert to normal.
                if (signed_exp >= float_bits<U>::exp_min)
                {
                    round_ties_to_even<U>(sig_bits);
                    // Note: Rounding may cause a bit spilling into exponent bits.
                    //      `+` operator is used, which will produce correct results.
                    return tsign_bit | (signed_exp_to_bits<T>(signed_exp) + (tbit_t)(sig_bits >> diff_sig<U>()));
                }

                // 6. Rest is subnormal.
                // 6-A. Add implied bit since T is normal.
                sig_bits |= implied_bit;
                // 6-B. Shift significand by exponent while canceling the effect of minimum exponent of target type.
                int rshift = -signed_exp + float_bits<U>::exp_min;
                sig_bits >>= rshift;
                // 6-C. Round ties-to-even.
                round_ties_to_even<U>(sig_bits);
                return tsign_bit | (tbit_t)(sig_bits >> diff_sig<U>());
           }


            /** Upconvert T-type bits into U-type bits.
             */
            template <typename U>
            static U upconvert_to(bit_t bits)
            {
                static_assert(nbits<U>::n > nbits<T>::n, "U must be larger than T.");

                constexpr auto dsig = float_bits<U>::template diff_sig<T>();
                using tbit_t = typename float_bits<U>::bit_t;

                // 0. Parse bits.
                tbit_t tsign_bit = float_bits<U>::template get_sign_bit_from<T>(bits);
                sbit_t signed_exp = get_signed_exponent(bits);
                bit_t sig_bits = get_significand_bits(bits);

                // 1. Nan or inf.
                if (signed_exp == exp_max + 1)
                {
                    // Just shifting significand.
                    return tsign_bit | float_bits<U>::inf_bits | ((tbit_t)(sig_bits) << dsig);
                }

                // 2. 0 or subnormal
                if (signed_exp == exp_min - 1)
                {
                    // 1-A. 0
                    if (sig_bits == 0)
                    {
                        return tsign_bit;
                    }

                    // 1-B. Subnormal
                    // Iterate left-shift until getting normalized expression
                    // `1.(significand)`.
                    sig_bits <<= 1;
                    // texp_bits starts from -exp_bias decrements until get normalized expression.
                    tbit_t texp_bits = float_bits<U>::exp_bias - exp_bias;
                    while ((sig_bits & implied_bit) == 0)
                    {
                        sig_bits <<= 1;
                        texp_bits--;
                    }
                    texp_bits <<= float_bits<U>::nsig;
                    tbit_t tsig_bits = ((tbit_t)(get_significand_bits(sig_bits))) << dsig;
                    return tsign_bit | texp_bits | tsig_bits;
                }

                // 3. Normalized.
                return tsign_bit | ((tbit_t)(signed_exp + float_bits<U>::exp_bias) << float_bits<U>::nsig) | ((tbit_t)(sig_bits) << dsig);
            }
        };
    }
}