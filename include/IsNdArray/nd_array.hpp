#pragma once

#include <IsNdArray/synced_array.hpp>

#include <memory>

namespace is
{
    namespace nbla
    {
        using std::make_shared;

        /* Dtype and backend agnostic multi-dimensional array. */
        class NdArray
        {
            SyncedArrayPtr array_;
            Shape_t shape_;
            Stride_t strides_;
            Size_t size_;
            Size_t ndim_;

            /**
             * @brief 配列形状の情報を更新.
             * 
             */
            void update_shape_info();

        public:
            using Ptr = shared_ptr<NdArray>;


            /**
             * @brief NdArrayのshared_ptrインスタンスを生成.
             * 
             * @tparam Args 
             * @param args 
             * @return Ptr 
             */
            template <typename... Args>
            static Ptr create(Args... args)
            {
                return make_shared<NdArray>(args...);
            }


            /**
             * @brief 形状を与える.
             * サイズに応じたSyncedArrayインスタンスが生成される.
             * 
             * @param shape Shape of creating array.
             */
            NBLA_API NdArray(const Shape_t& shape = {});


            /**
             * @brief 前回作成したSyncedArrayインスタンスを与える.
             * 
             * @param array Previously created SyncedArray.
             * @param shape Shape of N-d array. Total size must be the same as
             */
            NBLA_API NdArray(SyncedArrayPtr array, const Shape_t& shape = {});


            /**
             * @brief 形状を変更する.
             * 
             * @param shape N-d array will be reshaped to this shape.
             * @param force If total size doesn't match and true is given, array will
             * reset to total size, which means the content of array will become totally
             * different one.
             */
            NBLA_API void reshape(const Shape_t& shape, bool force = false);


            /**
             * @brief 内部の配列情報は共有する別インスタンスを生成する.
             * 
             * @return NBLA_API 
             */
            NBLA_API Ptr view();


            /**
             * @brief 内部の配列情報は共有するが、異なる形状の他のインスタンスを生成する.
             * 
             * @param shape shape N-d array reshaped to.
             */
            NBLA_API Ptr view(const Shape_t& shape);


            /**
             * @brief 形状を取得.
             * 
             */
            NBLA_API Shape_t shape() const;


            /**
             * @brief ストライドを取得.
             * 
             */
            NBLA_API Stride_t strides() const;


            /**
             * @brief  多次元配列のトータルサイズ(各次元を掛け合わせたもの)を取得する.
             * 
             * @param axis Size followed by given axis is computed.
             */
            NBLA_API Size_t size(Size_t axis = -1) const;


            /**
             * @brief  次元数を取得.
             * 
             */
            NBLA_API Size_t ndim() const;


            /**
             * @brief 型情報を取得.
             * 
             * @return NBLA_API 
             */
            NBLA_API inline dtypes dtype() const 
            {
                return array_->dtype();
            }

            /**
             * @brief 現在のNdArrayインスタンスが保持しているSyncedArrayインスタンスを取得する.
             * 
             * @note This is not copying data. Modifying content affects this.
             */
            NBLA_API SyncedArrayPtr array();

            /**
             * @brief 以前生成したSyncedArrayインスタンスと中身を入れ替える.
             * 
             * @param array 
             */
            NBLA_API void set_array(SyncedArrayPtr array);


            /**
             * @brief 多次元配列の要素をゼロで埋める.
             * @note This will be lazily evaluated when data is used.
             */
            NBLA_API void zero();


            /**
             * @brief 指定値で多次元配列の要素を埋める.
             *  @note This will be lazily evaluated when data is used.
             * 
             * @param v 
             */
            NBLA_API void fill(float v);


            /**
             * @brief 型情報とバックエンドを指定したconstなArray系インスタンスのポインタを取得する.
             * 
             * @param dtype 
             * @param ctx 
             * @return const Array* 
             */
            NBLA_API const Array* get(dtypes dtype, const Context& ctx);


            /**
             * @brief  型情報とバックエンドを指定したconstなArray系インスタンスのshared_ptrを取得する.
             * 
             * @param dtype 
             * @param ctx 
             * @return shared_ptr<const Array> 
             */
            NBLA_API shared_ptr<const Array> get_sp(dtypes dtype, const Context& ctx);


            /**
             * @brief Array系インスタンスが持つ内部メモリ配列の先頭ポインタを取得.
             * 
             * @param dtype Enum of data type.
             * @param ctx Descriptor of array backend.
             * @param wirte_only write_only No synchronization happens.
             * @return unsigned long 
             */
            NBLA_API unsigned long data_ptr(dtypes dtype, const Context& ctx, bool wirte_only = false);


            /**
             * @brief  型情報とバックエンドを指定した書き換え可能なArray系インスタンスのポインタを取得する.
             * 
             * @param dtype Enum of data type.
             * @param ctx Descriptor of array backend.
             * @param write_only No synchronization happens.
             * @return Array* 
             */
            NBLA_API Array* cast(dtypes dtype, const Context& ctx, bool write_only = false);


            /**
             * @brief 型情報とバックエンドを指定した書き換え可能なArray系インスタンスのshared_ptrを取得する.
             * 
             * @param dtype Enum of data type.
             * @param ctx Descriptor of array backend.
             * @param write_only No synchronization happens.
             * @return shared_ptr<Array> 
             */
            NBLA_API shared_ptr<Array> cast_sp(dtypes dtype, const Context& ctx, bool write_only = false);


            template <typename T>
            T* cast_data_and_get_pointer(const Context& ctx, bool write_only = false)
            {
                Array* arr = this->cast(get_dtype<T>(), ctx, write_only);
                return arr->pointer<T>();
            }

            template <typename T>
            const T* get_data_pointer(const Context& ctx)
            {
                const Array* arr = this->get(get_dtype<T>(), ctx);
                return arr->const_pointer<T>();
            }


            DISABLE_COPY_AND_ASSIGN(NdArray)
        }; // end NdArray

        // エイリアス
        using NdArrayPtr = NdArray::Ptr;
    }
}