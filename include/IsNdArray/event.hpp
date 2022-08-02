#pragma once

#include <IsNdArray/IsNdArray.hpp>
#include <IsNdArray/common.hpp>
#include <IsNdArray/context.hpp>


#include <memory>

namespace is
{
    namespace nbla
    {
        using std::shared_ptr;

        class NBLA_API Event
        {
        public:
            Event() {}
            virtual ~Event();

            virtual void wait_event(const Context& ctx,
                                    const int async_flags = AsyncFlag::NONE)
            {}
        };

        // エイリアス
        using EventPtr = shared_ptr<Event>;
    }
}