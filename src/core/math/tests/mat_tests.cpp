#include "mat_tests.h"

#include <core/math/mat.h>

namespace veer::tests
{

    result mat_constructors()
    {
        const math::vec4f v_zero(0.f);
        const math::mat4x4i zero{};
        const math::mat4x4f zero_2(v_zero, v_zero, v_zero, v_zero);
        const math::mat4x4f i = math::mat4x4f::identity();

        for (size_t r = 0; r < 4u; ++r)
        {
            for (size_t c = 0; c < 4u; ++c)
            {
                VEER_TEST_ASSERT(zero[r][c] == 0u);
                VEER_TEST_ASSERT(zero_2[r][c] == 0.f);
                VEER_TEST_ASSERT(i[r][c] == (r == c ? 1.f : 0.f));
            }
        }

        const math::vec2f a0{1.f, -2.f};
        const math::vec2f a1{-3.f, 4.f};
        const math::mat2x2f a{a0, a1};
        VEER_TEST_ASSERT(math::all(a[0] == a0));
        VEER_TEST_ASSERT(math::all(a[1] == a1));

        const math::mat2x2i b{a};
        VEER_TEST_ASSERT(math::all(b[0] == math::vec2i(a0)));
        VEER_TEST_ASSERT(math::all(b[1] == math::vec2i(a1)));

        // math::mat2x3i does_not_compile_as_it_should{a};
        // math::mat2x3i does_not_compile_as_it_should{a0, a1, a0};
        // math::mat3x2i does_not_compile_as_it_should{a0, a1, a0};

        return result::succeeded;
    }

    result mat_get_set()
    {
        math::vec2f a0{1.f, -2.f};
        math::vec2f a1{-3.f, 4.f};
        math::vec2f a2{-5.f, 6.f};

        math::vec3f b0{1.f, -2.f, -3.f};
        math::vec3f b1{4.f, -5.f, 6.f};

        const math::mat3x2f a(a0, a1, a2);
        const math::mat2x3f b(b0, b1);
        const math::mat4x4f z{};

        const math::mat2x2f a_upper_2x2 = a.get<2u, 2u>();
        const math::mat2x2f b_upper_2x2 = b.get<2u, 2u>();
        VEER_TEST_ASSERT(math::all(a_upper_2x2[0] == a0));
        VEER_TEST_ASSERT(math::all(a_upper_2x2[1] == a1));
        VEER_TEST_ASSERT(math::all(b_upper_2x2[0] == math::vec2f(b0.x(), b0.y())));
        VEER_TEST_ASSERT(math::all(b_upper_2x2[1] == math::vec2f(b1.x(), b1.y())));

        math::mat2x3f c = b;
        c.set(a.get<2u, 2u>());
        VEER_TEST_ASSERT(math::all(c[0] == math::vec3f(a0, b0.z())));
        VEER_TEST_ASSERT(math::all(c[1] == math::vec3f(a1, b1.z())));

        c.set(z.get<2u, 3u>());
        VEER_TEST_ASSERT(math::all(c[0] == math::vec3f(0.f)));
        VEER_TEST_ASSERT(math::all(c[1] == math::vec3f(0.f)));

        c.set(b.get<2u, 1u>());
        VEER_TEST_ASSERT(math::all(c[0] == math::vec3f(b0.x(), 0.f, 0.f)));
        VEER_TEST_ASSERT(math::all(c[1] == math::vec3f(b1.x(), 0.f, 0.f)));
        return result::succeeded;
    }

    result mat_utils()
    {
        const math::mat4x4f t_3d = veer::math::mat_utils::translation_3d(math::vec3f(4.f, 2.f, 0.f));
        const math::mat3x3f t_2d = veer::math::mat_utils::translation_2d(math::vec2f(6.f, 7.f));
        VEER_TEST_ASSERT(math::all(t_3d[0] == math::vec4f(1.f, 0.f, 0.f, 4.f)));
        VEER_TEST_ASSERT(math::all(t_3d[1] == math::vec4f(0.f, 1.f, 0.f, 2.f)));
        VEER_TEST_ASSERT(math::all(t_3d[2] == math::vec4f(0.f, 0.f, 1.f, 0.f)));
        VEER_TEST_ASSERT(math::all(t_3d[3] == math::vec4f(0.f, 0.f, 0.f, 1.f)));
        VEER_TEST_ASSERT(math::all(t_2d[0] == math::vec3f(1.f, 0.f, 6.f)));
        VEER_TEST_ASSERT(math::all(t_2d[1] == math::vec3f(0.f, 1.f, 7.f)));
        VEER_TEST_ASSERT(math::all(t_2d[2] == math::vec3f(0.f, 0.f, 1.f)));

        const float theta = static_cast<float>(math::s_pi) * 0.129f;
        const float cos_v = static_cast<float>(std::cos(theta));
        const float sin_v = static_cast<float>(std::sin(theta));
        const math::mat2x2f r_2d = veer::math::mat_utils::rotation_2d(theta);
        VEER_TEST_ASSERT(math::all(r_2d[0] == math::vec2f(cos_v, -sin_v)));
        VEER_TEST_ASSERT(math::all(r_2d[1] == math::vec2f(sin_v, cos_v)));

        const math::mat3x3f r_3d_x = math::mat_utils::rotation_3d(theta * math::vec3f(1.f, 0.f, 0.f));
        VEER_TEST_ASSERT(math::all(r_3d_x[0] == math::vec3f(1.f, 0.f, 0.f)));
        VEER_TEST_ASSERT(math::all(r_3d_x[1] == math::vec3f(0.f, cos_v, -sin_v)));
        VEER_TEST_ASSERT(math::all(r_3d_x[2] == math::vec3f(0.f, sin_v, cos_v)));

        const math::mat3x3f r_3d_y = math::mat_utils::rotation_3d(theta * math::vec3f(0.f, 1.f, 0.f));
        VEER_TEST_ASSERT(math::all(r_3d_y[0] == math::vec3f(cos_v, 0.f, sin_v)));
        VEER_TEST_ASSERT(math::all(r_3d_y[1] == math::vec3f(0.f, 1.f, 0.f)));
        VEER_TEST_ASSERT(math::all(r_3d_y[2] == math::vec3f(-sin_v, 0.f, cos_v)));

        const math::mat3x3f r_3d_z = math::mat_utils::rotation_3d(theta * math::vec3f(0.f, 0.f, 1.f));
        VEER_TEST_ASSERT(math::all(r_3d_z[0] == math::vec3f(cos_v, -sin_v, 0.f)));
        VEER_TEST_ASSERT(math::all(r_3d_z[1] == math::vec3f(sin_v, cos_v, 0.f)));
        VEER_TEST_ASSERT(math::all(r_3d_z[2] == math::vec3f(0.f, 0.f, 1.f)));

        const math::vec4f d(3.f, 5.f, 8.f, 13.f);
        const math::mat<float, 1u, 1u> s_1d = veer::math::mat_utils::scale(math::vec<float, 1u>(d.x()));
        const math::mat2x2f s_2d = veer::math::mat_utils::scale(math::vec2f(d.x(), d.y()));
        const math::mat3x3f s_3d = veer::math::mat_utils::scale(math::vec3f(d.x(), d.y(), d.z()));
        const math::mat4x4f s_4d = veer::math::mat_utils::scale(math::vec4f(d.x(), d.y(), d.z(), d.w()));

        for (size_t r = 0; r < 4; ++r)
        {
            for (size_t c = 0; c < 4; ++c)
            {
                if (r < 1 && c < 1)
                {
                    VEER_TEST_ASSERT(s_1d[r][c] == ((r == c) ? d[r] : 0.f));
                }

                if (r < 2 && c < 2)
                {
                    VEER_TEST_ASSERT(s_2d[r][c] == ((r == c) ? d[r] : 0.f));
                }

                if (r < 3 && c < 3)
                {
                    VEER_TEST_ASSERT(s_3d[r][c] == ((r == c) ? d[r] : 0.f));
                }

                if (r < 4 && c < 4)
                {
                    VEER_TEST_ASSERT(s_4d[r][c] == ((r == c) ? d[r] : 0.f));
                }
            }
        }

        const math::mat4x2f a(
            math::vec2f(1.f, 2.f), math::vec2f(3.f, 4.f), math::vec2f(5.f, 6.f), math::vec2f(7.f, 8.f)
        );
        const math::mat2x4f a_t = math::mat_utils::transpose(a);
        VEER_TEST_ASSERT(math::all(a_t[0] == math::vec4f(1.f, 3.f, 5.f, 7.f)));
        VEER_TEST_ASSERT(math::all(a_t[1] == math::vec4f(2.f, 4.f, 6.f, 8.f)));
        return result::succeeded;
    }

    result mat_operations()
    {
        using mat2x2b = math::mat<bool, 2u, 2u>;
        VEER_TEST_ASSERT(math::all(mat2x2b(math::vec2b(true, true), math::vec2b(true, true))));
        VEER_TEST_ASSERT(!math::all(mat2x2b(math::vec2b(true, true), math::vec2b(false, true))));
        VEER_TEST_ASSERT(!math::any(mat2x2b(math::vec2b(false, false), math::vec2b(false, false))));
        VEER_TEST_ASSERT(math::any(mat2x2b(math::vec2b(false, true), math::vec2b(false, false))));
        VEER_TEST_ASSERT(
            math::all(
                !mat2x2b(math::vec2b(false, true), math::vec2b(true, false)) ==
                mat2x2b(math::vec2b(true, false), math::vec2b(false, true))
            )
        );
        VEER_TEST_ASSERT(math::all((!math::vec2b(true, false)) == math::vec2b(false, true)));

        math::vec2f lhs_0(-1.f, 0.f);
        math::vec2f lhs_1(0.f, 1.f);
        math::mat2x2f lhs(lhs_0, lhs_1);
        math::vec2f rhs_0(1.f, 0.f);
        math::vec2f rhs_1(0.f, -1.f);
        math::mat2x2f rhs(lhs_0, lhs_1);

        VEER_TEST_ASSERT(math::all((lhs == rhs) == mat2x2b(math::vec2b(false, true), math::vec2b(true, false))));
        VEER_TEST_ASSERT(math::all((lhs != rhs) == mat2x2b(math::vec2b(true, false), math::vec2b(false, true))));
        VEER_TEST_ASSERT(math::all((lhs >= rhs) == mat2x2b(math::vec2b(false, true), math::vec2b(true, true))));
        VEER_TEST_ASSERT(math::all((lhs > rhs) == mat2x2b(math::vec2b(false, false), math::vec2b(false, true))));
        VEER_TEST_ASSERT(math::all((lhs <= rhs) == mat2x2b(math::vec2b(true, true), math::vec2b(true, false))));
        VEER_TEST_ASSERT(math::all((lhs < rhs) == mat2x2b(math::vec2b(true, false), math::vec2b(false, false))));

        math::vec2i a0(3, 1);
        math::vec2i a1(4, 1);
        math::vec2i a2(5, 9);
        const math::mat3x2i a(a0, a1, a2);

        math::vec4i b0(1, 3, 1, 2);
        math::vec4i b1(9, 8, 7, 6);
        const math::mat2x4i b(b0, b1);

        const math::mat2x2i sum = a.get<2, 2>() + b.get<2, 2>();
        VEER_TEST_ASSERT(math::all(sum[0] == math::vec2i(4, 4)));
        VEER_TEST_ASSERT(math::all(sum[1] == math::vec2i(13, 9)));

        const math::vec4i c0(12, 17, 10, 12);
        const math::vec4i c1(13, 20, 11, 14);
        const math::vec4i c2(86, 87, 68, 64);
        const math::mat3x4i c(c0, c1, c2);

        const math::mat3x4i product = a * b;
        VEER_TEST_ASSERT(math::all(product == c));

        const math::mat3x3i c_3x3 = c.get<3, 3>();
        const math::mat3x3i ci = c_3x3 * math::mat3x3i::identity();
        VEER_TEST_ASSERT(math::all(c_3x3 == ci));
        return result::succeeded;
    }

    result mat_all()
    {
        VEER_RUN_TEST(mat_constructors())
        VEER_RUN_TEST(mat_get_set())
        VEER_RUN_TEST(mat_utils())
        VEER_RUN_TEST(mat_operations())
        return result::succeeded;
    }
} // namespace veer::tests