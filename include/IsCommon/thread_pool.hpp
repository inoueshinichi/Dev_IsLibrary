/**
 * @file thread_pool.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-09-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

#include <IsCommon/IsCommon.hpp>

#include <condition_variable>
#include <exception>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

namespace is {
    namespace common {
        using std::bind;
        using std::condition_variable;
        using std::forward;
        using std::future;
        using std::make_shared;
        using std::move;
        using std::mutex;
        using std::packaged_task;
        using std::queue;
        using std::runtime_error;
        using std::thread;
        using std::unique_lock;
        using std::vector;


        class IS_COMMON_API ThreadPool {
        public:
            ThreadPool(int32 workers);
            virtual ~ThreadPool();

            template <typename Func, typename... Args>
            future<IS_INVOKE_RESULT_TYPE(Func, Args)> enqueue(Func&& func, Args&&... args) {  

                // キューにタスクを入れて、タスクの戻り値の型を取得する
                using return_type = IS_INVOKE_RESULT_TYPE(Func, Args);
                auto task = make_shared<packaged_task<return_type()>>(bind(forward<Func>(func), forward<Args>(args)...));
                future<return_type> result = task->get_future();

                // 排他領域
                {
                    unique_lock<mutex> locker_(mtx_);

                    // ストップリクエストがONの場合、エンキューを停止＆例外出力
                    if (stop_request_) {
                        throw runtime_error("Enqueue on stopped thread-pool.");
                    }

                    queue_.emplace([task]() {
                        (*task)();
                    }); // wrapped_task
                }

                // ワーカースレッドのどれかに通知
                condition_.notify_one();

                return result;
            }

        private:
            vector<thread> workers_;              // スレッドプールに存在するワーカースレッド
            queue<packaged_task<void()>> queue_;  // タスクのキュー
            mutex mtx_;                           // キューのミューテックス
            condition_variable condition_;        // キューの条件変数
            bool stop_request_{false};            // スレッドプールへのストップリクエスト

            IS_DISABLE_COPY_AND_ASSIGN(ThreadPool)
        };
    }
}