/**
 * @file singleton_manager-internal.hpp
 * @author your name (you@domain.com)
 * @brief SingletonManagerの別実装
 * @version 0.1
 * @date 2021-08-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <IsCommon/singleton_manager.hpp>

#include <typeinfo>
#include <iostream>
#include <string>

#if defined(_MSC_VER)
    #if !defined(_DEBUG) || defined(NDEBUG)
        #define IS_CHECK_SINGLETON(new_or_delete, singleton)
    #else
        #define IS_CHECK_SINGLETON(new_or_delete, singleton)                            \
            std::cout << #new_or_delete << " a singleton `" << typeid(singleton).name() \
                    << "`" << std::endl
#endif

#else
    // 以下の条件が正しいか疑問 Shinichi Inoue 21/2/5
    #if !defined(NDEBUG)
        #define IS_CHECK_SINGLETON(new_or_delete, singleton)
    #else
        // UNIX系だとlibstdc++に含まれる.
        // Windowsはvcruntime14X.dllかな...
        #if __has_include(<cxxabi.h>)
            #include <cxxabi.h>
            #define IS_CHECK_SINGLETON(new_or_delete, singleton)                          \
                std::string singleton_name;                                               \
                const std::type_info &type_id = typeid(singleton); /* RTTI */             \
                int stat{-1};                                                             \
                char *name = abi::__cxa_demangle(type_id.name(), 0, 0, &stat);            \
                if (name != nullptr)                                                      \
                {                                                                         \
                    if (stat == 0) /* success: stat == 0 */                               \
                    {                                                                     \
                        singleton_name = name;                                            \
                        std::cout << #new_or_delete << " a singleton `" << singleton_name \
                                  << "`" << std::endl;                                    \
                        ::free(name);                                                     \
                    }                                                                     \
                }                                                                         \
                if (stat != 0)                                                            \
                {                                                                         \
                    std::cout << #new_or_delete << " a singleton `"                       \
                              << typeid(SINGLETON).name() << "`" << std::endl;            \
                }
        #else
            #define IS_CHECK_SINGLETON(new_or_delete, singleton)      \
                std::cout << #new_or_delete << " a singleton `"       \
                    << typeid(singleton).name() << "`" << std::endl
        #endif
    #endif
#endif


namespace is {
    namespace common {

        template <typename SINGLETON>
        SINGLETON *SingletonManager::get() {
            // 内部リンケージのstatic変数は必ずdll側の*.cppで定義すること.
            // https://qiita.com/Chironian/items/3fb61cffa2a20dbee5c2
            std::mutex &mtx_ = SingletonManager::get_mtx();

            // クリティカルセクション作成
            std::lock_guard<std::mutex> locker(mtx_);

            static SINGLETON *instance{nullptr}; // 初回だけnullptrで初期化される
            if (instance) {
                return instance;
            }

            SingletonManager &self_ = SingletonManager::get_self();
            IS_CHECK_SINGLETON(Create, SINGLETON);
            instance = new SINGLETON{};

            auto deleter = [&]() -> void {
                IS_CHECK_SINGLETON(Delete, SINGLETON);
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
        int SingletonManager::get_id() {
            SingletonManager &self_ = SingletonManager::get_self();
            auto address = (uintptr_t)(get<SINGLETON>());
            return self_.address2id_[address];
        }

        template <typename SINGLETON>
        void SingletonManager::erase() {
            erase_by_id(get_id<SINGLETON>());
        }

// SingletonManagerクラスのメンバ関数テンプレートの明示的インスタンス化用のマクロ
// dllライブラリのエクスポートに対応するため
// https://qiita.com/Chironian/items/462a3bdf271d5f0b00b6
#define IS_INSTANTIATE_SINGLETON(api, singleton_class)                      \
    template api singleton_class *SingletonManager::get<singleton_class>(); \
    template api int SingletonManager::get_id<singleton_class>();           \
    template api void SingletonManager::erase<singleton_class>();
    }
}