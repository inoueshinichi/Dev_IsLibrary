#pragma once
#include <IsNdArray/dtypes.hpp>
#include <IsNdArray/half.hpp>

#include <type_traits>
#include <memory>

namespace is
{
    namespace nbla
    {
//         using std::shared_ptr;
//         using std::make_shared;

//         struct type_from
//         {
//         public:
//             virtual ~type_from_dtype() {};
//         };

//         template <typename T>
//         struct type_from_dtype : public type_from
//         {
//             using type = T;
//         };


//         shared_ptr<type_from> get_type_from_dtype_impl(dtypes dtype)
//         {
// #define NBLA_CASE_TYPE_FROM_DTYPE(type, type_enum)    \
//     case dtypes::type_enum:                           \
//         return make_shared<type_from_dtype<type>>();

//             switch(dtype)
//             {
//                 NBLA_CASE_TYPE_FROM_DTYPE(unsigned char, UBYTE);
// 				NBLA_CASE_TYPE_FROM_DTYPE(char, BYTE);
// 				NBLA_CASE_TYPE_FROM_DTYPE(unsigned short, USHORT);
// 				NBLA_CASE_TYPE_FROM_DTYPE(short, SHORT);
// 				NBLA_CASE_TYPE_FROM_DTYPE(unsigned int, UINT);
// 				NBLA_CASE_TYPE_FROM_DTYPE(int, INT);
// 				NBLA_CASE_TYPE_FROM_DTYPE(unsigned long, ULONG);
// 				NBLA_CASE_TYPE_FROM_DTYPE(long, LONG);
// 				NBLA_CASE_TYPE_FROM_DTYPE(unsigned long long, ULONGLONG);
// 				NBLA_CASE_TYPE_FROM_DTYPE(long long, LONGLONG);
// 				NBLA_CASE_TYPE_FROM_DTYPE(float, FLOAT);
// 				NBLA_CASE_TYPE_FROM_DTYPE(double, DOUBLE);
// 				NBLA_CASE_TYPE_FROM_DTYPE(bool, BOOL);
// 				NBLA_CASE_TYPE_FROM_DTYPE(long double, LONGDOUBLE);
// 				//NBLA_CASE_TYPE_FROM_DTYPE(uint16_t, HALF);

//                 default:
//                     NBLA_ERROR(error_code::type, "Unknown dtype %d", int(dtype));
//             }
// #undef NBLA_CASE_TYPE_FROM_DTYPE
//         }

        // /* bool */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::BOOL>* = nullptr>
        // struct type_from
        // {
        //     using type = bool;
        // };

        // /* char */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::BYTE>* = nullptr>
        // struct type_from
        // {
        //     using type = char;
        // };

        // /* unsigned char */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::UBYTE>* = nullptr>
        // struct type_from
        // {
        //     using type = unsigned char;
        // };

        // /* short */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::SHORT>* = nullptr>
        // struct type_from
        // {
        //     using type = short;
        // };

        // /* unsigned short */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::USHORT>* = nullptr>
        // struct type_from
        // {
        //     using type = unsigned short;
        // };

        // /* int */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::INT>* = nullptr>
        // struct type_from
        // {
        //     using type = int;
        // };
        
        // /* unsigned int */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::UINT>* = nullptr>
        // struct type_from
        // {
        //     using type = unsigned int;
        // };

        // /* long */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::LONG>* = nullptr>
        // struct type_from
        // {
        //     using type = long;
        // };

        // /* unsigned long */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::ULONG>* = nullptr>
        // struct type_from
        // {
        //     using type = unsigned long;
        // };

        // /* long long */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::LONGLONG>* = nullptr>
        // struct type_from
        // {
        //     using type = long long;
        // };

        // /* unsigned long long */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::ULONGLONG>* = nullptr>
        // struct type_from
        // {
        //     using type = unsigned long long;
        // };

        // /* float */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::FLOAT>* = nullptr>
        // struct type_from
        // {
        //     using type = float;
        // };

        // /* double */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::DOUBLE>* = nullptr>
        // struct type_from
        // {
        //     using type = double;
        // };

        // /* long double */
        // template <int DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == (size_t)dtypes::LONGDOUBLE>* = nullptr>
        // struct type_from
        // {
        //     using type = long dobule;
        // };

        // /* half */
        // template <size_t DTYPES_ENUM, typename std::enable_if_t<DTYPES_ENUM == dtypes::HALF>* = nullptr>
        // struct type_from
        // {
        //     using type = Half;
        // };

        // エイリアス
        // template <size_t DTYPES_ENUM>
        // using type_from_t = type_from<DTYPES_ENUM>::type;        
    } // namespace nbla
}