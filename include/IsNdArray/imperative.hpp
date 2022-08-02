#pragma once

#include <IsNdArray/function.hpp>
#include <IsNdArray/nd_array.hpp>

namespace is
{
    namespace nbla
    {
        namespace imp
        {
            /**
            * @brief 
            * @param[in] inputs A vector of NdArray as function inputs. NdArrays are converted to data regions of  Variables.
            * @param[in] n_outputs Number of function outputs.
            * @param[in,out] outputs, This can be empty usually. Elements which are not nullptr will be used as in-place outputs.
            */

            /**
            * @brief Execute a function given NdArray instances as inputs.
            * 
            * @param[in] func A shared pointer of Function.
            * @param[in] inputs A vector of NdArray as function inputs. NdArrays are converted to data regions of  Variables.
            * @param[in] n_outputs Number of function outputs.
            * @param[in,out] outputs, This can be empty usually. Elements which are not nullptr will be used as in-place outputs.
            */
            NBLA_API vector<NdArrayPtr> execute(FunctionPtr func,
                                                const NdArrays& inputs,
                                                int n_outputs,
                                                NdArrays outputs = {});
        
            NBLA_API void execute(FunctionPtr func,
                                const NdArrays& inputs,
                                const NdArrays& outputs);
        }
    }
}