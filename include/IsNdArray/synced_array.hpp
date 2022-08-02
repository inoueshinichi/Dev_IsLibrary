#pragma once

#include <IsNdArray/array.hpp>
#include <IsNdArray/common.hpp>
#include <IsNdArray/context.hpp>

#include <map>
#include <memory>

namespace is
{
    namespace nbla
    {
        using std::map;
		using std::shared_ptr;
		using std::pair;

        /*!
		 *  暗黙的にデバイスとデータ型の間でキャストとデータの転送を行うインターフェース配列
		 */
        class NBLA_API SyncedArray : public std::enable_shared_from_this<SyncedArray>
        {
            struct ArrayDesc
            {
                string key;
                string array_class;
                dtypes dtype;
            };

            ArrayDesc head_;    // 内部で生成されるArray系インスタンスを決める内容 : 転送される
            bool zeroing_;      // zero()関数の遅延評価のためのフラグ
            bool filling_;      // fill()関数の遅延評価のためのフラグ
            double fill_value_; // fill()関数の遅延評価で使用される指定値
            Size_t size_;       // 多次元配列のメモリサイズ

            // Context, Arrayタイプに応じて格納するバッファ
            // この中身のshared_ptr<Array>がCpuArray, CpuCachedArray, CudaArrayなど
            map<string, pair<shared_ptr<Array>, bool>> array_; // bool値がtrueのものがArrayDescに登録されている

            // Modification accounts for calling either cast, zero or fill.
            std::size_t modification_count_;

        public:
            SyncedArray(const Size_t size);
            ~SyncedArray();

           /**
            * @brief Cast and get array with dtype context 
            * 
            * This will return an array object with specified dtype and device with implicit
            * synchronization over different dtypes/devices.
            * 
            * @param write_only When true, just returns an Array instance requested without synchronization.
            * @param async_flags AsyncFlag::NONE    -> Synchronous synchronization
            *                    AsyncFlag::ASYNC   -> Asynchronous synchronization
            *                    AsyncFlag::UNSAFE  -> No synchronization to host
            *                    AsyncFlag::ASYNC | AsyncFlag::UNSAFE -> 
            *                                          The memory region of the source array of an
            *                                          asynchronous data transfer is not guranteed to be
            *                                          kept safe until the end of the transfer.
            */

            /**
             * @brief Cast and get array with dtype context
             * 
             * This will return an array object with specified dtype and device with implicit
             * synchronization over different dtypes/devices.
             * 
             * @param dtype 
             * @param ctx 
             * @param write_only When true, just returns an Array instance requested without synchronization.
             * @param async_flags AsyncFlag::NONE  -> Synchronous synchronization
                                  AsyncFlag::ASYNC -> Asynchronous synchronization
                                  AsyncFlag::UNSAFE  -> No synchronization to host
                                  AsyncFlag::ASYNC | AsyncFlag::UNSAFE ->
                                  The memory region of the source array of an
                                  asynchronous data transfer is not guranteed to be
                                  kept safe until the end of the transfer.
             * @return Array* 
             */
            Array* cast(dtypes dtype, const Context &ctx, bool write_only = false,
                        const int async_flags = AsyncFlag::NONE);


            /**
             * @brief Cast and get array with dtype context
             * 
             * @param dtype 
             * @param ctx 
             * @param write_only 
             * @param async_flags 
             * @return shared_ptr<Array> 
             */
            shared_ptr<Array> cast_sp(dtypes dtype, const Context& ctx, 
                                      bool write_only = false,
                                      const int async_flags = AsyncFlag::NONE);
            

            /**
             * @brief Get array with dtype context.
             * This will return an array object with specified dtype and device with implicit
             * synchronization. Note that this function call does not move the head in array list.
             * 
             * TODO: is "const" member function appropriate? This implicitly creates or
             *       modify array contents of specified dtype-context.
             * 
             * @param dtype 
             * @param ctx 
             * @param async_flags 
             * @return const Array* 
             */
            const Array* get(dtypes dtype, const Context& ctx,
                             const int async_flags = AsyncFlag::NONE);


            /**
             * @brief Get array with dtype context.
             * 
             * @param dtype 
             * @param ctx 
             * @param async_flags 
             * @return shared_ptr<const Array> 
             */
            shared_ptr<const Array> get_sp(dtypes dtype, const Context& ctx,
                                           const int async_flags = AsyncFlag::NONE);
            

            /**
             * @brief Get the head array.
             * 
             * @return Array* 
             */
            Array* head_array();


            /**
             * @brief  Get the head array as a shared pointer.
             * 
             * @return shared_ptr<Array> 
             */
            shared_ptr<Array> head_array_sp();


            /**
             * @brief  Get array's ptr.
             * 
             * @param dtype Enum of data type.
             * @param ctx Descriptor of array backend.
             * @param write_only No synchronization happens.
             * @param async_flags 
             *          AsyncFlag::NONE  -> Synchronous synchronization happens.
             *          AsyncFlag::ASYNC -> Asynchronous synchronization happens.
             *          AsyncFlag::SAFE  -> Same as AsyncFlag::NONE.
             *          AsyncFlag::ASYNC | AsyncFlag::SAFE -> Asynchronous synchronization
             *          happens and the synchronized source array
             *          keeps safe against the host operation.
             * @return const void* 
             */
            const void* data_ptr(dtypes dtype, const Context& ctx,
                                 bool write_only = false,
                                 const int async_flags = AsyncFlag::NONE);

            
            /**
             * @brief Get dtype
             * 
             * @return dtypes 
             */
            inline dtypes dtype() const
			{
				NBLA_CHECK(!head_.key.empty(), error_code::unclassified, "Array is not initialized.");
				return head_.dtype;
			}


            /**
             * @brief Get the array class of the head.
             * 
             * @return std::string 
             */
            inline std::string head_array_class()
			{
				return head_.array_class;
			}


            /**
             * @brief Get the number of arrays
             * 
             * @return Size_t 
             */
            inline Size_t get_num_arrays() const
            {
                return array_.size();
            }


            /**
             * @brief Get size.
             * 
             * @return Size_t 
             */
            inline Size_t size() const
			{
				return size_;
			}


            /**
             * @brief Fill all element with 0.
             * @note This is lazily evaluated at calling of get() or cast().
             */
            void zero();


            /**
             * @brief Fill all element with given double value.
             * @note This is lazily evaluated at calling of get() or cast().
             * 
             * @param value 
             */
            void fill(double value);


            /**
             * @brief Get number of modification count.
             * @note Modification accounts for calling either cast, zero or fill.
             * @return std::size_t 
             */
            std::size_t modification_count() const;


            /**
             * @brief Copy values from another SynedArray.
             * @note The copy is happening in a device and a dtype of source array.
             * 
             * @param src 
             */
            void copy_from(const SyncedArray* src);


            /**
             * @brief clear
             * 
             */
            void clear();


            /**
             * @brief  Get whether or not it fills array values obtained in cast/get call later.
             * @note This is provided to determine gradient accumulation flags in our
             *       computation graph engine, as well as to determine whether or not solver and
             *       communicator execute their operations by depending on whether gradients are
             *       updated.
             * @return true 
             * @return false 
             */
            bool zeroing() const;


        private:
            /**
             * @brief 同期(多次元配列の中身を型情報を変更)
             * @note 内部で新たに同じ要素数だけメモリを確保して、キャストしながらコピーする.
             *       CPU<->GPU間でメモリがあるデバイスが異なる場合でも適切にキャスト&コピーできる.
             * 
             * @param dtype 
             * @param ctx 
             * @param write_only 
             * @param async_flags 
             * @return ArrayDesc 
             */
            ArrayDesc sync(dtypes dtype, const Context& ctx_orig, bool write_only = false,
                           const int async_flags = AsyncFlag::NONE);


            /**
             * @brief 
             * 
             */
            void clear_all_array();


            /**
             * @brief Clearing zero and fill flags for lazy evaluation.
             * 
             */
            void clear_flags();


            DISABLE_COPY_AND_ASSIGN(SyncedArray)
        };


        // エイリアス
        using SyncedArrayPtr = shared_ptr<SyncedArray>;

        // 前方宣言 for friend
        class SingletonManager;

        // Get, Cast or Clear
        enum SyncedArrayCallbackTag
        {
            GET = 0,
            CAST,
            CLEAR
        };

        // Type of callback function for get, cast, and clear of SyncedArray.
        using synced_array_callback_func_type = std::function<void(
            SyncedArrayPtr saptr, const SyncedArrayCallbackTag func_name,
            const dtypes dtype, const Context& ctx, const bool write_only,
            const bool first_creation, const bool off_recording)>;
        
        /**
        Singleton class to store a callback function for get, cast, and clear of
        SyncedArray.
        */
       class NBLA_API SyncedArrayCallback
       {
           synced_array_callback_func_type callback_func_;

        public:
            ~SyncedArrayCallback();

            /**
             * @brief Check if callback function is not set.
             * 
             * @return true 
             * @return false 
             */
            bool empty();


            /**
             * @brief Set the callback func object
             * 
             * @param f 
             */
            void set_callback_func(synced_array_callback_func_type f);


            /**
             * @brief 
             * 
             * @param saptr 
             * @param func_name 
             * @param dtype 
             * @param ctx 
             * @param write_only 
             * @param first_creation 
             * @param off_recording 
             */
            void call_callback(SyncedArrayPtr saptr,
                               const SyncedArrayCallbackTag func_name,
                               const dtypes dtype,
                               const Context& ctx,
                               const bool write_only,
                               const bool first_creation,
                               const bool off_recording);
            
        private:
            // needs forward declaration
            // Never called by users
            friend SingletonManager;

            SyncedArrayCallback();

            DISABLE_COPY_AND_ASSIGN(SyncedArrayCallback)
       };
    }
}