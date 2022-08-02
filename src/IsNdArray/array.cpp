#include <IsNdArray/array.hpp>
#include <IsNdArray/common.hpp>
#include <IsNdArray/exception.hpp>

#include <vector>

namespace is
{
    namespace nbla
    {
        using std::vector;

        Array::Array(const Size_t size, dtypes dtype, const Context& ctx, AllocatorMemory&& mem)
            : size_(size), dtype_(dtype), ctx_(ctx), mem_(std::move(mem))
        {}


        Array::~Array() { wait_event(ctx_); }


        std::size_t Array::size_as_bytes(Size_t size, dtypes dtype)
        {
            return size * sizeof_dtype(dtype);
        }


        Context Array::filter_context(const Context& ctx)
        {
            NBLA_ERROR(error_code::not_implemented, 
                       "Array must implement filter_context(const Context&).");
        }


        void Array::set_event(EventPtr e) { event_ = e; }


        void Array::wait_event(const Context ctx, const int async_flags)
        {
            if (event_)
            {
                event_->wait_event(ctx, async_flags);
                event_.reset();
            }
        }


        bool Array::have_event() { return event_ != nullptr; }


        Array::Ptr Array::getptr() { return shared_from_this(); }
    }
}