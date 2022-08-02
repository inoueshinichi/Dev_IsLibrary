#pragma once


#include <IsNdArray/IsNdArray.hpp>
#include <IsNdArray/singleton_manager.hpp>

#include <mutex>

namespace is
{
    namespace nbla
    {
        /**
         * @brief Singleton class for storing global context.
         * 
         */
        class NBLA_API GlobalContext
        {
            Context current_;
            std::mutex mtx_;

        public:
            ~GlobalContext();

            Context get_current_context() const;

            void set_current_context(Context ctx);

        private:
            friend SingletonManager;

            // Never called by users.
            GlobalContext();
            DISABLE_COPY_AND_ASSIGN(GlobalContext)
        };
    }
}