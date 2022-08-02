#pragma once

#include <IsNdArray/nd_array.hpp>
#include <IsNdArray/global_context.hpp>

// Generation
/*OK*/#include <IsNdArray/function/rand.hpp>
/*OK*/#include <IsNdArray/function/randint.hpp>
/*OK*/#include <IsNdArray/function/randn.hpp>
/*OK*/#include <IsNdArray/function/arange.hpp>
/*OK*/#include <IsNdArray/function/constant.hpp>

// PointWise-1
/*OK*/#include <IsNdArray/function/abs.hpp>
/*OK*/#include <IsNdArray/function/exp.hpp>
/*OK*/#include <IsNdArray/function/sinc.hpp>
/*OK*/#include <IsNdArray/function/cos.hpp>
/*OK*/#include <IsNdArray/function/cosh.hpp>
/*OK*/#include <IsNdArray/function/acos.hpp>
/*OK*/#include <IsNdArray/function/acosh.hpp>
/*OK*/#include <IsNdArray/function/sin.hpp>
/*OK*/#include <IsNdArray/function/sinh.hpp>
/*OK*/#include <IsNdArray/function/asin.hpp>
/*OK*/#include <IsNdArray/function/asinh.hpp>
/*OK*/#include <IsNdArray/function/atan.hpp>
/*OK*/#include <IsNdArray/function/tan.hpp>
/*OK*/#include <IsNdArray/function/tanh.hpp>
/*OK*/#include <IsNdArray/function/atanh.hpp>
/*OK*/#include <IsNdArray/function/sqrt.hpp>
/*OK*/#include <IsNdArray/function/ceil.hpp>
/*OK*/#include <IsNdArray/function/floor.hpp>
/*OK*/#include <IsNdArray/function/round.hpp>
/*OK*/#include <IsNdArray/function/log.hpp>
/*OK*/#include <IsNdArray/function/log1p.hpp>
/*OK*/#include <IsNdArray/function/log2.hpp>
/*OK*/#include <IsNdArray/function/log10.hpp>
/*OK*/#include <IsNdArray/function/log_sigmoid.hpp>
/*OK*/#include <IsNdArray/function/sigmoid.hpp>
/*OK*/#include <IsNdArray/function/sign.hpp>

// Pointwise-2
/*OK*/#include <IsNdArray/function/add_scalar.hpp>
/*OK*/#include <IsNdArray/function/pow_scalar.hpp>
/*OK*/#include <IsNdArray/function/mul_scalar.hpp>
/*OK*/#include <IsNdArray/function/r_div_scalar.hpp>
/*OK*/#include <IsNdArray/function/r_pow_scalar.hpp>
/*OK*/#include <IsNdArray/function/r_sub_scalar.hpp>
/*OK*/#include <IsNdArray/function/maximum_scalar.hpp>
/*OK*/#include <IsNdArray/function/minimum_scalar.hpp>

// Reduce
/*OK*/#include <IsNdArray/function/sum.hpp>
/*OK*/#include <IsNdArray/function/mean.hpp>
/*OK*/#include <IsNdArray/function/max.hpp>
/*OK*/#include <IsNdArray/function/min.hpp>
/*OK*/#include <IsNdArray/function/prod.hpp>

// Ope1
/*OK*/#include <IsNdArray/function/transpose.hpp>
/*OK*/#include <IsNdArray/function/broadcast.hpp>
/*OK*/#include <IsNdArray/function/reshape.hpp>
/*OK*/#include <IsNdArray/function/slice.hpp>
/*OK*/#include <IsNdArray/function/split.hpp>
/*--*/#include <IsNdArray/function/concatenate.hpp>

// Ope2
/*OK*/#include <IsNdArray/function/sort.hpp>
/*OK*/#include <IsNdArray/function/cumsum.hpp>
/*OK*/#include <IsNdArray/function/cumprod.hpp>

// 2-Input
/*KO*/#include <IsNdArray/function/add2.hpp>
/*--*/#include <IsNdArray/function/sub2.hpp>
/*--*/#include <IsNdArray/function/mul2.hpp>
/*--*/#include <IsNdArray/function/div2.hpp>

// 3-Input
/*--*/#include <IsNdArray/function/where.hpp>

// Other
/**/#include <IsNdArray/function/affine.hpp>
/**/#include <IsNdArray/function/softmax.hpp>
/**/#include <IsNdArray/function/stack.hpp>



#include <memory>


namespace is
{
    namespace nbla
    {
        // Generation -------------------------------------------------------

        // zeros
        template <typename T>
        NdArrayPtr zeros(const Shape_t& shape)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto ndarray = NdArray::create(shape);
            ndarray->zero();
            ndarray->cast_data_and_get_pointer<T>(ctx);
            return ndarray;
        }


        // ones
        template <typename T>
        NdArrayPtr ones(const Shape_t& shape)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            auto ndarray = NdArray::create(shape);
            ndarray->fill(1);
            ndarray->cast_data_and_get_pointer<T>(ctx);
            return ndarray;
        }


        // eye
        template <typename T>
        NdArrayPtr eye(int rows, int cols)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Shape_t shape{rows, cols};
            auto ndarray = NdArray::create(shape);
            ndarray->zero();
            T* data = ndarray->cast_data_and_get_pointer<T>(ctx);

            Stride_t strides = ndarray->strides();
            int min_len = std::min(rows, cols);
            for (int i = 0; i < min_len; ++i)
            {
                data[i * strides[0] + i * strides[1]] = (T)1;
            }
            return ndarray;
        }

        
        // randn
        template <typename F>
        NdArrayPtr randn(const Shape_t& shape, float mu, float sigma, int seed = -1)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Randn<F> operation(ctx, mu, sigma, shape, seed);
            auto output = zeros<F>(shape);
            operation.setup({}, {output});
            operation.execute({}, {output});
            return output;
        }


        // rand
        template <typename F>
        NdArrayPtr rand(const Shape_t& shape, float low, float high, int seed = -1)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Rand<F> operation(ctx, low, high, shape, seed);
            auto output = zeros<F>(shape);
            operation.setup({}, {output});
            operation.execute({}, {output});
            return output;
        }

        // randint
        template <typename T>
        NdArrayPtr randint(const Shape_t &shape, int low, int high, int seed = -1)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Randint<T> operation(ctx, low, high, shape, seed);
            auto output = zeros<T>(shape);
            operation.setup({}, {output});
            operation.execute({}, {output});
            return output;
        }

        // arange
        template <typename T>
        NdArrayPtr arange(double start, double stop, double step)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Arange<T> operation(ctx, start, stop, step);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({}, {output});
            operation.execute({}, {output});
            return output;
        }

        // constant
        template <typename T>
        NdArrayPtr constant(const Shape_t& shape, float val)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Arange<T> operation(ctx, val, shape);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({}, {output});
            operation.execute({}, {output});
            return output;
        }

        // PointWise-1 -------------------------------------------------------

        // abs
        template <typename T>
        NdArrayPtr abs(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Abs<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // exp
        template <typename T>
        NdArrayPtr exp(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Exp<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // cos
        template <typename T>
        NdArrayPtr cos(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Cos<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // cosh
        template <typename T>
        NdArrayPtr cosh(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Cosh<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // acos
        template <typename T>
        NdArrayPtr acos(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            ACos<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // acosh
        template <typename T>
        NdArrayPtr acosh(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            ACosh<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // sin
        template <typename T>
        NdArrayPtr sin(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Sin<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // sinh
        template <typename T>
        NdArrayPtr sinh(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Sinh<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // asin
        template <typename T>
        NdArrayPtr asin(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            ASin<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // asinh
        template <typename T>
        NdArrayPtr asinh(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            ASinh<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // tan
        template <typename T>
        NdArrayPtr tan(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Tan<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // tanh
        template <typename T>
        NdArrayPtr tanh(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Tanh<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // atan
        template <typename T>
        NdArrayPtr atan(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            ATan<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // atanh
        template <typename T>
        NdArrayPtr atanh(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            ATanh<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // sqrt
        template <typename T>
        NdArrayPtr sqrt(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Sqrt<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // ceil
        template <typename T>
        NdArrayPtr ceil(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Ceil<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // floor
        template <typename T>
        NdArrayPtr floor(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Floor<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // round
        template <typename T>
        NdArrayPtr round(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Round<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // log
        template <typename T>
        NdArrayPtr log(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Log<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // log1p
        template <typename T>
        NdArrayPtr log1p(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Log1p<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // log2
        template <typename T>
        NdArrayPtr log2(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Log2<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // log10
        template <typename T>
        NdArrayPtr log10(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Log10<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // log_sigmoid
        template <typename T>
        NdArrayPtr log_sigmoid(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            LogSigmoid<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // sigmoid
        template <typename T>
        NdArrayPtr sigmoid(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Sigmoid<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // sign
        template <typename T>
        NdArrayPtr sign(NdArrayPtr input)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Sign<T> operation(ctx);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // PointWise-2 -------------------------------------------------------

        // add_scalar
        template <typename T>
        NdArrayPtr add_scalar(NdArrayPtr input, double val, bool inplace = false)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            AddScalar<T> operation(ctx, val, inplace);
            if (inplace) {
                operation.setup({input}, {input});
                operation.execute({input}, {input});
                return input;
            }
            else {
                auto output = NdArray::create();
                output->cast_data_and_get_pointer<T>(ctx);
                operation.setup({input}, {output});
                operation.execute({input}, {output});
                return output;
            }
        }

        // pow_scalar
        template <typename T>
        NdArrayPtr pow_scalar(NdArrayPtr input, double val, bool inplace = false)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            PowScalar<T> operation(ctx, val, inplace);
            if (inplace) {
                operation.setup({input}, {input});
                operation.execute({input}, {input});
                return input;
            }
            else {
                auto output = NdArray::create();
                output->cast_data_and_get_pointer<T>(ctx);
                operation.setup({input}, {output});
                operation.execute({input}, {output});
                return output;
            }
        }

        // mul_scalar
        template <typename T>
        NdArrayPtr mul_scalar(NdArrayPtr input, double val, bool inplace = false)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            MulScalar<T> operation(ctx, val, inplace);
            if (inplace) {
                operation.setup({input}, {input});
                operation.execute({input}, {input});
                return input;
            }
            else {
                auto output = NdArray::create();
                output->cast_data_and_get_pointer<T>(ctx);
                operation.setup({input}, {output});
                operation.execute({input}, {output});
                return output;
            }
        }

        // r_div_scalar
        template <typename T>
        NdArrayPtr r_div_scalar(NdArrayPtr input, double val, bool inplace = false)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            RDivScalar<T> operation(ctx, val, inplace);
            if (inplace) {
                operation.setup({input}, {input});
                operation.execute({input}, {input});
                return input;
            }
            else {
                auto output = NdArray::create();
                output->cast_data_and_get_pointer<T>(ctx);
                operation.setup({input}, {output});
                operation.execute({input}, {output});
                return output;
            }
        }

        // r_pow_scalar
        template <typename T>
        NdArrayPtr r_pow_scalar(NdArrayPtr input, double val, bool inplace = false)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            RPowScalar<T> operation(ctx, val, inplace);
            if (inplace) {
                operation.setup({input}, {input});
                operation.execute({input}, {input});
                return input;
            }
            else {
                auto output = NdArray::create();
                output->cast_data_and_get_pointer<T>(ctx);
                operation.setup({input}, {output});
                operation.execute({input}, {output});
                return output;
            }
        }

        // r_sub_scalar
        template <typename T>
        NdArrayPtr r_sub_scalar(NdArrayPtr input, double val, bool inplace = false)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            RSubScalar<T> operation(ctx, val, inplace);
            if (inplace) {
                operation.setup({input}, {input});
                operation.execute({input}, {input});
                return input;
            }
            else {
                auto output = NdArray::create();
                output->cast_data_and_get_pointer<T>(ctx);
                operation.setup({input}, {output});
                operation.execute({input}, {output});
                return output;
            }
        }

        // maximum_scalar
        template <typename T>
        NdArrayPtr maximum_scalar(NdArrayPtr input, double val)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            MaximumScalar<T> operation(ctx, val);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // minimum_scalar
        template <typename T>
        NdArrayPtr minimum_scalar(NdArrayPtr input, double val)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            MinimumScalar<T> operation(ctx, val);
            auto output = NdArray::create();
            output->cast_data_and_get_pointer<T>(ctx);
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // Reduce -------------------------------------------------------
        
        // sum
        template <typename T>
        NdArrayPtr sum(NdArrayPtr input, int axis = 0, bool keep_dims = false)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Sum<T> operation(ctx, {axis}, keep_dims);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // mean
        template <typename T>
        NdArrayPtr mean(NdArrayPtr input, int axis = 0, bool keep_dims = false) 
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Mean<T> operation(ctx, {axis}, keep_dims);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // max
        template <typename T>
        vector<NdArrayPtr> max(NdArrayPtr input, int axis = 0, bool keep_dims = false, bool with_index = false, bool only_index = false) 
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Max<T> operation(ctx, {axis}, keep_dims, with_index, only_index);

            if (!with_index)
            {
                auto output = NdArray::create();
                operation.setup({input}, {output});
                operation.execute({input}, {output});
                return {output};
            }
            else
            {
                auto output = NdArray::create();
                auto output_index = NdArray::create();
                operation.setup({input}, {output, output_index});
                operation.execute({input}, {output, output_index});
                return {output, output_index};
            }
        }

        // min
        template <typename T>
        vector<NdArrayPtr> min(NdArrayPtr input, int axis = 0, bool keep_dims = false, bool with_index = false, bool only_index = false) 
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Min<T> operation(ctx, {axis}, keep_dims, with_index, only_index);

            if (!with_index)
            {
                auto output = NdArray::create();
                operation.setup({input}, {output});
                operation.execute({input}, {output});
                return {output};
            }
            else
            {
                auto output = NdArray::create();
                auto output_index = NdArray::create();
                operation.setup({input}, {output, output_index});
                operation.execute({input}, {output, output_index});
                return {output, output_index};
            }
        }

        // prod
        template <typename T>
        NdArrayPtr prod(NdArrayPtr input, vector<int64_t>& axes, bool keep_dims = false) 
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Prod<T> operation(ctx, axes, keep_dims);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return {output};
        }


        // Ope1 -------------------------------------------------------
        
        // transpose
        template <typename T>
        NdArrayPtr transpose(NdArrayPtr input, const std::vector<int64_t> &axes)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Transpose<T> operation(ctx, axes);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }

        // broadcast
        template <typename T>
        NdArrayPtr broadcast(NdArrayPtr input, const Shape_t& shape)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Broadcast<T> operation(ctx, shape);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // reshape
        template <typename T>
        NdArrayPtr reshape(NdArrayPtr input, const std::vector<int64_t> &shape)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Reshape<T> operation(ctx, shape, false);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // slice
        template <typename T>
        NdArrayPtr slice(NdArrayPtr input, const vector<int64_t> &starts, 
                         const vector<int64_t> &stops, const vector<int64_t> &steps)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Slice<T> operation(ctx, starts, stops, steps);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return output;
        }


        // split
        template <typename T>
        vector<NdArrayPtr> split(NdArrayPtr input, int axis) 
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Split<T> operation(ctx, axis);
            auto sh = input->shape();
            int num = sh.at(axis);
            vector<NdArrayPtr> outputs(num);
            for (int i = 0; i < num; ++i)
            {
                outputs[i] = NdArray::create();
            }
            operation.setup({input}, outputs);
            operation.execute({input}, outputs);
            return outputs;
        }


        // concatenate
        template <typename T>
        NdArrayPtr concatenate(vector<NdArrayPtr> inputs, int axis)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Concatenate<T> operation(ctx, axis);
            auto output = NdArray::create();
            operation.setup(inputs, {output});
            operation.execute(inputs, {output});
            return output;
        }


        // Ope2 -------------------------------------------------------

        // sort
        template <typename T>
        vector<NdArrayPtr> sort(NdArrayPtr input, int axis = 0, bool reverse = false, bool with_index = false, bool only_index = false) 
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Sort<T> operation(ctx, {axis}, reverse, with_index, only_index);

            if (!with_index)
            {
                auto output = NdArray::create();
                operation.setup({input}, {output});
                operation.execute({input}, {output});
                return {output};
            }
            else
            {
                auto output = NdArray::create();
                auto output_index = NdArray::create();
                operation.setup({input}, {output, output_index});
                operation.execute({input}, {output, output_index});
                return {output, output_index};
            }
        }

        // cumsum
        template <typename T>
        NdArrayPtr cumsum(NdArrayPtr input, int axis = 0, bool exclusive = false, bool reverse = false) 
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            CumSum<T> operation(ctx, axis, exclusive, reverse);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return {output};           
        }

        // cumprod
        template <typename T>
        NdArrayPtr cumprod(NdArrayPtr input, int axis = 0, bool exclusive = false, bool reverse = false) 
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            CumProd<T> operation(ctx, axis, exclusive, reverse);
            auto output = NdArray::create();
            operation.setup({input}, {output});
            operation.execute({input}, {output});
            return {output};           
        }

    
        // 2-Input -------------------------------------------------------

        // add2
        template <typename T>
        NdArrayPtr add2(NdArrayPtr left, NdArrayPtr right)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Add2<T> operation(ctx, false/*inplace*/);

            auto output = NdArray::create();
            operation.setup({left, right}, {output});
            operation.execute({left, right}, {output});
            return output;
        }


        // sub2
        template <typename T>
        NdArrayPtr sub2(NdArrayPtr left, NdArrayPtr right)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Sub2<T> operation(ctx, false/*inplace*/);

            auto output = NdArray::create();
            operation.setup({left, right}, {output});
            operation.execute({left, right}, {output});
            return output;
        }


        // mul2
        template <typename T>
        NdArrayPtr mul2(NdArrayPtr left, NdArrayPtr right)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Mul2<T> operation(ctx, false/*inplace*/);

            auto output = NdArray::create();
            operation.setup({left, right}, {output});
            operation.execute({left, right}, {output});
            return output;
        }


        // div2
        template <typename T>
        NdArrayPtr div2(NdArrayPtr left, NdArrayPtr right)
        {
            const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
            Div2<T> operation(ctx, false/*inplace*/);

            auto output = NdArray::create();
            operation.setup({left, right}, {output});
            operation.execute({left, right}, {output});
            return output;
        }


    }
}