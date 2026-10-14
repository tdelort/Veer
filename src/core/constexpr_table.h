#pragma once

#include <core/containers/static_array.h>
#include <core/core.h>
#include <core/math/math.h>

namespace veer
{
    // I could upgrade to C++26, or I could use this ugly thing :3
    // The constraint here is that all member of the pack must be of the same type
    // template <typename T, T... PACK>
    // struct pack_indexing
    // {
    //     template <size_t INDEX>
    //     static consteval T get()
    //     {
    //         // I swear, I tried with compiler explorer and this is optimized
    //         // EVEN with -O0 and we are left with only the good path
    //         size_t i = 0ull;
    //         T accum{};
    //         ((accum = (i == INDEX ? PACK : accum), ++i), ...);
    //         return accum;
    //     }
    // };

    template <typename T, T... VALUES>
    struct get_max
    {
        static constexpr T value = math::max<T>(VALUES...);
    };
    template <typename T, T... VALUES>
    constexpr T get_max_v = get_max<T, VALUES...>::value;

    template <typename T, T... VALUES>
    struct is_contiguous
    {
        static consteval bool compute()
        {
            constexpr size_t array_size = static_cast<size_t>(get_max_v<T, VALUES...>) + 1u;
            containers::static_array<bool, array_size> array{};
            for (size_t i = 0; i < array.size(); ++i)
                array[i] = false;

            ((array[static_cast<size_t>(VALUES)] = true), ...);

            bool contiguous = true;
            for (size_t i = 0; i < array.size(); ++i)
                contiguous = contiguous && array[i];
            return contiguous;
        }

        static constexpr bool value = compute();
    };

    template <typename T, T... VALUES>
    constexpr bool is_contiguous_v = is_contiguous<T, VALUES...>::value;

    template <typename TABLE_BUILDER>
        requires requires {
            { TABLE_BUILDER::build() } -> std::same_as<typename TABLE_BUILDER::table_type>;
        }
    struct constexpr_table
    {
        using builder_type = TABLE_BUILDER;
        static constexpr size_t size = builder_type::table_size;

        template <size_t INDEX>
        static consteval const builder_type::value_type& get()
        {
            return m_data[INDEX];
        }

        static constexpr const builder_type::value_type& get(const size_t& _index)
        {
            return m_data[_index];
        }

        static constexpr builder_type::table_type m_data = builder_type::build();
    };

    template <typename T, T... VALUES>
    struct table_from_pack
    {
        static constexpr size_t table_size = sizeof...(VALUES);
        using value_type = T;
        using table_type = containers::static_array<T, table_size>;

        static consteval table_type build()
        {
            table_type data{};
            size_t i = 0ull;
            ((data[i++] = VALUES), ...);
            return data;
        }
    };

    template <typename T, T... VALUES>
    struct constexpr_pack_table : public constexpr_table<table_from_pack<T, VALUES...>>
    {
    };

    template <typename FROM, typename TO, std::pair<FROM, TO>... VALUES>
        requires is_contiguous_v<FROM, VALUES.first...>
    struct conversion_table_builder
    {
        static constexpr size_t table_size = static_cast<size_t>(get_max_v<FROM, VALUES.first...>) + 1u;
        using value_type = TO;
        using table_type = containers::static_array<value_type, table_size>;

        static consteval table_type build()
        {
            table_type data{};
            size_t i = 0ull;
            ((data[static_cast<size_t>(VALUES.first)] = VALUES.second), ...);
            return data;
        }
    };

    template <typename FROM, typename TO, std::pair<FROM, TO>... VALUES>
    struct constexpr_conversion_table : public constexpr_table<conversion_table_builder<FROM, TO, VALUES...>>
    {
        using base_type = constexpr_table<conversion_table_builder<FROM, TO, VALUES...>>;

        template <FROM FROM_VALUE>
        static consteval const TO& get()
        {
            return base_type::template get<static_cast<size_t>(FROM_VALUE)>();
        }

        static constexpr const TO& get(const FROM& _from)
        {
            return base_type::get(static_cast<size_t>(_from));
        }
    };

} // namespace veer