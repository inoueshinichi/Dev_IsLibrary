#include <IsCommon/math.hpp>

namespace is
{
    namespace common
    {
        namespace math
        {

        }

        const Vector2 Vector2::zero(0.0f, 0.0f);
        const Vector2 Vector2::unit_x(1.0f, 0.0f);
        const Vector2 Vector2::unit_y(0.0f, 1.0f);
        const Vector2 Vector2::ng_unit_x(-1.0f, 0.0f);
        const Vector2 Vector2::ng_unit_y(0.0f, -1.0f);
        const Vector2 Vector2::inf(math::inf, math::inf);
        const Vector2 Vector2::ng_inf(math::ng_inf, math::ng_inf);

        const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
        const Vector3 Vector3::unit_x(1.0f, 0.0f, 0.0f);
        const Vector3 Vector3::unit_y(0.0f, 1.0f, 0.0f);
        const Vector3 Vector3::unit_z(0.0f, 0.0f, 1.0f);
        const Vector3 Vector3::ng_unit_x(1.0f, 0.0f, 0.0f);
        const Vector3 Vector3::ng_unit_y(0.0f, 1.0f, 0.0f);
        const Vector3 Vector3::ng_unit_z(0.0f, 0.0f, 1.0f);
        const Vector3 Vector3::inf(math::inf, math::inf, math::inf);
        const Vector3 Vector3::ng_inf(math::ng_inf, math::ng_inf, math::ng_inf);
        ;
    }
}