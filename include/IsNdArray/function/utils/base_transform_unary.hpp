#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

#ifdef _OPENMP
#include <omp.h>
#endif

namespace is
{
    namespace nbla
    {
        /*引数の個数が1個の場合の関数Commandパターンの雛形*/
        template <typename... Args>
        class BaseTransformUnary : public BaseFunction<Args...>
        {
        protected:
            bool inplace_;

        public:
            BaseTransformUnary(const Context& ctx, bool inplace, Args... args)
                : BaseFunction<Args...>(ctx, args...)
                , inplace_(inplace) {}

            virtual ~BaseTransformUnary() {}

            virtual int min_inputs() override { return 1; }

            virtual int min_outputs() override { return 1; }

            virtual int inplace_data(int i) const override
            {
                if (!inplace_)
                {
                    return Function::NOT_INPLACE;
                }
                return Function::INPLACE;
            }

            virtual int inplace_data_with(int i) const override
            {
                return 0;
            }

        protected:
            virtual void setup_impl(const NdArrays& inputs, const NdArrays& outputs) override
            {
                outputs[0]->reshape(inputs[0]->shape(), true);
                if (inplace_)
                {
                    outputs[0]->set_array(inputs[0]->array());
                }
            }
        };


        /*引数が1個の場合の関数用コマンドパターンクラス*/
        // 演算操作は状態を持つポリシークラス.
        template <typename T, typename UnaryOp, typename... Args>
        class TransformUnary : public BaseTransformUnary<Args...>
        {
        protected:
            UnaryOp unary_op_;

        public:
            TransformUnary(const Context& ctx, bool inplace, Args... args)
                : BaseTransformUnary<Args...>(ctx, inplace, args...)
                , unary_op_(args...) {}

            virtual ~TransformUnary() {}

            virtual vector<dtypes> in_types() override { return vector<dtypes>{ get_dtype<T>() }; }

            virtual vector<dtypes> out_types() override { return vector<dtypes>{ get_dtype<T>() }; }

            virtual vector<string> allowed_array_classes()
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }

        protected:
            virtual void execute_impl(const NdArrays& inputs, const NdArrays& outputs);
        };


        /* 演算操作を行うポリシークラス */
        class BaseUnaryOp
        {
        public:
            inline BaseUnaryOp() {}

            template <typename T> inline
            T operator()(const T x)
            {
                NBLA_ERROR(error_code::not_implemented,
                            "Execute operation is not implemented.");
            }
        };


        /* execute演算実行テンプレート関数 */
        template <typename T, typename UnaryOp>
        void transform_unary(Size_t size, const T* x, T* y, UnaryOp op)
        {
#ifdef _OPENMP
    #pragma omp parallel for schedule(static)
#endif
            for (Size_t idx = 0; idx < size; ++idx)
            {
                y[idx] = op(x[idx]);
            }
        }

        /* execute_impl for TransformUnary */
        template <typename T, typename UnaryOp, typename... Args>
        void TransformUnary<T, UnaryOp, Args...>::execute_impl(
            const NdArrays& inputs, const NdArrays& outputs)
        {
            const T* x = inputs[0]->get_data_pointer<T>(this->ctx_);
            T* y = outputs[0]->cast_data_and_get_pointer<T>(this->ctx_, !this->inplace_); // 常にinplace?
            transform_unary(inputs[0]->size(), x, y, unary_op_);
        }

/********************** Transform-Unary(Op)系クラス作成用ヘルパーマクロ **********************************/

#define NBLA_DEFINE_UNARY_OP_CLASS(NAME)                                                              \
    class NAME##UnaryOp : public BaseUnaryOp

#define NBLA_DEFINE_UNARY_OP_EXECUTE(OP)                                                              \
    template <typename T> inline T operator()(const T x) { return OP; }

#define NBLA_DEFINE_TRANSFORM_UNARY_CLASS_COMMON(NAME)                                                \
public:                                                                                               \
    virtual ~NAME() {}                                                                                \
    virtual string name() { return #NAME; }                                                           \

// ----------------------------------------------------------------------------------------------------
// Zero argument
// ----------------------------------------------------------------------------------------------------

#define NBLA_DEFINE_UNARY_OP(NAME, OP)                                                                \
    NBLA_DEFINE_UNARY_OP_CLASS(NAME)                                                                  \
    {                                                                                                 \
    public:                                                                                           \
        NBLA_DEFINE_UNARY_OP_EXECUTE(OP)                                                              \
    };

/* Transform Unary クラス */
#define NBLA_DEFINE_TRANSFORM_UNARY_CLASS(NAME)                                                       \
    template <typename T>                                                                             \
    class NAME : public TransformUnary<T, NAME##UnaryOp>                                              \
    {                                                                                                 \
        NBLA_DEFINE_TRANSFORM_UNARY_CLASS_COMMON(NAME)                                                \
                                                                                                      \
        NAME(const Context& ctx)                                                                      \
            : TransformUnary<T, NAME##UnaryOp>(ctx, false) {}                                         \
                                                                                                      \
        virtual shared_ptr<Function> copy() const                                                     \
        {                                                                                             \
            return create_##NAME(this->ctx_);                                                         \
        }                                                                                             \
    };

/* Commandパターンに必要な機能3点セットでマクロ定義 */
#define NBLA_DEFINE_TRANSFORM_UNARY(NAME, OP)                                                         \
    NBLA_REGISTER_FUNCTION_HEADER(NAME)                                                               \
    NBLA_DEFINE_UNARY_OP(NAME, OP)                                                                    \
    NBLA_DEFINE_TRANSFORM_UNARY_CLASS(NAME)

// ----------------------------------------------------------------------------------------------------
// One argument
// ----------------------------------------------------------------------------------------------------

#define NBLA_DEFINE_TRANSFORM_UNARY_CLASS_1(NAME, A0)                                                 \
    template <typename T>                                                                             \
    class NAME : public TransformUnary<T, NAME##UnaryOp, A0>                                          \
    {                                                                                                 \
        NBLA_DEFINE_TRANSFORM_UNARY_CLASS_COMMON(NAME)                                                \
                                                                                                      \
        NAME(const Context& ctx, const A0& a0)                                                        \
            : TransformUnary<T, NAME##UnaryOp, A0>(ctx, false, a0) {}                                 \
                                                                                                      \
        virtual shared_ptr<Function> copy() const                                                     \
        {                                                                                             \
            return create_##NAME(this->ctx_, std::get<0>(this->args_));                               \
        }                                                                                             \
    };

#define NBLA_DEFINE_TRANSFORM_UNARY_CLASS_1_INPLACE(NAME, A0)                                         \
    template <typename T>                                                                             \
    class NAME : public TransformUnary<T, NAME##UnaryOp, A0>                                          \
    {                                                                                                 \
        NBLA_DEFINE_TRANSFORM_UNARY_CLASS_COMMON(NAME)                                                \
                                                                                                      \
        NAME(const Context &ctx, const A0 &a0, bool inplace)                                          \
            : TransformUnary<T, NAME##UnaryOp, A0>(ctx, inplace, a0) {}                               \
                                                                                                      \
        virtual shared_ptr<Function> copy() const                                                     \
        {                                                                                             \
            return create_##NAME(this->ctx_, std::get<0>(this->args_), this->inplace_);               \
        }                                                                                             \
    };

#define NBLA_DEFINE_UNARY_OP_1(NAME, OP, A0)                                                          \
    NBLA_DEFINE_UNARY_OP_CLASS(NAME)                                                                  \
    {                                                                                                 \
    public:                                                                                           \
        A0 a0;                                                                                        \
                                                                                                      \
        inline NAME##UnaryOp(A0 a0_)                                                                  \
            : a0(a0_) {}                                                                              \
                                                                                                      \
        NBLA_DEFINE_UNARY_OP_EXECUTE(OP)                                                              \
    };

/* Commandパターンに必要な機能3点セットでマクロ定義 */
#define NBLA_DEFINE_TRANSFORM_UNARY_1(NAME, OP, A0)                                                   \
    NBLA_REGISTER_FUNCTION_HEADER(NAME, A0)                                                           \
    NBLA_DEFINE_UNARY_OP_1(NAME, OP, A0)                                                              \
    NBLA_DEFINE_TRANSFORM_UNARY_CLASS_1(NAME, A0)  

/* Commandパターンに必要な機能3点セットでマクロ定義: INPLACE */
#define NBLA_DEFINE_TRANSFORM_UNARY_1_INPLACE(NAME, OP, A0)                                           \
    NBLA_REGISTER_FUNCTION_HEADER(NAME, A0, bool)                                                     \
    NBLA_DEFINE_UNARY_OP_1(NAME, OP, A0)                                                              \
    NBLA_DEFINE_TRANSFORM_UNARY_CLASS_1_INPLACE(NAME, A0)


#define NBLA_DEFINE_UNARY_OP_1(NAME, OP, A0)                                                          \
  NBLA_DEFINE_UNARY_OP_CLASS(NAME)                                                                    \
  {                                                                                                   \
  public:                                                                                             \
    A0 a0;                                                                                            \
                                                                                                      \
    inline NAME##UnaryOp(A0 a0_)                                                                      \
        : a0(a0_) {}                                                                                  \
                                                                                                      \
    NBLA_DEFINE_UNARY_OP_EXECUTE(OP)                                                                  \
  };

    } // namespace nbla
}