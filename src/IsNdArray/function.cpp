#include <IsNdArray/function.hpp>

#include <algorithm>
#include <memory>

namespace is
{
    namespace nbla
    {
        using std::make_shared;

        Function::Function(const Context& ctx)
            : ctx_(ctx)
            , fall_back_func_(nullptr) {}


        Function::~Function() {}

        
        void Function::setup(const NdArrays& inputs, const NdArrays& outputs)
        {
            if (fall_back_func_)
            {
                // Fall back to the specified Function.
                fall_back_func_->setup(inputs, outputs);
                return;
            }

            // Check if specified array_class by context matches to allowed array classes.
            int array_class_index = 0; // Default array is 0-th array_class in allowed_array_classes().


            for (vector<string>::size_type i = 0;
                 i < this->allowed_array_classes().size(); ++i)
            {
                if (ctx_.array_class == this->allowed_array_classes()[i])
                {
                    array_class_index = i;
                }
            }
            
            ctx_.set_array_class(this->allowed_array_classes()[array_class_index]);

            // Check number of inputs and outputs
            auto&& in_types = this->in_types();
            auto&& out_types = this->out_types();
            auto min_inputs = static_cast<NdArrays::size_type>(this->min_inputs());
            auto min_outputs = static_cast<NdArrays::size_type>(this->min_outputs());

            NBLA_CHECK(min_inputs <= inputs.size(), error_code::value,
                       "%s needs at least %d inputs (given %d). ", this->name().c_str(),
                       this->min_inputs(), inputs.size());
            NBLA_CHECK(min_outputs <= outputs.size(), error_code::value,
                       "%s needs at least %d outputs (given %d). ", this->name().c_str(),
                       this->min_outputs(), outputs.size());
            
            // Call setup implementation
            this->setup_impl(inputs, outputs);

            if (fall_back_func_)
            {
                return;
            }

            // Memorize shapes
            in_shapes_.clear();
            out_shapes_.clear();
            for (NdArrays::size_type i = 0; i < inputs.size(); ++i)
            {
                in_shapes_.push_back(make_shared<Shape_t>(inputs[i]->shape()));
            }
            for (NdArrays::size_type i = 0; i < outputs.size(); ++i)
            {
                out_shapes_.push_back(make_shared<Shape_t>(outputs[i]->shape()));
            }
        }


        static 
        void check_shapes(Function* function, 
                          const NdArrays& inputs, const NdArrays& outputs,
                          const vector<shared_ptr<Shape_t>>& in_shapes,
                          const vector<shared_ptr<Shape_t>>& out_shapes)
        {
            NBLA_CHECK(inputs.size() == in_shapes.size(), error_code::value,
				       "Num of inputs has been changed since setup is called in %s. "
				       "Given: %d != previously: %d. ",
				       function->name().c_str(), inputs.size(), in_shapes.size());
			NBLA_CHECK(outputs.size() == out_shapes.size(), error_code::value,
				       "Num of outputs has been changed since setup is called in %s. "
				       "Given: %d != previously: %d. ",
				       function->name().c_str(), outputs.size(), out_shapes.size());

            for (std::size_t i = 0; i < inputs.size(); ++i)
			{
				NBLA_CHECK(*in_shapes[i] == inputs[i]->shape(), error_code::value,
					       "Inconsistent shape in input %d of %s. "
					       "Setup: (%s) != Given: (%s).",
					       i, function->name().c_str(),
					       string_join(*(in_shapes[i]), string(", ")).c_str(),
					       string_join(inputs[i]->shape(), string(", ")).c_str());
			}

			for (std::size_t i = 0; i < outputs.size(); ++i)
			{
				NBLA_CHECK(*out_shapes[i] == outputs[i]->shape(), error_code::value,
					       "Inconsistent shape in output %d of %s. "
					       "Setup: (%s) != Given: (%s).",
					       i, function->name().c_str(),
					       string_join(*(out_shapes[i]), string(", ")).c_str(),
					       string_join(outputs[i]->shape(), string(", ")).c_str());
			}
        }


        void Function::execute(const NdArrays& inputs, const NdArrays& outputs)
        {
            if (fall_back_func_)
            {
                // Fall back to the specified function.
                fall_back_func_->execute(inputs, outputs);
                return;
            }

            check_shapes(this, inputs, outputs, in_shapes_, out_shapes_);
            this->execute_impl(inputs, outputs);
        }

        

        Context Function::context() const
        {
            return ctx_; 
        }
    } // namespace nbla
}