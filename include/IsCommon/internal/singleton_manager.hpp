/**
 * @file singleton_manager.hpp
 * @author your name (you@domain.com)
 * @brief シングルトン管理クラス
 * @version 0.1
 * @date 2021-08-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <IsCommon/internal/defs.hpp>

#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace is {
    namespace common {
        using std::pair;
        using std::unique_ptr;
        using std::unordered_map;

        // mozc式シングルトン
        // https://qiita.com/kikuuuty/items/fcf5f7df2f0493c437dc
        class IS_COMMON_API SingletonManager {
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
            template <typename SINGLETON>
            static SINGLETON *get();

            /**
             * @brief シングルトンクラスを識別するIDを取得
             * 
             * @tparam SINGLETON 
             * @return int32
             */
            template <typename SINGLETON>
            static int get_id();

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
            template <typename SINGLETON>
            static void erase();

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

            /**
             * @brief シングルトンインスタンスへのポインタ
             * https://qiita.com/Chironian/items/3fb61cffa2a20dbee5c2
             */
            static SingletonManager *self_; // Never be destroyed.

            /**
             * @brief このシングルトンインスタンスを取得
             * @warning シングルトンインスタンスの生成をスレッドセーフにするために
             *          必ずstd::mutexを定義しているSingletonManager::get()内部から
             *          呼び出すように制限をかけるからprivateスコープにする(get_rmtx関数を定義すれば大丈夫かも...)
             * @return SingletonManager& 
             */
            static SingletonManager &get_self();

            /**
             * @brief 再帰ロック可能なミューテックスを取得
             * @note 追加 Shinichi Inoue 21/2/5
             * @return std::recursive_mutex& 
             */
            static std::mutex &get_mtx();

            // コンストラクタ/デストラクタを非公開
            SingletonManager();
            ~SingletonManager();

            // コピー系オペレータをprivateに変更
            IS_DISABLE_COPY_AND_ASSIGN(SingletonManager)
        };
    }
}
