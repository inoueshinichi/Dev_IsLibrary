#pragma once

#ifdef _MSC_VER
    // https://qiita.com/Chironian/items/462a3bdf271d5f0b00b6#%EF%BC%92%EF%BC%93c4251%E8%AD%A6%E5%91%8A%E3%81%8C%E5%87%BA%E3%82%8B
    #pragma warning(disable:4251)
#endif

#if defined(_MSC_VER) && !defined(__CUDACC__)
    #if defined(IsComputerVision_EXPORTS) || defined(IsComputerVision_dbg_EXPORTS)
        #define IS_CV_API __declspec(dllexport)
    #else
        #define IS_CV_API __declspec(dllimport)
    #endif
#else
    #define IS_CV_API
#endif


//////////////////////////////////////////////////////////////////////////////////
// Std headers
//////////////////////////////////////////////////////////////////////////////////
#include <stdexcept>
#include <exception>
#include <cassert>

#include <limits>
#include <numeric>

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>

#include <algorithm>

// C++11 gives alignas as standard
// http://en.cppreference.com/w/cpp/language/alignas
#define IS_IMGPROC_ALIGN(N) alignas(N)

#if _MSC_VER
  #define _USE_MATH_DEFINES
#endif
#include <cmath>

    // Memory
#include <cstring>
#define IS_ZERO_MEMORY(ptr, bytesize) std::memset((void *)ptr, 0, bytesize)

    // Log
#if (0)
  #define IS_DEBUG_FLAG 0
#else
  #include <cstdio>
  #define IS_DEBUG_FLAG 1
#endif

#include <tuple>
#include <vector>
#include <deque>
#include <list>

//////////////////////////////////////////////////////////////////////////////////
// IsCommon headers
//////////////////////////////////////////////////////////////////////////////////
#include <IsCommon/format_string.hpp>
#include <IsCommon/split_string.hpp>
#include <IsCommon/join_string.hpp>
#include <IsCommon/IsCommon.hpp>

#define IS_DEBUG_CHECK_NDARRAY_STATE(func, flag, ndarray) \
  if (flag > 0) {                                         \
    auto ndim = ndarray->ndim();                          \
    auto shape = ndarray->shape();                        \
    auto strides = ndarray->strides();                    \
    auto dtype = ndarray->dtype();                        \
    auto type = nbla::dtype_to_string(dtype);             \
    std::ostringstream oss;                               \
    oss << #func << ", ";                                 \
    oss << "Type: " << type << ", ";                      \
    oss << "ndim: " << ndim << ", ";                      \
    oss << "shape: (";                                    \
    for (int d = 0; d < ndim; ++d) {                      \
      oss << shape[d];                                    \
      if (d != ndim - 1) {                                \
        oss << ",";                                       \
      } else {                                            \
        oss << "), ";                                     \
      }                                                   \
    }                                                     \
                                                          \
    oss << "strides: (";                                  \
    for (int d = 0; d < ndim; ++d) {                      \
      oss << strides[d];                                  \
      if (d != ndim - 1) {                                \
        oss << ",";                                       \
      } else {                                            \
        oss << ")";                                       \
      }                                                   \
    }                                                     \
    std::cout << oss.str() << std::endl;                  \
  }


#define IS_CHECK_NDARRAY_SHAPE_AS_IMAGE(ndarray)                                 \
  {                                                                              \
    if (!ndarray.get()) {                                                        \
      throw std::runtime_error("NdArray is nullptr.");                           \
    }                                                                            \
    auto ndim = ndarray->ndim();                                                 \
    auto shape = ndarray->shape();                                               \
    auto strides = ndarray->strides();                                           \
                                                                                 \
    if (ndim != 3) {                                                             \
      throw std::runtime_error(                                                  \
          is::common::format_string("Miss matching 3 ndim. Given is %d", ndim)); \
    } else {                                                                     \
      /*(C, H, W)*/                                                              \
      int channels = shape[0];                                                   \
      if (channels > 3 || channels == 2) {                                                        \
        throw std::runtime_error(is::common::format_string(                      \
            "Miss matching channels of top shape (<=3 or ==2). Given is %d",            \
            channels));                                                          \
      }                                                                          \
    }                                                                            \
  }



    //////////////////////////////////////////////////////////////////////////////////
    // IsNdArray headers
    //////////////////////////////////////////////////////////////////////////////////
/*Context*/
#include <IsNdArray/global_context.hpp>
#include <IsNdArray/singleton_manager.hpp>

/*NdArray*/
#include <IsNdArray/nd_array.hpp>
#include <IsNdArray/nd_array_extra.hpp>

//////////////////////////////////////////////////////////////////////////////////
// SaturateClamp headers
//////////////////////////////////////////////////////////////////////////////////
namespace is
{
  namespace imgproc
  {
    using namespace nbla;

    /**
     * @brief saturate_clamp for NdArray
     *
     * @tparam T
     * @param src
     * @return NdArrayPtr
     */
    template <typename T>
    NdArrayPtr saturate_clamp(NdArrayPtr src)
    {
      const auto &ctx =
          SingletonManager::get<GlobalContext>()->get_current_context();

      #define IS_SATURATE_CLAMP(dtype, type)                      \
        case dtypes::dtype:                                       \
        {                                                         \
          type *data = src->cast_data_and_get_pointer<type>(ctx); \
          for (size_t k = 0; k < src->size(); ++k)                \
          {                                                       \
            data[k] = (type)saturate_clamp<T>(data[k]);           \
          }                                                       \
        }                                                         \
        break

      auto dtype = src -> dtype();
      switch (dtype)
      {
        IS_SATURATE_CLAMP(UBYTE, unsigned char);
        IS_SATURATE_CLAMP(BYTE, char);
        IS_SATURATE_CLAMP(USHORT, unsigned short);
        IS_SATURATE_CLAMP(SHORT, short);
        IS_SATURATE_CLAMP(UINT, unsigned int);
        IS_SATURATE_CLAMP(INT, int);
        IS_SATURATE_CLAMP(ULONG, unsigned long);
        IS_SATURATE_CLAMP(LONG, long);
        IS_SATURATE_CLAMP(ULONGLONG, unsigned long long);
        IS_SATURATE_CLAMP(LONGLONG, long long);
        IS_SATURATE_CLAMP(FLOAT, float);
        IS_SATURATE_CLAMP(DOUBLE, double);
        IS_SATURATE_CLAMP(BOOL, bool);
        IS_SATURATE_CLAMP(LONGDOUBLE, long double);

        case dtypes::HALF:
        {
          Half *data = src->cast_data_and_get_pointer<Half>(ctx);
          for (size_t k = 0; k < src->size(); ++k)
          {
            data[k] = (Half)saturate_clamp<T>((float)data[k]);
          }
        }
        break;
      }

      return src;
  #undef IS_SATURATE_CLAMP
    }
  }
}

#include <IsComputerVision/saturate_clamp.hpp>

//////////////////////////////////////////////////////////////////////////////////
// Type headers
//////////////////////////////////////////////////////////////////////////////////
#include <IsComputerVision/type.hpp>
