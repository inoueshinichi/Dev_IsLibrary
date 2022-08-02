#pragma once

#include <IsNdArray/IsNdArray.hpp>

#include <cstddef>
#include <memory>
#include <string>

namespace is
{
    namespace nbla
    {
        using std::string;
        using std::size_t;
        using std::shared_ptr;

        // /**
        //  * @brief デバイスのメモリ状態
        //  * 
        //  */
        // enum DeviceMemoryState
        // {
        //     Locked = 0,   // 将来, デバイス上で使われるメモリ状態. クリアできない.
        //     Unlocked = 1, // デバイス上でもう使用されないメモリ状態. クリア可能.
        // };


        // /**
        //  * @brief 物理的メモリのインターフェースクラス
        //  * 
        //  */
        // class PhysicalMemory
        // {
        // protected:
        //     bool allocated_;
        //     size_t bytes_;
        //     string device_id_;

        // public:
        //     explicit PhysicalMemory(size_t bytes, const string& device_id)
        //         : allocated_(false)
        //         , bytes_(bytes)
        //         , device_id_(device_id) {};
            
        //     ~PhysicalMemory() = default;

        //     /**
        //      * @brief 少なくともbytes_より多く物理的メモリを確保する.
        //      * @note このメソッドは派生クラスで実装しなければならない.
        //      * @return 確保したメモリサイズ(要求されたサイズと実際に確保したサイズは違うかもしれない)
        //      * 
        //      */
        //     virtual size_t alloc() = 0;

        //     inline size_t bytes() { return bytes_; }

        //     inline bool allocated() { return allocated_; }
        // };


        // using PhysicalMemoryPtr = shared_ptr<PhysicalMemory>;
        // using VecPhysicalMemoryPtr = std::vector<PhysicalMemoryPtr>;


        class Memory
        {
            string device_id_;
            bool locked_{false};
            Memory* next_{nullptr};
            Memory* prev_{nullptr};

        protected:
            // // メモリフラグ
            // enum MemoryType
            // {
            //     Normal = 0, // default
            //     Virtual = 1, // 内部に物理メモリを持つ仮想メモリ
            // };

            size_t bytes_{0};
            // long long requested_bytes_{-1};
            void* ptr_{nullptr};
            // MemoryType memory_type_{MemoryType::Normal};
            // VecPhysicalMemoryPtr p_memories_;

        private:
            inline void disable() { ptr_ = nullptr; }

        public:

            /**
             * @brief 派生クラスから呼ばれるコンストラクタ.
             * 
             * @param bytes 
             * @param device_id 
             */
            NBLA_API Memory(size_t bytes, const string& device_id);


            /**
             * @brief 派生クラスはデストラクタを呼ばなければならない.
             * このデストラクタは何もしない.
             * 派生クラスのMemory::alloc_impl()関数で確保したメモリは, 
             * 派生クラスのデストラクタで破棄しなければならない.
             * 実行時エラーをチェックするために, this->prev()関数がfalseを返すことを
             * 確認するアサーションを挿入することが望ましい.
             * 
             * @note もしptr_がnullptrならば, メモリは既に別のメモリにマージされており,
             * 破棄すべきメモリブロックは保持していない.
             * 
             */
            virtual ~Memory();

            inline size_t bytes() const { return bytes_; }

            // inline size_t requested_bytes() const 
            // {
            //     return requested_bytes_ < 0 ? bytes_ : requested_bytes_;
            // }

            inline string device_id() const { return device_id_; }

            inline void* pointer() { return ptr_; }

            inline const void* const_pointer() const { return ptr_; }

            /**
             * @brief このメモリインスタンスが利用可能ならば, trueを返す.
             * 
             * @note ロック状態は, RAII方式によってAllocatorMemoryインスタンスによって管理されている.
             * Allocator::alloc()関数は, ロック状態のMemoryインスタンスを保持したAllocatorMemoryインスタンスを戻り値として返す.
             * AllocatorMemoryインスタンスのライフタイムが終了するときに, Memory::try_merge()関数で
             * 連続するメモリブロック(next_ or prev_)とマージするか否かを知らせるために、ロック状態が解放される.
             * 
             * @return true 
             * @return false 
             */
            inline bool locked() const { return locked_; }

            /**
             * @brief 他のメモリブロックとマージによってMemoryインスタンスが利用できないか否かの状態を返す.
             * 
             * @return true 
             * @return false 
             */
            inline bool disabled() { return !ptr_; }

            inline Memory* next() const { return next_; }

            inline Memory* prev() const { return prev_; }

            inline void lock() { locked_ = true; }

            inline void release() { locked_ = false; }

            // /**
            //  * @brief 参照として物理的なメモリを取得する.
            //  * 
            //  * @return VecPhysicalMemoryPtr& 
            //  */
            // inline VecPhysicalMemoryPtr& get_physical_memory() { return p_memories_; }


            // /**
            //  * @brief 物理的メモリを追加
            //  * 
            //  * @param p_mems 
            //  */
            // inline void append_physical_memories(VecPhysicalMemoryPtr& p_mems)
            // {
            //     for (auto& m : p_mems)
            //         p_memories_.emplace_back(m);
            // }

            // inline void clear_physical_memory() { p_memories_.clear(); }


            /**
             * @brief 実装クラスのMemory::alloc_impl()関数によってメモリを確保する.
             * 
             * @note この関数は, このインスタンスを利用する前に, 呼び出すべき.
             * 派生クラスのAllocator::alloc_impl()関数の中で呼び出されるAllocator::alloc_retry()関数
             * を通じて, この関数は呼び出されるように設計されている.
             */
            void alloc();

            size_t bytes_active();

            shared_ptr<Memory> divide(size_t second_start);


            /**
             * @brief 可能であれば, fromで指定した別のメモリブロックとマージする.
             * 
             * マージを満たす条件
             * - `from` is a valid pointer.
             * - `from` is not locked.
             * - `from` is next or previous of this.
             * 
             * マージされたとき, `from`はMemory::disable()関数で利用禁止になる.
             * 
             * @param from 
             */
            void try_merge(Memory* from);


            static void associate_consecutive(Memory* left, Memory* right);


            // /**
            //  * @brief 物理的メモリを仮想メモリのアドレスに紐付ける.
            //  * このメソッドは, memory_type_ == MemType::Virtualの場合に限って, 実行できる.
            //  * 
            //  */
            // void bind();


            // /**
            //  * @brief 対応する仮想アドレスから物理メモリへの紐付けを解除し, 物理メモリをvectorのリストとして返す.
            //  * このメソッドは, memory_type_ == MemType::Virtualの場合に限って, 実行できる.
            //  * 
            //  */
            // void unbind();

            // /**
            //  * @brief 既に割付けされている仮想アドレスから仮想メモリを成長させる.
            //  * 
            //  * @param p_mems 
            //  * @return bool 
            //  */
            // bool grow(VecPhysicalMemoryPtr& p_mems);


            // /**
            //  * @brief デバイスメモリの状態を取得.
            //  * デフォルトでは, この関数は何もしない.
            //  * DeviceMemoryState::Unlockedを返す.
            //  * 
            //  * @return DeviceMemoryState 
            //  */
            // virtual DeviceMemoryState get_device_memory_state()
            // {
            //     return DeviceMemoryState::Unlocked;
            // }


            // /**
            //  * @brief デバイスのメモリ状態を`request`に要求する.
            //  * `request`はDeviceMemoryStateである必要がある.
            //  * デフォルトでは, この関数は何もしない.
            //  * 
            //  */
            // virtual void lock_device_memory() {};


            // inline void set_requested_bytes(size_t bytes) { requested_bytes_ = bytes; }


        // 仮想関数
        protected:
            /**
             * @brief 派生クラスで実装すること.
             * 
             * - Allocate a memory block.
             * - Set an allocated memory to ptr_ as a void*.
             * 
             * この関数はMemory::alloc()関数から呼び出される.
             * @note 実装クラスは, 確保したメモリブロックを解放するデストラクタを実装しなければならない.
             * もし, そのメモリブロックが元来確保されていて, まだアクティブ状態ならば.
             * See CpuMemory::~CpuMemory().
             * 
             * @return bool
             */
            virtual bool alloc_impl() = 0;
            

            /**
             * @brief 以前にMemory::alloc_impl()関数で確保したメモリのサブブロックを保持する
             * 新しいMemoryインスタンスを作る関数を派生クラスで実装すること.
             * 
             * このインスタンスのバイトサイズと分割されたメモリとこのメモリの連続性については,
             * インターフェース関数であるMemory::divide()によって扱われる.
             * 
             * @param second_start 
             * @return shared_ptr<Memory> 
             */
            virtual shared_ptr<Memory> divide_impl(size_t second_start) = 0;

            
            virtual void merge_next_impl(Memory* from) = 0;

            virtual void merge_prev_impl(Memory* from) = 0;

            // virtual void bind_impl()
            // {
            //     NBLA_ERROR(error_code::not_implemented, "bind_impl() is not implemented.");
            // }

            // virtual void unbind_impl()
            // {
            //     NBLA_ERROR(error_code::not_implemented, "unbind_impl() is not implemented.");
            // }

            // virtual bool grow_impl(VecPhysicalMemoryPtr& p_mems)
            // {
            //     NBLA_ERROR(error_code::not_implemented, "grow_impl() is not implemented.");
            // }
        };
    }
}