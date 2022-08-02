#include <IsNdArray/global_context.hpp>
#include <IsNdArray/singleton_manager-internal.hpp>

namespace is
{
    namespace nbla
    {
        GlobalContext::GlobalContext()
            : current_(Context({"cpu:float"}, "CpuArray", "0")) {}

        GlobalContext::~GlobalContext() {}

        Context GlobalContext::get_current_context() const { return current_; }

        void GlobalContext::set_current_context(const Context ctx)
        {
            std::lock_guard<std::mutex> lock(mtx_);
            current_ = ctx;
        }

        NBLA_INSTANTIATE_SINGLETON(NBLA_API, GlobalContext)
    }
}