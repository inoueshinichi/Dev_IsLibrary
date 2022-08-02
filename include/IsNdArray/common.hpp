#pragma once

#include <IsNdArray/context.hpp>
#include <IsNdArray/exception.hpp>
#include <IsNdArray/init.hpp>

#include <algorithm>
#include <cstdint>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace is
{
    namespace nbla
    {
        using std::vector;
        using std::stringstream;
        using std::shared_ptr;

        using Shape_t = vector<int64_t>;
        using Stride_t = vector<int>;
        using Size_t = int64_t;

        // Flags for SyncedArray and data transfer
        enum AsyncFlag 
        { 
            NONE = 0b0,
            ASYNC = 0b1,
            UNSAFE = 0b10,
            OFFREC = 0b100
        };

        /**
         * @brief 形状によって配列サイズを計算する.
         * 
         * @param shape 
         * @param axis 
         * @return Size_t 
         */
        inline Size_t compute_size_by_shape(const Shape_t& shape, Size_t axis = 0)
        {
            axis = std::max(static_cast<Size_t>(0), axis);

            NBLA_CHECK((size_t)axis <= shape.size(), error_code::value,
                    "axis must be less than or equal to size of shape."
                    "axis: %ld > size of shape: %ld.",
                    axis, shape.size());

            Size_t ret = 1;
            for (size_t i = axis; i < shape.size(); ++i)
            {
                ret *= shape[i];
            }
            return ret;
        }

        /**
         * @brief Get the c contigous strides object
         * 
         * C型の連続メモリ配置のストライド数を取得するヘルパー関数
         * 
         * @param shape 
         * @return Stride_t 
         */
        inline Stride_t get_c_contiguous_strides(const Shape_t& shape)
        {
            if (!shape.size())
            {
                return Stride_t();// 0形状配列のストライドとして空を渡す
            }

            Stride_t strides(shape.size(), 1); // shape(1,2,3) -> size=3 -> vector{1,1,1}
            for (int64_t i = strides.size() - 2; i >=0; --i)
            {
                // shape(N,) -> strides.size() = 1
                // shape(1,2,3) -> strides(6,3,1) strides[2] = 1, strides[1] = 3*1, strides[0] = 2*3,   strides[i] = strides[i+1] * shape[i+1]
                strides[i] *= strides[i + 1] * shape[i + 1];
            }
            return strides;
        }


        inline string byte_to_human_readable(long double byte)
        {
            vector<string> units = {"B", "KB", "MB", "GB"};

            bool neg = byte < 0;
            if (neg)
                byte = -byte;
            
            string unit;
            double div = 1 << 10; // 1024で割る
            for (auto& u : units)
            {
                unit = u;
                if (byte < div)
                    break;
                byte /= div;
            }

            std::ostringstream oss;
            oss.precision(2);
            oss << std::fixed << byte;

            return (neg ? "-" : "") + oss.str() + unit;
        }

        /**
         * @brief デリミッター付きのstring型のJoin.
         * 
         *  pythonの"-".join([a, b, c]) = "a-b-c"と同じ
         * 
         * @tparam T 
         * @param vec 
         * @param delimiter 
         * @return string 
         */
        template <typename T>
        inline string string_join(const vector<T>& vec, const string& delimiter)
        {
            stringstream ss;
            if (vec.empty())
            {
                return "";
            }

            for (std::size_t i = 0; i < vec.size() - 1; ++i)
            {
                ss << vec[i] << delimiter;
            }
            ss << vec[vec.size() - 1];
            return ss.str();
        }

        /**
         * @brief スコープ付きコールバック
         * 
         * ※ 例外発生時に関数スタックの巻き戻しが起こるので，
         * 必ず処理させたいものは，このDestractorCallbackの関数オブジェクトに入れるのだろうと思う.
         * e.g リソースの解放など
         * 
         * 注意) 実装をヘッダーに書いているので, NBLA_APIでdllエクスポートしない.
         */
        class DestructorCallback
        {
            std::function<void(void)> callback_;
        public:
            inline DestructorCallback(std::function<void(void)> callback)
                : callback_(callback)
            {}

            inline ~DestructorCallback()
            {
                callback_();
            }
        };

        /**
         * @brief  ハッシュ値の組み合わせ関数
         * 
         * 参考：http://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
         * 
         * @tparam T 
         * @param seed 
         * @param v 
         */
        template <typename T>
        void hash_combine(size_t& seed, T const& v)
        {
            seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

        /**
         * @brief shared_ptr<>の生ポインタを取得
         * 
         * @tparam T 
         * @param vector 
         * @return vector<T*> 
         */
        template <typename T>
        vector<T*> as_pointer_array(const vector<shared_ptr<T>>& vec)
        {
            vector<T*> ret(vec.size());
            for (int i = 0; i < vec.size(); ++i)
            {
                ret[i] = vec[i].get();
            }
            return ret;
        }

        /**
         * @brief Get the from variadic args object
         * 
         * 可変個数引数テンプレートのゲッター(可変個数引数タプルの任意のインデックスの値を取得する)
         * 
         * @tparam index 
         * @tparam Args 
         * @param args 
         * @return std::remove_reference<
         * decltype(std::get<index>(std::make_tuple(args...)))>::type 
         */
        template <int index, typename... Args>
        auto get_from_variadic_args(Args&&... args) -> typename std::remove_reference<
            decltype(std::get<index>(std::make_tuple(args...)))>::type
        {
            auto&& t = std::forward_as_tuple(args...);
            return std::get<index>(t);
        }

    // コピーコンストラクタとコピー代入演算子を禁止する
    #define DISABLE_COPY_AND_ASSIGN(class_name)            \
    private:                                               \
        class_name(const class_name&);                     \
        class_name& operator=(const class_name&); 
        
    } // namespace nbla
}