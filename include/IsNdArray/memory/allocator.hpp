#pragma once

#include <IsNdArray/common.hpp>
#include <IsNdArray/memory/allocator_callback.hpp>
#include <IsNdArray/memory/memory.hpp>

#include <cstddef>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>


namespace is
{
    namespace nbla
    {
        using std::unordered_map;
        using std::unique_ptr;
        using std::shared_ptr;
        using std::make_shared;

        // 前方宣言
        class Allocator;

        /**
         * Allocatorクラスによって確保されたMemoryのためのRAIIクラス.
         * デストラクタで，借りていたMemoryが，コンストラクタで登録したAllocatorインスタンス
         * に戻される.
         */
        class AllocatorMemory
        {
            shared_ptr<Memory> memory_;

            /**
             * memory_を確保するために使われるAllocatorは, shared_ptrとして保持する必要があります.
             * そうしないと, memory_を破棄する前にallocator_が破棄され，破棄されたAllocatorにアクセス
             * する可能性がある。
             */
            shared_ptr<Allocator> allocator_;

            void release();

        public:
            /**
             * @brief Construct a new Allocator Memory object
             * 
             * これはAllocator::alloc()からMemoryインスタンスにラップするために呼ばれる.
             * 
             * @param memory 
             * @param allocator 
             */
            NBLA_API AllocatorMemory(shared_ptr<Memory> memory, shared_ptr<Allocator> allocator);

            /**
             * @brief Construct a new Allocator Memory object
             * 
             * 空のインスタンスを作る.
             * 
             */
            NBLA_API AllocatorMemory();

            /**
             * @brief Destroy the Allocator Memory object
             * 
             * コンストラクタでallocatorに与えられたMemoryインスタンスを戻す.
             * 
             */
            NBLA_API ~AllocatorMemory();

            /**
             * @brief Construct a new Allocator Memory object
             * 
             * ムーブコンストラクタ
             * 
             * @param rhs 
             */
            NBLA_API AllocatorMemory(AllocatorMemory&& rhs);

            /**
             * @brief ムーブ代入演算子
             * 
             * @param rhs 
             * @return AllocatorMemory& 
             */
            NBLA_API AllocatorMemory& operator=(AllocatorMemory&& rhs);

            /**
             * @brief このAllocatorMemoryインスタンスが所有するMemoryを取得する.
             * 
             * @return shared_ptr<Memory> 
             */
            inline shared_ptr<Memory> memory() { return memory_; }

            /**
             * @brief Memory::bytesを取得
             * 
             * @return size_t 
             */
            inline size_t bytes() const { return memory_-> bytes(); }

            /**
             * @brief Memory::device_idを取得
             * 
             * @return string 
             */
            inline string device_id() const { return memory_-> device_id(); }

            /**
             * @brief Memory 実装クラスで定義されたメモリブロックの生ポインタを取得.
             * 
             * @return void* 
             */
            inline void* pointer() { return memory_-> pointer(); }

            /**
             * @brief Memory 実装クラスで定義されたメモリブロックの読み取り専用生ポインタを取得.
             * 
             * @return const void* 
             */
            inline const void* const_pointer() const { return memory_->const_pointer(); }

            /* コピー構築とコピー代入を禁止する */
            AllocatorMemory(const AllocatorMemory&) = delete;
            AllocatorMemory& operator=(const AllocatorMemory&) = delete;
        };

        /**
         * @brief Allocator インターフェースクラス.
         * 
         * 派生クラスはキャッシュしたデバイスメモリブロックを管理するロジックを実装する.
         * テンプレート引数としてMemoryクラスを受け取る派生クラスNativeAllocatorは，
         * キャッシングなしでメモリアロケータを実装する.
         * Allocatorインスタンスはshared_ptrとしてインスタンス化しなければならない.
         */

        class NBLA_API Allocator : public std::enable_shared_from_this<Allocator>
        {
        protected:
            unique_ptr<AllocatorCallback> callback_;
            unordered_map<string, size_t> device_memory_used_in_bytes_;

            std::mutex mtx_;

        public:
            using MemCountMap = unordered_map<string, int>;

            std::function<void(void)> callback_tmp_ = nullptr;

            // コンストラクタは何もしない
            Allocator();
            virtual ~Allocator();

            /**
             * @brief AllocatorMemoryでラップされたメモリブロックを要求する.
             * 
             * @param bytes Number of bytes of memory block requested.
             * @param device_id Device specifier string. A format of allow specifier
             *                  is determined depending on Memory class
             *                  implementation.
             * @return AllocatorMemory 
             * 
             * コンパイラRVO(Return Value Optimization)は、戻り値のコピーを防ぐために有効になっている
             * と想定されています.
             * 返されたAllocatorMemoryは、std::moveを使用してArrayインスタンスに移動する必要があります.
             */
            AllocatorMemory alloc(size_t bytes, const string& device_id);

            /**
             * @brief AllocatorMemoryのデストラクタで呼ばれる.
             * 
             * 確保されたメモリブロックはこのAllocatorインスタンスに返却される.
             * 派生クラスはAllocator::free_impl関数を実装しなければならない.
             * free_impl関数 : キャッシングアロケータ -> メモリブロックをプールに返却.
             *                 ネイティブアロケータ   -> freeする.
             * 
             * @param memory 
             */
            void free(shared_ptr<Memory> memory);

            /**
             * @brief すべてのデバイスから使用していないメモリブロックを解放する.
             * 
             * Allocato:free_unused_device_cachesは、以前にAllocator::alloc関数に渡された
             * すべてのデバイスIDに対して呼び出されます.
             * 
             * @return size_t 
             */
            size_t free_unused_caches();

            /**
             * @brief 特定デバイスの使用していないメモリブロックを解放する.
             * 
             * 派生クラスはAllocator::free_unused_device_caches関数を実装しなければならない.
             * Allocator::free_unused_device_caches関数は, 元々生成された未使用のメモリブロック
             * 例えば、分割していないプールから捨てられたメモリブロックなど.
             * 
             * @param device_id 
             * @return size_t 
             */
            size_t free_unused_device_caches(const string& device_id);

            /**
             * @brief 特定デバイスの現在確保しているメモリブロックをバイト単位で取得.
             * 
             * @param device_id 
             * @return size_t 
             */
            size_t device_memory_used_in_bytes(const string& device_id);


            /** APIs for memory cache analysis
             */

            /**
             * @brief 
             * 
             */
            void print_memory_cache_map() { print_memory_cache_map_impl(); }

            /**
             * @brief Get the fragmentation bytes object
             * 
             * @param device_id 
             * @return size_t 
             */
            virtual size_t get_fragmentation_bytes(const string &device_id) { return 0; }
            
            /**
             * @brief Get the max available bytes object
             * 
             * @param device_id 
             * @return size_t 
             */
            virtual size_t get_max_available_bytes(const string &device_id) { return 0; }

            /**
             * @brief Get the used memory counts object
             * 
             * @param device_id 
             * @return vector<int> 
             */
            virtual vector<int> get_used_memory_counts(const string &device_id) {
                return {};
            }

        protected:
            /**
             * @brief 再確保有りでMemory::alloc関数を呼ぶ.
             * 
             * もし初回のMemory::alloc関数が失敗したら, Allocator::free_unused_device_caches関数を実行する.
             * その後, Memory::alloc関数を再度呼ぶ.
             * 
             * この関数は, 派生クラスのAllocator::alloc_impl関数で使われるように設計されている.
             * 
             * @param memory 
             */
            void alloc_retry(shared_ptr<Memory> memory);

            /**
             * @brief メモリブロックを要求する.
             * 
             * 派生クラスでのalloc_impl関数に要求される機能
             * - 新しいMemoryインスタンスを生成
             * - Allocator::alloc_retry関数によって生成されたMemoryインスタンスのメモリ確保を呼ぶ.
             * - 確保されたMemoryを戻り値として返す.
             * 
             * @param bytes 
             * @param device_id 
             * @return shared_ptr<Memory> 
             */
            virtual shared_ptr<Memory> alloc_impl(size_t orig_bytes, const string& device_id) = 0;

            /**
             * @brief プールにメモリを返却する.
             * 
             * 派生クラスでは, 与えられたZMemoryをプールに返す(caching-allocator) or freeする.
             * 
             * @param memory 
             */
            virtual void free_impl(shared_ptr<Memory> memory) = 0;

            /**
             * @brief 特定デバイスの使用していないメモリブロックを解放する.
             * 
             * 派生クラスは、使用されていないか、最初に割り当てられたメモリブロックから分割された
             * メモリインスタンスをプールから破棄する必要がある.
             * 
             * @param device_id 
             * @return size_ 
             */
            virtual size_t free_unused_device_caches_impl(const string& device_id) = 0;


            /**
             * @brief print_memory_cache_map_impl
             * 
             */
            virtual void print_memory_cache_map_impl() {};

            /* コピー構築とコピー代入を禁止する */
            DISABLE_COPY_AND_ASSIGN(Allocator)
        };
    }
}