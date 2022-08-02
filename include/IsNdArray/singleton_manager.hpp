#pragma once

#include <IsNdArray/IsNdArray.hpp>
#include <IsNdArray/array.hpp>
#include <IsNdArray/context.hpp>
#include <IsNdArray/synced_array.hpp>

#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace is
{
    namespace nbla
    {
        using std::unique_ptr;
		using std::unordered_map;
		using std::pair;

        // mozc式シングルトン
		// https://qiita.com/kikuuuty/items/fcf5f7df2f0493c437dc
        class NBLA_API SingletonManager
        {
        public:
            /**
             * @brief SINGLETON型のシングルトンクラスを作成
             * SINGLETON型は必ず以下のルールを遵守すること.
             * 1) コンストラクタはprivate
             * 2) デストラクタはpublic
             * 3) SingletonManagerをfriendに登録
             * 
             * @tparam SINGLETON 
             * @return SINGLETON* 
             */
            template <typename SINGLETON> static SINGLETON* get();


            /**
             * @brief シングルトンクラスを識別するIDを取得
             * 
             * @tparam SINGLETON 
             * @return int 
             */
            template <typename SINGLETON> int get_id();


            /**
             * @brief 登録済みのすべてのシングルトンインスタンスを削除
             * 
             */
            static void clear();


            /**
             * @brief SINGLETON型の登録済みシングルトンインスタンスを削除
             * 
             * @tparam SINGLETON 
             */
            template <typename SINGLETON> void erase();


            /**
             * @brief IDによる登録済みシングルトンインスタンスの削除
             * 
             * @param id 
             */
            static void erase_by_id(int id);

        private:
            int count_{0}; // 登録されたシングルトンインスタンスの数

            // IDから[シングルトンinstance, (addressとdeleter関数)]へのHashマップ
            unordered_map<int, pair<uintptr_t, std::function<void()>>> singletons_;

            // addressからIDへのHashマップ
            unordered_map<uintptr_t, int> address2id_;

            // シングルトンインスタンスへのポインタ
            // https://qiita.com/Chironian/items/3fb61cffa2a20dbee5c2
            static SingletonManager* self_; // Never be destroyed.

            // このシングルトンクラスを取得
            // 注意) シングルトンインスタンスの生成をスレッドセーフにするために
            //       必ずstd::mutexを定義しているSingletonManager::get()内部から
            //       呼び出す制限をかけるからprivateスコープにする(get_rmtx関数を定義すれば大丈夫かも...)
			static SingletonManager& get_self();

            // 再帰ロック可能なミューテックスを取得
            // 追加 Shinichi Inoue 21/2/5
            static std::recursive_mutex& get_rmtx();

            // コンストラクタ/デストラクタを非公開
            SingletonManager();
			~SingletonManager();

            // コピー系オペレータを禁止
            DISABLE_COPY_AND_ASSIGN(SingletonManager)
        };
    }
}