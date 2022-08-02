#pragma once

#include <IsNdArray/cpu.hpp>
#include <IsNdArray/function.hpp>
#include <IsNdArray/function_registry.hpp>

namespace is
{
    namespace nbla
    {
        NBLA_REGISTER_FUNCTION_HEADER(MatrixDiagPart)

        /**
         * @brief matrix diagonal part
         * 
         */

        template <typename T>
        class MatrixDiagPart : public BaseFunction<>
        {
        protected:
            Size_t last_ndim_;

        public:
            MatrixDiagPart(const Context& ctx) : BaseFunction<>(ctx) {}
            virtual ~MatrixDiagPart() {}
            virtual shared_ptr<Function> copy() const { return create_MatrixDiagPart(ctx_); }
            virtual vector<dtypes> in_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual vector<dtypes> out_types() { return vector<dtypes>{get_dtype<T>()}; }
            virtual int min_inputs() { return 1; }
            virtual int min_outputs() { return 1; }
            virtual string name() { return "MatrixDiagPart"; }
            virtual vector<string> allowed_array_classes() 
            {
                return SingletonManager::get<Cpu>()->array_classes();
            }

        protected:
            NBLA_API virtual void setup_impl(const NdArrays& inputs,
                                             const NdArrays& outputs);

            NBLA_API virtual void execute_impl(const NdArrays& inputs,
                                               const NdArrays& outputs);
        };
    }
}