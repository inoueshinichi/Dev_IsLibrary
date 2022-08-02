#pragma once

#include <IsNdArray/array/cpu_array.hpp>

namespace is
{
    namespace nbla
    {
        // cpu_array_wrapperクラステンプレートのcopy関数の内部で使われる.
        template <typename Ta, typename Tb>
        void cpu_array_copy(const Array* src, Array* dst)
        {
            const Ta* p_src = src->const_pointer<Ta>();
            Tb* p_dst = dst->pointer<Tb>();

            if (!src->size())
            {
                // zero-size means scalar
                *p_dst = *p_src;
                return;
            }

            std::copy(p_src, p_src + src->size(), p_dst);
        }


        // fill(float value)関数の中で使われる.
        template <typename T>
        void cpu_fill(Array* self, double value)
        {
            T* ptr = self->pointer<T>();
            std::size_t size = self->size();
            std::fill(ptr, ptr + size, static_cast<T>(value));
        }


        // cpu_array_wrapperクラステンプレートを定義
        NBLA_DEFINE_COPY_WRAPPER(cpu_array_copy)
    }
}