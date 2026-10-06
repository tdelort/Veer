#pragma once

#include <core/concepts.h>
#include <core/core.h>
#include <core/math/vec.h>

namespace veer::math
{
    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    struct mat
    {
    public:
        using value_type = TYPE;
        using type = mat<TYPE, ROW_COUNT, COLUMN_COUNT>;
        using size_type = vec<size_t, 2u>;
        static constexpr size_type m_size = size_type(ROW_COUNT, COLUMN_COUNT);

    private:
        // TODO : inspect memory layout, it might not be the best idea to write it like that
        vec<TYPE, COLUMN_COUNT> m_rows[ROW_COUNT];
        // In case its a problem write it like this :
        // TYPE m_data[COLUMN_COUNT * ROW_COUNT];

    public:
        // I don't really like that this is possible but it's very useful to have a default ctor
        // This will return a fully zeroed out matrix. For identidy, use ::identity()
        constexpr mat();

        // supply the arguments row by row
        template <typename... ARGS>
        constexpr mat(ARGS... _args)
            requires(
                std::conjunction<std::is_same<ARGS, vec<TYPE, COLUMN_COUNT>>...>::value &&
                Equal<sizeof...(ARGS), ROW_COUNT>
            )
        {
            size_t i = 0u;
            ((m_rows[i++] = _args), ...);
        }

        // supply the arguments flat (not a big fan)
        // constexpr mat(std::initializer_list<TYPE> _list)
        // {
        //     static_assert(_list.size() == (ROW_COUNT * COLUMN_COUNT), "Wrong size of initializer list");
        //     size_t i = 0;
        //     for(auto it = _list.begin(); it != _list.end(); ++it)
        //     {
        //         m_rows[i % COLUMN_COUNT][i / ROW_COUNT] = *it;
        //         ++i;
        //     }
        // }

        static constexpr type identity();

        // TODO : add copy/move ctors + cast ctors
        template <Arithmetic OTHER_TYPE>
        constexpr type& operator=(const mat<OTHER_TYPE, ROW_COUNT, COLUMN_COUNT>& _other)
        {
            for (size_t i = 0; i < ROW_COUNT; ++i)
                m_rows[i] = static_cast<vec<OTHER_TYPE, COLUMN_COUNT>>(_other[i]);
            return *this;
        }

        template <Arithmetic OTHER_TYPE>
        constexpr mat(const mat<OTHER_TYPE, ROW_COUNT, COLUMN_COUNT>& _other)
        {
            *this = _other;
        }

        // not constexpr because of the bounds check assert
        [[nodiscard]] const vec<TYPE, COLUMN_COUNT>& operator[](size_t _index) const;
        vec<TYPE, COLUMN_COUNT>& operator[](size_t _index);

        template <size_t OTHER_ROW_COUNT, size_t OTHER_COLUMN_COUNT>
            requires LessEqual<OTHER_ROW_COUNT, ROW_COUNT> && LessEqual<OTHER_COLUMN_COUNT, COLUMN_COUNT>
        mat<TYPE, OTHER_ROW_COUNT, OTHER_COLUMN_COUNT> get() const;

        template <size_t OTHER_ROW_COUNT, size_t OTHER_COLUMN_COUNT>
            requires LessEqual<OTHER_ROW_COUNT, ROW_COUNT> && LessEqual<OTHER_COLUMN_COUNT, COLUMN_COUNT>
        void set(const mat<TYPE, OTHER_ROW_COUNT, OTHER_COLUMN_COUNT>& _m);

        constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT>& operator+=(
            const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs
        );
        constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT>& operator-=(
            const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs
        );

        constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT>& operator*=(TYPE _s);
    };

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT> operator+(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs
    );

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ROW_COUNT> operator-(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs
    );

    // multiply by a ...
    // ... scalar
    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT> operator*(
        TYPE _s, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _m
    );

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<TYPE, ROW_COUNT, COLUMN_COUNT> operator*(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _m, TYPE _s
    );

    // ... vector
    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr vec<TYPE, ROW_COUNT> operator*(
        const vec<TYPE, ROW_COUNT>& _v, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _m
    );

    // ... matrix
    template <Arithmetic TYPE, size_t M, size_t N, size_t P>
    [[nodiscard]] constexpr mat<TYPE, M, P> operator*(const mat<TYPE, M, N>& _lhs, const mat<TYPE, N, P>& _rhs);

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<bool, ROW_COUNT, COLUMN_COUNT> operator==(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs
    );

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<bool, ROW_COUNT, COLUMN_COUNT> operator!=(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs
    );

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<bool, ROW_COUNT, COLUMN_COUNT> operator<(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs
    );

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<bool, ROW_COUNT, COLUMN_COUNT> operator>(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs
    );

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<bool, ROW_COUNT, COLUMN_COUNT> operator<=(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs
    );

    template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<bool, ROW_COUNT, COLUMN_COUNT> operator>=(
        const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _lhs, const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _rhs
    );

    template <size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr mat<bool, ROW_COUNT, COLUMN_COUNT> operator!(const mat<bool, ROW_COUNT, COLUMN_COUNT>& _mat);

    template <size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr bool all(const mat<bool, ROW_COUNT, COLUMN_COUNT>& _mat);

    template <size_t ROW_COUNT, size_t COLUMN_COUNT>
    [[nodiscard]] constexpr bool any(const mat<bool, ROW_COUNT, COLUMN_COUNT>& _mat);


    namespace mat_utils
    {
        template <Arithmetic TYPE, size_t ROW_COUNT, size_t COLUMN_COUNT>
        [[nodiscard]] constexpr mat<TYPE, COLUMN_COUNT, ROW_COUNT> transpose(
            const mat<TYPE, ROW_COUNT, COLUMN_COUNT>& _m
        );

        template <Arithmetic TYPE>
        [[nodiscard]] constexpr mat<TYPE, 2u, 2u> rotation_2d(TYPE _s);

        template <Arithmetic TYPE>
        [[nodiscard]] constexpr mat<TYPE, 3u, 3u> rotation_3d(vec<TYPE, 3u> _euler);
        template <Arithmetic TYPE>
        [[nodiscard]] constexpr mat<TYPE, 3u, 3u> rotation_3d_x(TYPE _s);
        template <Arithmetic TYPE>
        [[nodiscard]] constexpr mat<TYPE, 3u, 3u> rotation_3d_y(TYPE _s);
        template <Arithmetic TYPE>
        [[nodiscard]] constexpr mat<TYPE, 3u, 3u> rotation_3d_z(TYPE _s);

        template <Arithmetic TYPE>
        [[nodiscard]] constexpr mat<TYPE, 3u, 3u> translation_2d(vec<TYPE, 2u> _pos);

        template <Arithmetic TYPE>
        [[nodiscard]] constexpr mat<TYPE, 4u, 4u> translation_3d(vec<TYPE, 3u> _pos);

        template <Arithmetic TYPE, size_t ELEM_COUNT>
        [[nodiscard]] constexpr mat<TYPE, ELEM_COUNT, ELEM_COUNT> scale(vec<TYPE, ELEM_COUNT> _scale);
    }; // namespace mat_utils

    using mat1x1f = mat<float, 1u, 1u>;
    using mat1x2f = mat<float, 1u, 2u>;
    using mat1x3f = mat<float, 1u, 3u>;
    using mat1x4f = mat<float, 1u, 4u>;
    using mat2x1f = mat<float, 2u, 1u>;
    using mat2x2f = mat<float, 2u, 2u>;
    using mat2x3f = mat<float, 2u, 3u>;
    using mat2x4f = mat<float, 2u, 4u>;
    using mat3x1f = mat<float, 3u, 1u>;
    using mat3x2f = mat<float, 3u, 2u>;
    using mat3x3f = mat<float, 3u, 3u>;
    using mat3x4f = mat<float, 3u, 4u>;
    using mat4x1f = mat<float, 4u, 1u>;
    using mat4x2f = mat<float, 4u, 2u>;
    using mat4x3f = mat<float, 4u, 3u>;
    using mat4x4f = mat<float, 4u, 4u>;

    using mat1x1i = mat<int32_t, 1u, 1u>;
    using mat1x2i = mat<int32_t, 1u, 2u>;
    using mat1x3i = mat<int32_t, 1u, 3u>;
    using mat1x4i = mat<int32_t, 1u, 4u>;
    using mat2x1i = mat<int32_t, 2u, 1u>;
    using mat2x2i = mat<int32_t, 2u, 2u>;
    using mat2x3i = mat<int32_t, 2u, 3u>;
    using mat2x4i = mat<int32_t, 2u, 4u>;
    using mat3x1i = mat<int32_t, 3u, 1u>;
    using mat3x2i = mat<int32_t, 3u, 2u>;
    using mat3x3i = mat<int32_t, 3u, 3u>;
    using mat3x4i = mat<int32_t, 3u, 4u>;
    using mat4x1i = mat<int32_t, 4u, 1u>;
    using mat4x2i = mat<int32_t, 4u, 2u>;
    using mat4x3i = mat<int32_t, 4u, 3u>;
    using mat4x4i = mat<int32_t, 4u, 4u>;

    // TODO : investigate up to which point it makes sense to implement vec<T, N> as a mat<T, 1, N> ?
    // because tbf, writing this file it, there was a voice in my head screaming that it would be very pretty, but I
    // don't trust it since its the same voice that told me to avoid implementing a string class because you could have
    // a resizable_array<char> (but I still ended up implementing the string because it actually makes sense)
    // ONE thing that I think could cause problems (or that could actually be very useful depending on how you look at
    // it) is that we could have mat<T, 1, N> and mat<T, N, 1> be different types !!

} // namespace veer::math

#include "mat.hpp"