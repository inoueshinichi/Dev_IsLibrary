#pragma once

#include <IsNdArray/array.hpp>
#include <IsNdArray/context.hpp>
#include <IsNdArray/nd_array.hpp>

#include <memory>
#include <string>
#include <tuple>

namespace is
{
    using std::string;
    using std::vector;
    using std::shared_ptr;
    using std::make_shared;
    using std::tuple;
    using std::get;

    namespace nbla
    {
        using NdArrays = vector<NdArrayPtr>;

        /**
         * @brief 計算ユニットのインターフェース
         * execute()関数を実装する実装関数クラスに派生する
         * 
         * execute:
         *  y = f(x)
         * 
         */
        // 入出力情報・Inplace/Not・演算操作のAPIを定義するクラス
        class NBLA_API Function
        {
            bool used_{false};

            // Function::setup()関数が一度呼ばれると, このフラグはTrueになる
            bool called_setup_{false};

        public:
            using Ptr = shared_ptr<Function>;

        protected:
            Context ctx_;
            vector<shared_ptr<Shape_t>> in_shapes_;
            vector<shared_ptr<Shape_t>> out_shapes_;

            /* フォールバック関数 */
            // 関数のインスタンス化時にセットされると関数の振る舞いが
            // フォールバック関数に置き換わる
            Ptr fall_back_func_;

        public:
            // Inplace level used in inplace_data function.
            static const int NOT_INPLACE = 0;
            static const int INPLACE_NOT_MODIFY = 1;
            static const int INPLACE = 2;

            // Copying and storing Context.
            explicit Function(const Context& ctx);
            virtual ~Function() = 0;

            /**
             * @brief Setting up function.
             * Functionのインスタンスが使用される前に、呼び出す必要がある.
             * 
             * - Determin Array class used according to the return of
			 *   Function::allowed_array_classes and the given context.
             * 
             * - Type and shape check.
             * 
             * - Calling Function::setup_impl.
             * 
             * - Pre-allocate memory to prevent locking in asynchronous execution in CUDA etc.
             * 
             * @param inputs 
             * @param outputs 
             */
            void setup(const NdArrays& inputs, const NdArrays& outputs);


            /**
             * @brief 計算結果をoutputのデータに格納する
             * 
             * Checking shapes before calling execute_impl() which
             * be implemented in a derived function.
             * 
             * @param inputs 
             * @param outputs 
             */
            // void forward(const Variables& inputs, const Variables& outputs);
            void execute(const NdArrays& inputs, const NdArrays& outputs);
            
            /**
             * @brief この関数で使われているContextを取得する.
             * 
             * @return Context 
             */
            Context context() const;


            /**
             * @brief 入力のDtypesを取得
             * 
             * @return vector<dtypes> 
             */
            virtual vector<dtypes> in_types() = 0;


            /**
             * @brief 出力のDtypesを取得
             * 
             * @return vector<dtypes> 
             */
            virtual vector<dtypes> out_types() = 0;


            /**
             * @brief  Get minimum number of inputs.
             * This is meant to be used in setup fuction with in_types which is used
             * to get maximum number of inputs.
             * @return int
             */
            virtual int min_inputs() = 0;

            /**
             * @brief Get minimum number of outputs.
             * This is meant to be used in setup function with out_types which is used to get
             * max number of outputs.
             * @return int
             */
            virtual int min_outputs() = 0;


            /**
             * @brief Get function name in string
             * 
             * @return string 
             */
            virtual string name() = 0;


            /**
             * @brief Get array classes that are allowed to be specified by Context
             * 
             * @return vector<string> 
             */
            virtual vector<string> allowed_array_classes() = 0;


            /**
             * @brief  Get in-place-level of i-th input variable's data (see below).
             * 0 (NOT_INPLACE): Not in-placed
             * 1 (INPLACE_NOT_MODIFY): In-placed but not modified.
             * 2 (INPLACE): In-placed and modified.
             * 
             * @param i Input variable index.
             * @return int 0 by default.
             * 
             * @note If a subclass uses in-place computation, the function must override
             *       this function.
             */
            virtual int inplace_data(int i) const
            {
                return NOT_INPLACE;
            }


            /**
             * @brief Get the output variable index where i-th variables' data in-placed to.
             * 
             * @param i Input variable index.
             * @return int 
             * 
             * @note This is only valid if the i-th variable is in-placed.
             *       The maintainer of a sub-class function must override
             *       this function.
             */
            virtual int inplace_data_with(int i) const
            {
                NBLA_ERROR(error_code::not_implemented,
                           "This must be implemented for in-place support of this function.");
            }

            /**
             * @brief Copy another instance of Function with the same context.
             * 
             * @return shared_ptr<Function> 
             */
            virtual shared_ptr<Function> copy() const = 0;


            /**
             * @brief Check whether this was already used, and turn it used.
             * 
             * @return true 
             * @return false 
             */
            inline bool ask_if_used_and_use()
            {
                bool r = used_;
                used_ = true;
                return r;
            }


        protected:
            /**
             * @brief Implementation part of setup().
             * It must do:
             * - Reshape output Variables.
             * - Allocate resources used in execute computation if necessary.
             * - Checking shapes and dtypes etc.
             * 
             * @param inputs 
             * @param outputs 
             */
            virtual void setup_impl(const NdArrays& inputs, const NdArrays& outputs) = 0;


            /**
             * @brief Implementation part of execute().
             * It must do:
             * - Take data in inputs and store results into data in outputs.
             * 
             * @param inputs 
             * @param outputs 
             */
            virtual void execute_impl(const NdArrays& inputs, const NdArrays& outputs) = 0;


            DISABLE_COPY_AND_ASSIGN(Function)
        };

        // エイリアス
        using FunctionPtr = Function::Ptr;


        /**
         * @brief Base function.
         * 
         * Keep arguments
         * 
         */
        // 演算操作に必要な仮引数の型情報を保持するクラス
        template <typename... Args>
        class BaseFunction : public Function
        {
        protected:
            tuple<typename std::remove_reference_t<Args>...> args_;

        public:
            using base_function_type = BaseFunction<Args...>;

            BaseFunction(const Context& ctx, Args... args)
                : Function(ctx)
                , args_(args...) {}

            /**
             * @brief コンストラクタに与えた引数の数を取得.
             * 
             * @return int 
             */
            int num_args()
            {
                return sizeof...(Args);
            }


            /**
             * @brief タプルとして引数を取得.
             * 
             * @return * const tuple<Args...>& 
             */
            const tuple<Args...>& get_args()
            {
                return args_;
            }


            template <int Index>
            auto get_arg() -> decltype(std::get<Index>(args_))
            {
                return std::get<Index>(args_);
            }
        };

        using FunctionPtr = Function::Ptr;


// 追加 shinichi inoue 20210504
#define NBLA_INSTANTIATE_CLASS(CLS, TYPE) \
    template class CLS<TYPE>;

    } // namespace nbla
}