#include <IsComputerVision/common.hpp> 

#include <iostream>
#include <string>
#include <cstdio>
#include <iomanip>
#include <cstring>

#define TEST_INPUT_COLOR_DIR "/home/inoue/Images/Color/"
#define TEST_INPUT_MONO_256_DIR "/home/inoue/Images/Mono/256X256/"
#define TEST_INPUT_MONO_512_DIR "/home/inoue/Images/Mono/512X512/"
#define TEST_OUTPUT_DIR "/home/inoue/Desktop/"


using namespace is::nbla;

auto property = [](const std::string& test_name, auto&& func, auto&&... args) {
    std::cout << "[ENTER] " << test_name << "---------------" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    std::forward<decltype(func)>(func)(std::forward<args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = end - start;
    double duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    std::cout << duration_ms << " [ms]" << std::endl;

    std::cout << "[EXIT] " << test_name << "---------------" << std::endl;
    std::cout << std::endl;
};


void show_status_memory(Memory *memory) 
{
    std::cout << "memory: " << std::hex << memory << std::dec << std::endl;
    std::cout << "bytes: " << memory->bytes() << std::endl;
    std::cout << "device_id: " << memory->device_id() << std::endl;
    std::cout << "ptr: " << std::hex << memory->pointer() << std::dec << std::endl;
    std::cout << "locked: " << std::boolalpha << memory->locked() << std::endl;
    std::cout << "disabled: " << std::boolalpha << memory->disabled() << std::endl;
    std::cout << "next: " << std::hex << memory->next() << std::dec << std::endl;
    std::cout << "prev: " << std::hex << memory->prev() << std::dec << std::endl;
    std::cout << "active bytes: " << memory->bytes_active() << std::endl;
    std::cout << "---------------------------" << std::endl;
}

void show_ndarray_property(NdArrayPtr ndarray)
{
    int ndim = ndarray->ndim();
    auto sh = ndarray->shape();
    auto st = ndarray->strides();
    auto dtype = ndarray->dtype();
    auto dtype_str = dtype_to_string(dtype);
    if (ndim == 1)
    {
        std::printf("type=%s\n", dtype_str.c_str());
        std::printf("ndim=%d\n", ndim);
        std::printf("shape(%lld,)\n", sh[0]);
        std::printf("strides(%d,)\n", st[0]);
    }
    else if (ndim == 2)
    {
        std::printf("type=%s\n", dtype_str.c_str());
        std::printf("ndim=%d\n", ndim);
        std::printf("shape(%lld,%lld)\n", sh[0], sh[1]);
        std::printf("strides(%d,%d)\n", st[0], st[1]);
    }
    else if (ndim == 3)
    {
        std::printf("type=%s\n", dtype_str.c_str());
        std::printf("ndim=%d\n", ndim);
        std::printf("shape(%lld,%lld,%lld)\n", sh[0], sh[1], sh[2]);
        std::printf("strides(%d,%d,%d)\n", st[0], st[1], st[2]);
    }
    else if (ndim == 4)
    {
        std::printf("type=%s\n", dtype_str.c_str());
        std::printf("ndim=%d\n", ndim);
        std::printf("shape(%lld,%lld,%lld,%lld)\n", sh[0], sh[1], sh[2], sh[3]);
        std::printf("strides(%d,%d,%d,%d)\n", st[0], st[1], st[2], st[3]);
    }
    else if (ndim == 5)
    {
        std::printf("type=%s\n", dtype_str.c_str());
        std::printf("ndim=%d\n", ndim);
        std::printf("shape(%lld,%lld,%lld,%lld,%lld)\n", sh[0], sh[1], sh[2], sh[3], sh[4]);
        std::printf("strides(%d,%d,%d,%d,%d)\n", st[0], st[1], st[2], st[3], st[4]);
    }
    else
    {
        std::ostringstream oss;
        oss << "Error: ndim > 5. Given is " << ndim << ".";
        throw std::runtime_error(oss.str());
    }
}

template <typename T>
void show_ndarray_contents(NdArrayPtr ndarray)
{
    const auto &ctx = SingletonManager::get<GlobalContext>()->get_current_context();
    Shape_t shape = ndarray->shape();
    Stride_t strides = ndarray->strides();
    Size_t ndim = ndarray->ndim();
    const T *data = ndarray->cast_data_and_get_pointer<T>(ctx);
    auto dtype = get_dtype<T>();

    if (ndim == 1)
    {
        std::cout << "[";
        for (int i = 0; i < shape[0]; ++i)
        {
            if (dtype == dtypes::BYTE || dtype == dtypes::UBYTE)
            {
                std::cout << +data[i * strides[0]];
            }
            else
            {
                std::cout << data[i * strides[0]];
            }
            if (i != shape[0] - 1)
                    std::cout << " ";
        }
        std::cout << "]" << std::endl;
    }
    else if (ndim ==2)
    {
        std::cout << "[";
        for (int i = 0; i < shape[0]; ++i)
        {
            std::cout << "[";
            for (int j = 0; j < shape[1]; ++j)
            {
                if (dtype == dtypes::BYTE || dtype == dtypes::UBYTE)
                {
                    std::cout << +data[i * strides[0] + j * strides[1]];
                }
                else
                {
                    std::cout << data[i * strides[0] + j * strides[1]];
                }
                if (j != shape[1] - 1)
                    std::cout << "  ";
            }
            std::cout << "]";

            if (i != shape[0] - 1)
                std::cout << std::endl;
        }
        std::cout << "]" << std::endl;
    }
    else if (ndim == 3)
    {
        std::cout << "[";
        for (int c = 0; c < shape[0]; ++c)
        {
            std::cout << "[";
            for (int j = 0; j < shape[1]; ++j)
            {
                if (j != 0)
                    std::cout << "  ";
                std::cout << "[";
                for (int i = 0; i < shape[2]; ++i)
                {
                    if (dtype == dtypes::BYTE || dtype == dtypes::UBYTE)
                    {
                        std::cout << +data[c * strides[0] + j * strides[1] + i * strides[2]];
                    }
                    else
                    {
                        std::cout << data[c * strides[0] + j * strides[1] + i * strides[2]];
                    }  
                    if (i != shape[2] - 1)
                        std::cout << " ";
                }

                if (j == shape[1] - 1)
                {
                    if (c != shape[0] - 1)
                        std::cout << "]]" << std::endl;
                    else
                        std::cout << "]]";
                }
                else
                    std::cout << "]" << std::endl;
            }
        }
        std::cout << "]" << std::endl;
    }
    else if (ndim == 4)
    {
        std::cout << "[";
        for (int n = 0; n < shape[0]; ++n)
        {
            if (n != 0)
                std::cout << std::endl;
            std::cout << "[";
            for (int c = 0; c < shape[1]; ++c)
            {
                if (c != 0)
                    std::cout << " ";
                std::cout << "[";
                for (int j = 0; j < shape[2]; ++j)
                {
                    if (j != 0)
                        std::cout << "  ";
                    std::cout << "[";
                    for (int i = 0; i < shape[3]; ++i)
                    {
                        if (dtype == dtypes::BYTE || dtype == dtypes::UBYTE)
                        {
                            std::cout << +data[n * strides[0] + c * strides[1] + j * strides[2] + i * strides[3]];
                        }
                        else
                        {
                            std::cout << data[n * strides[0] + c * strides[1] + j * strides[2] + i * strides[3]];
                        }
                        if (i != shape[3] - 1)
                            std::cout << " ";
                    }

                    if (j == shape[2] - 1)
                    {
                        if (c != shape[1] - 1)
                            std::cout << "]]" << std::endl;
                        else
                            std::cout << "]]";
                    }
                    else
                        std::cout << "]" << std::endl;
                }
            }
            std::cout << "]";
        }
        std::cout << "]" << std::endl;
    }
    else
    {
        std::string err_status = format_string("Not implimentation. func: %s, line: %d, file: %s", __func__, __LINE__, __FILE__);
        throw std::runtime_error(err_status);
    }
}
