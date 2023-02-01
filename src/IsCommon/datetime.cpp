#include <IsCommon/datetime.hpp>

#include <ctime>

namespace is
{
    namespace common
    {
        std::string get_datetime()
        {
            // 時間取得
            char timeString[256];
            std::memset((void *)&timeString, '\0', sizeof(timeString));
            time_t theTime = time(nullptr);
            struct tm *stm = localtime(&theTime);
            if (theTime == -1 || stm == nullptr)
            {
                std::snprintf(timeString, 256, "Unknown");
            }
            else
            {
                strftime(timeString, 256, "%Y-%m-%d %H:%M", stm);
            }

            return std::string(timeString);
        }
    }
}