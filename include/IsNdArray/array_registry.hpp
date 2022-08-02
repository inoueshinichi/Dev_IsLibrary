#pragma once

#include <IsNdArray/array.hpp>
#include <IsNdArray/synced_array.hpp>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>

namespace is
{
    namespace nbla
    {
        using std::shared_ptr;
		using std::string;
		using std::map;
		using std::pair;

        // copy無しでget/castできるArray系クラスグループ
        class NBLA_API ArrayGroup
        {
        public:
            // Array系クラスをグループ名にマッピング
            using Registry_t = map<string, string>; 

            /**
             * @brief 新しいArray系クラスを登録.
             * 
             * @param array_class 
             * @param group_name 
             */
            static void add_group(const string& array_class, const string& group_name);

            
            /**
             * @brief Array系クラスのグループ名を取得.
             * 
             * @param array_class 
             * @return string 
             */
            static string get_group(const string& array_class);


        private:
            // Never be created
			inline ArrayGroup();


            /**
             * @brief Array系インスタンスの生成関数の登録情報を取得.
             * 
             * @return Registry_t& 
             */
            static Registry_t& get_registry();
        };


        class NBLA_API ArrayCreator
        {
        public:
            using Creator = std::function<Array* (const Size_t, dtypes, const Context&)>;
            using FilterContext = std::function<Context (const Context&)>;
            using Registry_t = map<string, pair<Creator, FilterContext>>;

            /**
             * @brief Array系インスタンスを生成するインターフェース.
             * 
             * @param size 
             * @param dtype 
             * @param ctx 
             * @return Array* 
             */
            static Array* create(const Size_t size, dtypes dtype, const Context& ctx);


            /**
             * @brief コンテキストを記述する最小の情報にフィルタリングしたコンテキストを返す.
             * 
             * @param ctx 
             * @return Context 
             */
            static Context filter_context(const Context& ctx);


            /**
             * @brief 新しいArray系インスタンスを登録する.
             * 
             * @param name 
             * @param creator 
             * @param filter_context 
             */
            static void add_creator(const string& name, Creator creator, FilterContext filter_context);


        private:
            // Never be created
			inline ArrayCreator() {}


            /**
             * @brief Array系クラスの生成関数の登録情報を取得する.
             * 
             * TODO: これが、privateスコープで正しいのか検証すること.
             * 
             * @return Registry_t& 
             */
            static Registry_t& get_registry();
        };


        class NBLA_API ArraySynchronizer
        {
        public:
            using Synchronizer = std::function<void(Array*, Array*, const int)>;
            using Registry_t = map<pair<string, string>, Synchronizer>;


            /**
             * @brief Array系インスタンス同士の同期(型情報を変更してコピーする)
             * 
             * @param src_class 
             * @param src_array 
             * @param dst_class 
             * @param dst_array 
             * @param async_flags 
             */
            static void synchronize(const string& src_class, Array* src_array,
                                    const string& dst_class, Array* dst_array,
                                    const int async_flags = AsyncFlag::NONE);

            /**
             * @brief 新規の同期情報を登録.
             * 
             * @param src_class 
             * @param dst_class 
             * @param synchronizer 
             */
            static void add_synchronizer(const string& src_class, const string& dst_class,
                                         Synchronizer synchronizer);

        private:
			// Never be created
			inline ArraySynchronizer() {}

            /**
             * @brief Array系クラスのコピー関数の登録情報を取得する.
             * 
             * TODO: これが、privateスコープで正しいのか検証すること.
             * 
             * @return Registry_t& 
             */
            static Registry_t& get_registry();
        };


        /**
         * @brief Synchronizer(同期)は、Array::copy_from()関数に依存する.
         * 
         *  This should be used as a synchronizer between classes that are using the
         *  same device class like CpuArray-CpuCachedArray.
         * 
         *  async_flags are not used in synchronizer_default.
         */
        NBLA_API void synchronizer_default(Array* src, Array* dst,
                                           const int async_flags = AsyncFlag::NONE);
        

        /**
         * @brief ヘルパーマクロの定義
         * 
         */

// Array系クラスをグループに登録
#define NBLA_REGISTER_ARRAY_GROUP(CLASS, GROUP)                                  \
	{ ArrayGroup::add_group(#CLASS, #GROUP); }

// Array系インスタンスを生成する関数を登録
#define NBLA_REGISTER_ARRAY_CREATOR(CLS)                                         \
	{                                                                            \
		std::function<Array*(const Size_t, dtypes, const Context&)> func =       \
			[](const Size_t size, dtypes dtype, const Context& ctx) {            \
				return new CLS(size, dtype, ctx);                                \
			};                                                                   \
		ArrayCreator::add_creator(#CLS, func, CLS::filter_context);              \
	}

#define NBLA_REGISTER_ARRAY_SYNCHRONIZER(SRC_CLASS, DST_CLASS, SYNCHRONIZER)     \
	{ ArraySynchronizer::add_synchronizer(#SRC_CLASS, #DST_CLASS, SYNCHRONIZER); }

    }
}
