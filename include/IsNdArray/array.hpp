#pragma once

#include <IsNdArray/common.hpp>
#include <IsNdArray/dtypes.hpp>
#include <IsNdArray/event.hpp>
#include <IsNdArray/exception.hpp>
#include <IsNdArray/half.hpp>
#include <IsNdArray/memory/allocator.hpp>

#include <memory>
#include <type_traits>

namespace is
{
    namespace nbla
    {
        /**
		* CpuArrayやCudaArrayのような実装クラスのための配列インターフェイスクラス.
		* Arrayクラス達はユーザーに直接触らせない
		*/

        // 配列インターフェースの基底クラス
		// このクラスはCpuArrayやCudaArrayなどの実装クラスによって拡張される.
        class Array : public std::enable_shared_from_this<Array>
        {
        protected:
            Size_t size_;         // 配列のサイズ
            dtypes dtype_;        // float32, int32, uint8のような型
            Context ctx_;         // 何のデバイス配列が使用されているかを識別するための情報を保持する
            AllocatorMemory mem_; // Memoryインスタンスを保持する
            EventPtr event_;      // 非同期でのメモリコピーを待つためのEventインスタンスを保持する


            /**
             * @brief Construct a new Array object
             * コンストラクタは派生クラスのみから呼ばれなければならない.
             * AllocatorMemoryはnbla::Allocatorの実現インスタンスによって作成され，右辺値参照として渡されるはず.
             * 
             * @param size 
             * @param dtype 
             * @param ctx 
             * @param mem 
             */
            NBLA_API Array(const Size_t size, dtypes dtype, const Context& ctx, AllocatorMemory&& mem);


            /**
             * @brief 多次元配列のサイズから使用している総バイト数を計算する.
             * 
             * @param size 
             * @param dtype 
             * @return std::size_t 
             */
            static NBLA_API std::size_t size_as_bytes(Size_t size, dtypes dtype);


            /**
             * @brief Memoryインスタンスが保持する先頭の生ポインタを取得する.
             * このメソッドはArray::pointer()をオーバーライドするために使われる.
             * 
             * @return void* 
             */
            virtual NBLA_API void* mem_pointer() { return mem_.pointer(); }


            /**
             * @brief Memoryインスタンスが保持するconstな先頭の生ポインタを取得する.
             * このメソッドはArray::conset_pointer()をオーバーライドするために使われる.
             * 
             * @return const void* 
             */
            virtual NBLA_API const void* mem_const_pointer() const { return mem_.const_pointer(); }


        public:
            using Ptr = shared_ptr<Array>;


            virtual NBLA_API ~Array() = 0;


            /**
             * @brief Array系インスタンスが持つ先頭の生ポインタを取得する.
             * 
             * @tparam T 
             * @return T* 
             */
            template <typename T = void>
            T* pointer()
            {
                return reinterpret_cast<T*>(mem_pointer());
            }

            
            /**
             * @brief Array系インスタンスが持つconstな先頭の生ポインタを取得する.
             * 
             * @tparam T 
             * @return const T* 
             */
            template <typename T = void>
            const T* const_pointer() const
            {
                return reinterpret_cast<const T*>(mem_const_pointer());
            }

            
            /**
             * @brief dtypesを取得する.
             * 
             * @return dtypes 
             */
            inline dtypes dtype() const { return dtype_; }


            /**
             * @brief 多次元配列のサイズを取得する.
             * 
             * 例えば，
			 * [[[1,2,3]
			 *   [4,5,6]
			 *   [7,8,9]]
			 *  [[9,8,7]
			 *   [6,5,4]
			 *   [3,2,1]]]
			 * Size_t = (2,3,3)の多次元配列(tensor)
             * 
             * @return Size_t 
             */
            inline Size_t size() const { return size_; }


            /**
             * @brief Contextを取得する.
             * 
             * @return Context 
             */
            inline Context context() const { return ctx_; }


            /**
             * @brief 他のArray系インスタンスから中身をコピーする.
             * 
             * @param src_array 
             */
            virtual void copy_from(const Array* src_array) = 0;


            /**
             * @brief zero(0)ですべての配列要素を埋める.
             * 
             * @return void 
             */
            virtual NBLA_API void zero() = 0;


            /**
             * @brief 与えた数値ですべての配列要素を埋める.
             * 
             * @param value 
             */
            virtual NBLA_API void fill(double value) = 0;


            /**
             * @brief Array系インスタンスを記述する最小情報でフィルタしたContextを取得する.
             * 
             * @param ctx 
             * @return Context 
             */
            static Context filter_context(const Context& ctx);


            /**
             * @brief Set the event object
             * 
             * @param eptr 
             */
            virtual NBLA_API void set_event(EventPtr eptr);


            /**
             * @brief イベントの終了を待つ.
             * 
             * 
             * @param ctx イベントの終了を待っているContext.
             * @param async_flags 
             * @return void 
             */
            virtual NBLA_API void wait_event(const Context ctx, const int async_flags = AsyncFlag::NONE);


            /**
             * @brief イベントを持っているかチェックする.
             * 
             * @return bool 
             */
            virtual NBLA_API bool have_event();


            /**
             * @brief Array系インスタンスのshared_ptrを取得する.
             * 
             * @return Ptr 
             */
            virtual NBLA_API Ptr getptr();

        protected:
            DISABLE_COPY_AND_ASSIGN(Array)
        };

        // エイリアス
        using ArrayPtr = Array::Ptr;
        using ConstArray = const Array;
        using ConstArrayPtr = shared_ptr<ConstArray>;

        /**
         * @brief 以下，マクロによるヘルパー関数群.
         * 
         * [copy()関数]
         * NBLA_DEFINE_FUNC_COPY_FROMマクロ
         *  ↑
         * NBLA_CASE_ARRAY_COPY_FROMマクロ
         *  ↑
         * NBLA_ARRAY_COPY_FROMマクロ
         *  ↑
         * NBLA_CASE_ARRAY_COPY_FROM_TOマクロ
         * 
         * 
         * [fill(float v)関数]
         * NBLA_DEFILE_FUNC_FILLマクロ
         *  ↑
         * NBLA_CASE_ARRAY_FILLマクロ
         */
//-------------------------------------------------------------------------------------------
//////////////////////////////////////////
// 別のdtypes型にコピー関数の実装マクロを定義
//////////////////////////////////////////
#define NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, type_enum, src_type, dst_type, name)        \
    case dtypes::type_enum:                                                                 \
        copy_func##_wrapper<src_type, dst_type>::copy(src_array, this);                     \
        break;

#define NBLA_ARRAY_COPY_FROM(copy_func, type, name)                                         \
	switch (this->dtype())                                                                  \
	{                                                                                       \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, UBYTE, type, unsigned char, name);          \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, BYTE, type, char, name);                    \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, USHORT, type, unsigned short, name);        \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, SHORT, type, short, name);                  \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, UINT, type, unsigned int, name);            \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, INT, type, int, name);                      \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, ULONG, type, unsigned long, name);          \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, LONG, type, long, name);                    \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, ULONGLONG, type, unsigned long long, name); \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, LONGLONG, type, long long, name);           \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, FLOAT, type, float, name);                  \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, DOUBLE, type, double, name);                \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, BOOL, type, bool, name);                    \
		NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, LONGDOUBLE, type, long double, name);       \
	    NBLA_CASE_ARRAY_COPY_FROM_TO(copy_func, HALF, type, nbla::Half, name);              \
		default:                                                                            \
			NBLA_ERROR(error_code::unclassified, "Disabled dtype %s.",                      \
						dtype_to_string(this->dtype()).c_str());                            \
	}
// ------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////
// 別のdtypes型を持つ配列でコピーを作成する関数をマクロで定義
////////////////////////////////////////////////////////
#define NBLA_CASE_ARRAY_COPY_FROM(copy_func, type_enum, src_type, name)                     \
	case dtypes::type_enum:                                                                 \
		NBLA_ARRAY_COPY_FROM(copy_func, src_type, name);                                    \
		break;

#define NBLA_DEFINE_FUNC_COPY_FROM(array_class, copy_func, name)                            \
	void array_class::copy_from(const Array* src_array)                                     \
    {                                                                                       \
		if (src_array->size() != this->size_)                                               \
        {                                                                                   \
			NBLA_ERROR(error_code::unclassified, "Size mismatch.");                         \
		}                                                                                   \
		switch (src_array->dtype())                                                         \
        {                                                                                   \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, UBYTE, unsigned char, name);               \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, BYTE, char, name);                         \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, USHORT, unsigned short, name);             \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, SHORT, short, name);                       \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, UINT, unsigned int, name);                 \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, INT, int, name);                           \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, ULONG, unsigned long, name);               \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, LONG, long, name);                         \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, ULONGLONG, unsigned long long, name);      \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, LONGLONG, long long, name);                \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, FLOAT, float, name);                       \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, DOUBLE, double, name);                     \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, BOOL, bool, name);                         \
			NBLA_CASE_ARRAY_COPY_FROM(copy_func, LONGDOUBLE, long double, name);            \
	        NBLA_CASE_ARRAY_COPY_FROM(copy_func, HALF, nbla::Half, name);                   \
			default:                                                                        \
				NBLA_ERROR(error_code::unclassified, "Disabled dtype %s.",                  \
							dtype_to_string(src_array->dtype()).c_str());                   \
		}                                                                                   \
	}
// ------------------------------------------------------------------------------------------

//////////////////////////////////
// 配列要素を指定値で埋める関数を定義
//////////////////////////////////
#define NBLA_CASE_ARRAY_FILL(fill_func, type_enum, type, name)                              \
	case dtypes::type_enum:                                                                 \
		fill_func<type>(this, value);                                                       \
		break;

#define NBLA_DEFINE_FUNC_FILL(array_class, fill_func, name)                                 \
	void array_class::fill(double value)                                                    \
    {                                                                                       \
		switch (this->dtype())                                                              \
        {                                                                                   \
			NBLA_CASE_ARRAY_FILL(fill_func, UBYTE, unsigned char, name);                    \
			NBLA_CASE_ARRAY_FILL(fill_func, BYTE, char, name);                              \
			NBLA_CASE_ARRAY_FILL(fill_func, USHORT, unsigned short, name);                  \
			NBLA_CASE_ARRAY_FILL(fill_func, SHORT, short, name);                            \
			NBLA_CASE_ARRAY_FILL(fill_func, UINT, unsigned int, name);                      \
			NBLA_CASE_ARRAY_FILL(fill_func, INT, int, name);                                \
			NBLA_CASE_ARRAY_FILL(fill_func, ULONG, unsigned long, name);                    \
			NBLA_CASE_ARRAY_FILL(fill_func, LONG, long, name);                              \
			NBLA_CASE_ARRAY_FILL(fill_func, ULONGLONG, unsigned long long, name);           \
			NBLA_CASE_ARRAY_FILL(fill_func, LONGLONG, long long, name);                     \
			NBLA_CASE_ARRAY_FILL(fill_func, FLOAT, float, name);                            \
			NBLA_CASE_ARRAY_FILL(fill_func, DOUBLE, double, name);                          \
			NBLA_CASE_ARRAY_FILL(fill_func, BOOL, bool, name);                              \
			NBLA_CASE_ARRAY_FILL(fill_func, LONGDOUBLE, long double, name);                 \
		  NBLA_CASE_ARRAY_FILL(fill_func, HALF, nbla::Half, name);                          \
			default:                                                                        \
				NBLA_ERROR(error_code::unclassified, "Disabled dtype %s.",                  \
							dtype_to_string(this->dtype()).c_str());                        \
		}                                                                                   \
	} 
    }// namespace: nbla
    // ------------------------------------------------------------------------------------------

    /**
     * @brief 以下、関数オブジェクト用ヘルパーマクロ
     * 
     * NBLA_DEFINE_COPY_WRAPPER(copy_func)
     *      copy_func_wrapperクラスとcopy_func_is_disableクラスの
     *      プライマリーテンプレートを作成するマクロ.
     * 
     * NBLA_DISABLE_TYPE(copy_func, fill_func, TYPE)
     *      コピーできないdtypes型または、未定義のdtypes型の場合、実装エラーにするようなメタ関数
     *      を定義.
     * 
     */

//////////////////////////////////////////////////
// コピー用の2種類のテンプレートクラスを定義するマクロ
//////////////////////////////////////////////////
// 以下２つはプライマリーテンプレート
#define NBLA_DEFINE_COPY_WRAPPER(copy_func)                                                 \
	template <typename Ta, typename Tb, typename Enabled = void>                            \
	struct copy_func##_wrapper                                                              \
    {                                                                                       \
		static void copy(const Array* src, Array* dst)                                      \
        {                                                                                   \
			copy_func<Ta, Tb>(src, dst);                                                    \
		}                                                                                   \
	};                                                                                      \
                                                                                            \
	template <typename T>                                                                   \
    struct copy_func##_is_disabled                                                          \
    {                                                                                       \
		static constexpr bool value = false;                                                \
    };
// ------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////
// SFINAEによる特殊化でコピー可能or不可能を示すテンプレートクラスを定義するマクロ
///////////////////////////////////////////////////////////////////////////
// copy_func##_wrapperテンプレートクラスをSFINAEで中身の実装を切り替える.
// copy_func##_is_disabledテンプレートクラスは補助クラス.
#define NBLA_DISABLE_TYPE(copy_func, fill_func, TYPE)                                       \
	template <typename Ta, typename Tb>                                                     \
	struct copy_func##_wrapper<Ta, Tb,                                                      \
                typename std::enable_if<std::is_same<Ta, TYPE>::value>::type>               \
    {                                                                                       \
		static void copy(const Array* src, Array* dst)                                      \
        {                                                                                   \
			NBLA_ERROR(error_code::not_implemented,                                         \
						"`" #TYPE "` is disabled in `" #copy_func "`.");                    \
		}                                                                                   \
	};                                                                                      \
                                                                                            \
	template <>                                                                             \
    struct copy_func##_is_disabled<TYPE>                                                    \
    {                                                                                       \
		static constexpr bool value = true;                                                 \
	};                                                                                      \
                                                                                            \
	template <typename Ta, typename Tb>                                                     \
	struct copy_func##_wrapper<Ta, Tb,                                                      \
                typename std::enable_if<!copy_func##_is_disabled<Ta>::value &&              \
										std::is_same<Tb, TYPE>::value>::type>               \
    {                                                                                       \
		static void copy(const Array* src, Array* dst)                                      \
        {                                                                                   \
			NBLA_ERROR(error_code::not_implemented,                                         \
						"`" #TYPE "` is disabled in `" #copy_func "`.");                    \
		}                                                                                   \
	};                                                                                      \
                                                                                            \
	template <>                                                                             \
    void fill_func<TYPE>(Array* self, float value)                                          \
    {                                                                                       \
		NBLA_ERROR(error_code::not_implemented,                                             \
					"`" #TYPE "` is disabled in `" #fill_func "`.");                        \
	}

} // namespace: is