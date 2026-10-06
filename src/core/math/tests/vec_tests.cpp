#include "vec_tests.h"

#include <core/math/vec.h>

namespace veer::tests
{
    result vec_constructors()
    {
        math::vec4u zero{};
        math::vec4u zero_2{0u};
        math::vec4u ones{1u};
        for (size_t i = 0; i < 4u; ++i)
        {
            VEER_TEST_ASSERT(zero[i] == 0u);
            VEER_TEST_ASSERT(zero_2[i] == 0u);
            VEER_TEST_ASSERT(ones[i] == 1u);
        }

        math::vec2i a{-1, 2};
        VEER_TEST_ASSERT((a[0] == -1) && (a[1] == 2));

        math::vec3i b{a, 3};
        VEER_TEST_ASSERT((b[0] == -1) && (b[1] == 2) && (b[2] == 3));

        // math::vec3i does_not_compile_as_it_should{a, 3, 5};
        // math::vec3i does_not_compile_as_it_should{a};
        // math::vec2i does_not_compile_as_it_should{1, 3, 5};

        math::vec3f c = b;
        VEER_TEST_ASSERT((c[0] == -1.f) && (c[1] == 2.f) && (c[2] == 3.f));

        math::vec3f d(b);
        VEER_TEST_ASSERT((d[0] == -1.f) && (d[1] == 2.f) && (d[2] == 3.f));

        return result::succeeded;
    }

    result vec_swizzle()
    {
        math::vec4f64 v{3.14f, -6.7f, 42.f, 100000.f};
        VEER_TEST_ASSERT(v[0] == v.x());
        VEER_TEST_ASSERT(v[0] == v.r());
        VEER_TEST_ASSERT(v[1] == v.y());
        VEER_TEST_ASSERT(v[1] == v.g());
        VEER_TEST_ASSERT(v[2] == v.z());
        VEER_TEST_ASSERT(v[2] == v.b());
        VEER_TEST_ASSERT(v[3] == v.w());
        VEER_TEST_ASSERT(v[3] == v.a());

        return result::succeeded;
    }

    result vec_operations()
    {
        VEER_TEST_ASSERT(math::all(math::vec4b(true, true, true, true)));
        VEER_TEST_ASSERT(!math::all(math::vec4b(true, true, false, true)));
        VEER_TEST_ASSERT(!math::any(math::vec4b(false, false, false, false)));
        VEER_TEST_ASSERT(math::any(math::vec4b(false, true, false, false)));
        VEER_TEST_ASSERT(math::all((!math::vec2b(true, false)) == math::vec2b(false, true)));

        VEER_TEST_ASSERT(
            math::all((math::vec3f(-1.f, 0.f, 1.f) == math::vec3f(1.f, 0.f, -1.f)) == math::vec3b(false, true, false))
        );
        VEER_TEST_ASSERT(
            math::all((math::vec3f(-1.f, 0.f, 1.f) != math::vec3f(1.f, 0.f, -1.f)) == math::vec3b(true, false, true))
        );
        VEER_TEST_ASSERT(
            math::all((math::vec3f(-1.f, 0.f, 1.f) >= math::vec3f(1.f, 0.f, -1.f)) == math::vec3b(false, true, true))
        );
        VEER_TEST_ASSERT(
            math::all((math::vec3f(-1.f, 0.f, 1.f) > math::vec3f(1.f, 0.f, -1.f)) == math::vec3b(false, false, true))
        );
        VEER_TEST_ASSERT(
            math::all((math::vec3f(-1.f, 0.f, 1.f) <= math::vec3f(1.f, 0.f, -1.f)) == math::vec3b(true, true, false))
        );
        VEER_TEST_ASSERT(
            math::all((math::vec3f(-1.f, 0.f, 1.f) < math::vec3f(1.f, 0.f, -1.f)) == math::vec3b(true, false, false))
        );

        const math::vec3f a{1.f, -3.f, 5.f};
        math::vec3f b;
        math::vec3f c;

        c = a;
        c += 3.f;
        VEER_TEST_ASSERT(math::all(c == math::vec3f(4.f, 0.f, 8.f)));

        c = a;
        c -= 3.f;
        VEER_TEST_ASSERT(math::all(c == math::vec3f(-2.f, -6.f, 2.f)));

        c = a;
        c *= 2.f;
        VEER_TEST_ASSERT(math::all(c == math::vec3f(2.f, -6.f, 10.f)));

        c = a;
        c /= 2.f;
        VEER_TEST_ASSERT(math::all(c == math::vec3f(0.5f, -1.5f, 2.5f)));

        VEER_TEST_ASSERT(math::all((a + a) == math::vec3f(2.f, -6.f, 10.f)));
        VEER_TEST_ASSERT(math::all((a - a) == math::vec3f(0.f, 0.f, 0.f)));
        VEER_TEST_ASSERT(math::all((a * a) == math::vec3f(1.f, 9.f, 25.f)));
        VEER_TEST_ASSERT(math::all((a / a) == math::vec3f(1.f, 1.f, 1.f)));
        VEER_TEST_ASSERT(math::all((a + math::vec3f(-5.f, 3.f, 1.f)) == math::vec3f(-4.f, 0.f, 6.f)));
        VEER_TEST_ASSERT(math::all((a - math::vec3f(-5.f, 3.f, 1.f)) == math::vec3f(6.f, -6.f, 4.f)));
        VEER_TEST_ASSERT(math::all((a * math::vec3f(-5.f, 3.f, 1.f)) == math::vec3f(-5.f, -9.f, 5.f)));
        VEER_TEST_ASSERT(math::all((a / math::vec3f(-5.f, 3.f, 1.f)) == math::vec3f(-0.2f, -1.f, 5.f)));
        VEER_TEST_ASSERT(math::all((a * 3.f) == math::vec3f(3.f, -9.f, 15.f)));
        VEER_TEST_ASSERT(math::all((3.f * a) == math::vec3f(3.f, -9.f, 15.f)));

        VEER_TEST_ASSERT(math::all((-a) == math::vec3f(-1.f, 3.f, -5.f)));

        VEER_TEST_ASSERT(math::sq_length(math::vec3f(1.f, 2.f, 3.f)) == (1.f + 4.f + 9.f));
        const float l = math::length(math::normalize(a));
        VEER_TEST_ASSERT((l <= 1.001f) && (l >= 0.999f));
        VEER_TEST_ASSERT(math::length(math::normalize_safe(math::vec4f(0.f))) == 0.f);

        VEER_TEST_ASSERT(
            math::all(math::cross(math::vec3f(1.f, 0.f, 0.f), math::vec3f(0.f, 1.f, 0.f)) == math::vec3f(0.f, 0.f, 1.f))
        );

        VEER_TEST_ASSERT(math::dot(math::vec3f(1.f, 0.f, 0.f), math::vec3f(0.f, 1.f, 0.f)) == 0.f);

        return result::succeeded;
    }

    result vec_all()
    {
        VEER_RUN_TEST(vec_constructors())
        VEER_RUN_TEST(vec_swizzle())
        VEER_RUN_TEST(vec_operations())
        return result::succeeded;
    }
} // namespace veer::tests