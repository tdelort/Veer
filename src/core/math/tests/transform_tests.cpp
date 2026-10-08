#include "transform_tests.h"

#include <core/math/vec.h>
#include <core/math/transform.h>

namespace veer::tests
{
    result transform_constructors()
    {
        const math::vec3f pos_3d(4.f, 2.f, 1.f);

        math::transform identity{};
        const math::mat4x4f mat = identity.generate_matrix();
        const math::mat4x4f inv_mat = identity.generate_matrix();
        const math::vec4f p0 = (math::vec4f(pos_3d, 1.f) * mat);
        const math::vec4f p1 = (math::vec4f(pos_3d, 1.f) * inv_mat);
        VEER_TEST_ASSERT(all(p0.get<3>() == pos_3d));
        VEER_TEST_ASSERT(all(p1.get<3>() == pos_3d));

        const math::vec3f translation = math::vec3f(-1.f, 5.f, 12.f);
        const math::vec3f rotation = static_cast<float>(math::s_pi) * math::vec3f(-0.2f, 1.7f, 0.08f);
        const math::vec3f scale = math::vec3f(1.f, 2.3f, 2.f);
        math::transform a{translation, rotation, scale};
        VEER_TEST_ASSERT(all(a.get_translation() == translation));
        VEER_TEST_ASSERT(all(a.get_rotation() == rotation));
        VEER_TEST_ASSERT(all(a.get_scale() == scale));
        return result::succeeded;
    }

    result transform_operations()
    {
        const math::vec2f pos_2d(1.f, 2.f);
        const math::vec3f pos_3d(1.f, 2.f, 3.f);

        const math::vec2f v_t_2d = math::vec2f(6.f, 7.f);
        const math::vec3f v_t_3d = math::vec3f(4.f, 2.f, 0.f);
        const math::mat3x3f m_t_2d = math::transform::s_translation_2d(math::vec2f(6.f, 7.f));
        const math::mat4x4f m_t_3d = math::transform::s_translation_3d(math::vec3f(4.f, 2.f, 0.f));
        VEER_TEST_ASSERT(all((math::vec3f(pos_2d, 1.f) * m_t_2d).get<2u>() == (pos_2d + v_t_2d)));
        VEER_TEST_ASSERT(all((math::vec4f(pos_3d, 1.f) * m_t_3d).get<3u>() == (pos_3d + v_t_3d)));

        const float theta = static_cast<float>(math::s_pi) * 0.5f;
        const float cos_v = static_cast<float>(std::cos(theta));
        const float sin_v = static_cast<float>(std::sin(theta));
        const math::mat2x2f r_2d = math::transform::s_rotation_2d(theta);
        VEER_TEST_ASSERT(all(math::nearly_equal(pos_2d * r_2d, math::vec2f(-2.f, 1.f))));
        VEER_TEST_ASSERT(all(math::nearly_equal(pos_2d * r_2d * r_2d * r_2d * r_2d, pos_2d)));

        const math::mat3x3f r_3d_x = math::transform::s_rotation_3d_x(theta);
        const math::mat3x3f r_3d_y = math::transform::s_rotation_3d_y(theta);
        const math::mat3x3f r_3d_z = math::transform::s_rotation_3d_z(theta);
        VEER_TEST_ASSERT(all(math::nearly_equal(pos_3d * r_3d_x, math::vec3f(1.f, -3.f, 2.f)))); // x unchanged
        VEER_TEST_ASSERT(all(math::nearly_equal(pos_3d * r_3d_y, math::vec3f(3.f, 2.f, -1.f)))); // y unchanged
        VEER_TEST_ASSERT(all(math::nearly_equal(pos_3d * r_3d_z, math::vec3f(-2.f, 1.f, 3.f)))); // z unchanged

        const math::vec3f d(3.f, 5.f, 8.f);
        const math::mat2x2f s_2d = math::transform::s_scale(math::vec2f(d.x(), d.y()));
        const math::mat3x3f s_3d = math::transform::s_scale(math::vec3f(d.x(), d.y(), d.z()));
        VEER_TEST_ASSERT(all(math::nearly_equal(pos_2d * s_2d, (pos_2d * d.get<2u>()))));
        VEER_TEST_ASSERT(all(math::nearly_equal(pos_3d * s_3d, (pos_3d * d))));


        const math::vec3f translation = math::vec3f(-1.f, 5.f, 12.f);
        const math::vec3f rotation = static_cast<float>(math::s_pi) * math::vec3f(-0.2f, 1.7f, 0.08f);
        const math::vec3f scale = math::vec3f(1.f, 2.3f, 2.f);
        math::transform a{translation, rotation, scale};
        const math::mat4x4f mat = a.generate_matrix();
        const math::mat4x4f inv_mat = a.generate_inverse_matrix();
        VEER_TEST_ASSERT(all(math::nearly_equal(mat * inv_mat, math::mat4x4f::identity())));

        return result::succeeded;
    }

    result transform_all()
    {
        VEER_RUN_TEST(transform_constructors())
        VEER_RUN_TEST(transform_operations())
        return result::succeeded;
    }
}