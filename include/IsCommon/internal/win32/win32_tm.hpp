#include <IsCommon/win32/win32_defs.hpp>

namespace is
{
    namespace common
    {
        namespace win32
        {
            /**
             * @brief  Win32 API GetTickCount 精度： 1ms程度
             * 単位: [ms]
             */
            auto invoke_tm_win32_1 = [](auto &&func, auto &&...args) -> double {
                ULONGLONG start = ::GetTickCount64();
                std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...); // 完全転送
                ULONGLONG end = ::GetTickCount64();
                double duration = (double)(end - start);
                std::cout << "Win32 GetTickCount64(): ";
                std::cout << IS_STR_FUNCNAME(func) << ": " << duration << "[ms]" << std::endl;
                return duration;
            };

            /**
             * @brief  Win32 API timeGetTime 精度： 1ms程度
             * 
             */
            auto invoke_tm_win32_2 = [](auto &&func, auto &&...args) -> double {
                DWORD start = ::timeGetTime();
                std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...); // 完全転送
                DWORD end = ::timeGetTime();
                double duration = (double)(end - start);
                std::cout << "Win32 timeGetTime(): ";
                std::cout << IS_STR_FUNCNAME(func) << ": " << duration << "[ms]" << std::endl;
                return duration;
            };

            /**
             * @brief Win32 API QueryPerformanceCounter 精度： 1ns程度
             * 
             */
            auto invoke_tm_win32_3 = [](auto &&func, auto &&...args) -> double {
                LARGE_INTEGER freq;
                if (!(::QueryPerformanceFrequency(&freq))) {
                    std::cout << "Win32 QueryPerformanceCounter(): ";
                    std::cout << "fail to get time unit." << std::endl;
                    return 0;
                }

                LARGE_INTEGER start, end;
                if (!::QueryPerformanceFrequency(&start)) {
                    std::cout << "Win32 QueryPerformanceCounter(): ";
                    std::cout << "fail to start." << std::endl;
                    return 0;
                }

                std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...); // 完全転送

                if (!::QueryPerformanceFrequency(&end)) {
                    std::cout << "Win32 QueryPerformanceCounter(): ";
                    std::cout << "fail to end." << std::endl;
                    return 0;
                }

                double duration = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
                std::cout << "Win32 QueryPerformanceCounter(): ";
                std::cout << IS_STR_FUNCNAME(func) << ": " << duration << "[ms]" << std::endl;
                return duration;
            };
        }
    }
}