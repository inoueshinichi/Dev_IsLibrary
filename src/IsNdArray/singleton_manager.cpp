#include <IsNdArray/singleton_manager-internal.hpp>

namespace is
{
    namespace nbla
    {
        SingletonManager* SingletonManager::self_ = nullptr;

        // 注意) privateスコープ
        std::recursive_mutex& SingletonManager::get_rmtx()
        {
            // 内部リンケージ
            static std::recursive_mutex rmtx_;
            return rmtx_;
        }

        // 注意) privateスコープ
        // コンストラクタ/デストラクタ
        SingletonManager::SingletonManager() {}
        SingletonManager::~SingletonManager() {}


        // 注意) privateスコープ
        SingletonManager& SingletonManager::get_self()
        {
            std::recursive_mutex& rmtx_ = SingletonManager::get_rmtx();
            std::lock_guard<std::recursive_mutex> locker(rmtx_);

            if (!self_)
            {
                self_ = new SingletonManager{};
            }
            return *self_;
        }


        void SingletonManager::clear()
        {
            SingletonManager& self_ = SingletonManager::get_self();
            for (int i = 0; i < self_.count_; ++i)
            {
                // もしアクティブならSingletonインスタンスを破棄
                erase_by_id(i); 
            }
            // clear all
            self_.singletons_.clear();
            self_.address2id_.clear();
            self_.count_ = 0;
        }


        void SingletonManager::erase_by_id(int id)
        {
            SingletonManager& self_ = SingletonManager::get_self();
            auto iter = self_.singletons_.find(id);
            if (iter == self_.singletons_.end())
            {
                return;
            }
            
            iter->second.second(); // Invoke deleter
            self_.address2id_.erase(iter->second.first);
            self_.singletons_.erase(iter);
        }
        
    }
}