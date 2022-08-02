#pragma once

#include <IsNdArray/exception.hpp>

namespace is {
    namespace nbla {
        // 前方宣言
		struct Half;

        // Enum of dtypes
        // numpyのdtypeと同じ発想
        enum class dtypes {
			BOOL = 0,
			BYTE,
			UBYTE,
			SHORT,
			USHORT,
			INT,
			UINT,
			LONG,
			ULONG,
			LONGLONG,
			ULONGLONG,
			FLOAT,
			DOUBLE,
			LONGDOUBLE,
			// 以下はnumpy互換
			// CFLOAT,
			// CDOUBLE,
			// CLONGDOUBLE,
			// BOJECT = 17,
			// STRING,
			// UNICODE,
			// VOID,
			// Appended in numpy 1.6
			// DATETIME,
			// TIMEDELTA
			HALF = 23,
			// NTYPES,
			// NOTYPE,
			// CHAR,
			// USERDEF = 256,
			// NTYPES_ABI_COMPATIBLE = 21
		};


        /**
         * @brief Get the dtype object
         * プライマリーテンプレート
         * i.e. dtypes dtype = get_dtype<T>::type;
         * 
         * @tparam T 
         * @return dtypes 
         */
        template <typename T>
        dtypes get_dtype() {
            NBLA_ERROR(error_code::type, "Unsupported dtype.");
        }


// 明示的特殊化のマクロ
#define GET_DTYPE_TEMPLATE_SPECIAL(type, Dtype)                          \
    template <> inline                                                   \
	dtypes get_dtype<type>() { return dtypes::Dtype; }

	GET_DTYPE_TEMPLATE_SPECIAL(bool, BOOL)
	GET_DTYPE_TEMPLATE_SPECIAL(unsigned char, UBYTE)
	GET_DTYPE_TEMPLATE_SPECIAL(char, BYTE)
	GET_DTYPE_TEMPLATE_SPECIAL(unsigned short, USHORT)
	GET_DTYPE_TEMPLATE_SPECIAL(short, SHORT)
	GET_DTYPE_TEMPLATE_SPECIAL(unsigned int, UINT)
	GET_DTYPE_TEMPLATE_SPECIAL(int, INT)
	GET_DTYPE_TEMPLATE_SPECIAL(unsigned long, ULONG)
	GET_DTYPE_TEMPLATE_SPECIAL(long, LONG)
	GET_DTYPE_TEMPLATE_SPECIAL(unsigned long long, ULONGLONG)
	GET_DTYPE_TEMPLATE_SPECIAL(long long, LONGLONG)
	GET_DTYPE_TEMPLATE_SPECIAL(float, FLOAT)
	GET_DTYPE_TEMPLATE_SPECIAL(double, DOUBLE)
	GET_DTYPE_TEMPLATE_SPECIAL(long double, LONGDOUBLE)
	GET_DTYPE_TEMPLATE_SPECIAL(is::nbla::Half, HALF)

#undef GET_DTYPE_TEMPLATE_SPECIAL


        /**
         * @brief dtypesをstringに変換する.
         * 
         * @param dtype 
         * @return string 
         */
        inline string dtype_to_string(dtypes dtype)
		{
#define GET_DTYPE_STRING(type)                           \
	case dtypes::type:                                   \
		s = #type;                                       \
		break;

			string s;
			switch (dtype)
			{
				GET_DTYPE_STRING(UBYTE);
				GET_DTYPE_STRING(BYTE);
				GET_DTYPE_STRING(USHORT);
				GET_DTYPE_STRING(SHORT);
				GET_DTYPE_STRING(UINT);
				GET_DTYPE_STRING(INT);
				GET_DTYPE_STRING(ULONG);
				GET_DTYPE_STRING(LONG);
				GET_DTYPE_STRING(ULONGLONG);
				GET_DTYPE_STRING(LONGLONG);
				GET_DTYPE_STRING(FLOAT);
				GET_DTYPE_STRING(DOUBLE);
				GET_DTYPE_STRING(BOOL);
				GET_DTYPE_STRING(LONGDOUBLE);
				GET_DTYPE_STRING(HALF);
			}

			if (s.empty())
			{
				NBLA_ERROR(error_code::type, "Unknown dtype %d", int(dtype));
			}

			return s;
#undef GET_DTYPE_STRING
		}


        /**
         * @brief dtypeのサイズを取得する.
         * 
         * @param dtype 
         * @return size_t 
         */
        inline size_t sizeof_dtype(dtypes dtype)
		{
#define GET_DTYPE_SIZE(type, TYPE)                                \
	case dtypes::TYPE:                                            \
		s = sizeof(type);                                         \
		break;

			size_t s = 0;
			switch (dtype)
			{
				GET_DTYPE_SIZE(unsigned char, UBYTE);
				GET_DTYPE_SIZE(char, BYTE);
				GET_DTYPE_SIZE(unsigned short, USHORT);
				GET_DTYPE_SIZE(short, SHORT);
				GET_DTYPE_SIZE(unsigned int, UINT);
				GET_DTYPE_SIZE(int, INT);
				GET_DTYPE_SIZE(unsigned long, ULONG);
				GET_DTYPE_SIZE(long, LONG);
				GET_DTYPE_SIZE(unsigned long long, ULONGLONG);
				GET_DTYPE_SIZE(long long, LONGLONG);
				GET_DTYPE_SIZE(float, FLOAT);
				GET_DTYPE_SIZE(double, DOUBLE);
				GET_DTYPE_SIZE(bool, BOOL);
				GET_DTYPE_SIZE(long double, LONGDOUBLE);
				GET_DTYPE_SIZE(uint16_t, HALF);
			}

			if (s == 0) 
			{
				NBLA_ERROR(error_code::type, "Unsupported type: %s",
					       dtype_to_string(dtype).c_str());
			}

			return s;

#undef GET_DTYPE_SIZE
		}
    }
}