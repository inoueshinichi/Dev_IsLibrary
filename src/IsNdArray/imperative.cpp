#include <IsNdArray/imperative.hpp>
#include <IsNdArray/nd_array.hpp>

#include <memory>

namespace is
{
    namespace nbla
    {
        namespace imp
        {
            using std::make_shared;

            vector<NdArrayPtr> execute(FunctionPtr func,
                                    const NdArrays& inputs,
                                    int n_outputs,
                                    NdArrays outputs)
            {
                // Check inplace outputs size.
                NBLA_CHECK(outputs.size() <= static_cast<unsigned>(n_outputs),
                        error_code::value,
                        "Num of in-place arrays must not be greater than n_outputs. "
                        "In-place array: %d <= n_outputs: %d",
                        outputs.size(), n_outputs);
                
                if (outputs.size() != static_cast<unsigned>(n_outputs))
                {
                    outputs.resize(n_outputs, nullptr);
                }

                // Copy if function is alreadly used.
                if (func->ask_if_used_and_use())
                {
                    func = func->copy();
                }

                // // Function inputs and outputs must be Variables.
                // vector<VariablePtr> vinputs(inputs.size());
                // vector<VariablePtr> voutputs(outputs.size());
                // for (unsigned int i = 0; i < inputs.size(); ++i) {
                //     vinputs[i] = make_shared<Variable>(inputs[i]);
                // }
                // for (int i = 0; i < n_outputs; ++i) {
                //     voutputs[i] = make_shared<Variable>();
                // }
                // auto finputs = as_pointer_array(vinputs);
                // auto foutputs = as_pointer_array(voutputs);

                // Setup function
                func->setup(inputs, outputs);

                // // Set inplace buffer to function output buffer if size matches.
                // for (unsigned int i = 0; i < outputs.size(); ++i) {
                //     if (!outputs[i]) {
                //     outputs[i] = foutputs[i]->data();
                //     }
                //     NBLA_CHECK(outputs[i]->size() == foutputs[i]->size(), error_code::value,
                //             "In-place array size and function output size must match. "
                //             "outputs[%d] size: %d, function output[%d] size: %d",
                //             i, outputs[i]->size(), i, foutputs[i]->size());
                //     foutputs[i]->data()->set_array(outputs[i]->array()); // Inplace.
                // }

                // Execute 
                func->execute(inputs, outputs);
                return outputs;
            }


            void execute(FunctionPtr func,
                        const NdArrays& inputs,
                        const NdArrays& outputs)
            {
                func->setup(inputs, outputs);
                func->execute(inputs, outputs);
            }
        }
    }
}