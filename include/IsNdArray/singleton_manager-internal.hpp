#pragma once

#include <IsNdArray/singleton_manager.hpp>

#include <typeinfo>
#include <iostream>
#include <string>


#if defined(_MSC_VER)
    #if !defined(_DEBUG) || defined(NDEBUG)
        #define NBLA_SINGLETON_LOG(MESSAGE, SINGLETON)
    #else
        #define NBLA_SINGLETON_LOG(MESSAGE, SINGLETON)                                       \
            std::cout << "[" << #MESSAGE << "] a singleton `" << typeid(SINGLETON).name()    \
                        << "`" << std::endl;
    #endif
#else
    // 以下の条件が正しいか疑問 Shinichi Inoue 21/2/5
    #if defined(NDEBUG)
        #define NBLA_SINGLETON_LOG(MESSAGE, SINGLETON)
    #else
        // UNIX系だとlibstdc++に含まれる.
        // Windowsは未調査.
        // Shinichi Inoue 21/2/5
        #if __has_include(<cxxabi.h>)
            #include <cxxabi.h>
            #define NBLA_SINGLETON_LOG(MESSAGE, SINGLETON)                                 \
                std::string singleton_name;                                                \
                const std::type_info& type_id = typeid(SINGLETON); /* RTTI */              \
                int stat{-1};                                                              \
                char* name = abi::__cxa_demangle(type_id.name(), 0, 0, &stat);             \
                if (name != nullptr)                                                       \
                {                                                                          \
                    if (stat == 0) /* success: stat == 0 */                                \
                    {                                                                      \
                        singleton_name = name;                                             \
                        std::cout << "[" << #MESSAGE << "] a singleton `" << singleton_name\
                                << "`" << std::endl;                                       \
                        ::free(name);                                                      \
                    }                                                                      \
                }                                                                          \
                if (stat != 0)                                                             \
                {                                                                          \
                    std::cout << "[" << #MESSAGE << "] a singleton `"                      \
                              << typeid(SINGLETON).name() << "`" << std::endl;             \
                }
        #else
            #define NBLA_SINGLETON_LOG(MESSAGE, SINGLETON)                                 \
                std::cout << "[" << #MESSAGE << "] a singleton `"                          \
                        << typeid(SINGLETON).name() << "`" << std::endl;
        #endif
    #endif
#endif

namespace is
{
    namespace nbla
    {
        template <typename SINGLETON>
        SINGLETON* SingletonManager::get()
        {
            // 内部リンケージのstatic変数は必ずdll側の*.cppで定義すること.
            // https://qiita.com/Chironian/items/3fb61cffa2a20dbee5c2
            std::mutex& mtx_ = SingletonManager::get_mtx();

            // クリティカルセクション作成
            std::lock_guard<std::recursive_mutex> locker(mtx_);

            static SINGLETON* instance {nullptr}; // 初回だけnullptrで初期化される
            if (instance)
            {
                return instance;
            }

            SingletonManager& self_ = SingletonManager::get_self();
            instance = new SINGLETON{};
            NBLA_SINGLETON_LOG(Create, SINGLETON)

            auto deleter = [&]() -> void {
                NBLA_SINGLETON_LOG(Delete, SINGLETON)
                delete instance;
                instance = nullptr;
            };

            int id = self_.count_;
            self_.singletons_.insert({id, {(uintptr_t)instance, deleter}}); // Register deleter
            self_.address2id_.insert({(uintptr_t)instance, id});            // Register ID
            self_.count_ += 1;
            return instance;
        }


        template <typename SINGLETON>
        int SingletonManager::get_id()
        {
            SingletonManager& self_ = SingletonManager::get_self();
            auto address = (uintptr_t)(get<SINGLETON>());
            return self_.address2id_[address];
        }


        template <typename SINGLETON>
        void SingletonManager::erase()
        {
            erase_by_id(get_id<SINGLETON>());
        }

        
        // SingletonManagerクラスのメンバ関数テンプレートの明示的インスタンス化用のマクロ
        // dllライブラリのエクスポートに対応するため
        // https://qiita.com/Chironian/items/462a3bdf271d5f0b00b6
        #define NBLA_INSTANTIATE_SINGLETON(API, SINGLETON_CLASS)                     \
            template API SINGLETON_CLASS *SingletonManager::get<SINGLETON_CLASS>();  \
            template API int SingletonManager::get_id<SINGLETON_CLASS>();            \
            template API void SingletonManager::erase<SINGLETON_CLASS>();
    }
}