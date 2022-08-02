
#include <IsCommon/thread_pool.hpp>

namespace is {
    namespace common {

        ThreadPool::ThreadPool(int workers) {
            
            for (int i = 0; i < workers; ++i) {

                // スレッド内で処理されるコンテキスト
                auto done_on_thread = [this]() {

                    while (true) {
                        // キューから受け取るタスク
                        packaged_task<void()> wrapped_task;

                        // 排他領域
                        {
                            unique_lock<mutex> locker(mtx_);

                            // キューにタスクが入るまで待機
                            condition_.wait(locker, [this]() {
                                return stop_request_ || !queue_.empty();  // ストップリクエストON or キューにタスクが残っている場合、trueを出力
                            });

                            // ストップリクエストON & キューが空の場合、ThreadPoolを終了する
                            if (stop_request_ && queue_.empty())
                                return;

                            // タスクの受け取り
                            wrapped_task = move(queue_.front());

                            // 中身の無い要素を削除
                            queue_.pop();

                            // タスク実行
                            wrapped_task();
                        }
                    }
                };

                // スレッドにタスクを移譲
                workers_.emplace_back(done_on_thread);
            }
        }

        ThreadPool::~ThreadPool() {
            // 排他制御
            {
                unique_lock<mutex> locker(mtx_);
                stop_request_ = true;
            }

            condition_.notify_all();
            for (auto& worker : workers_) {
                worker.join();
            }
        }
    }
}