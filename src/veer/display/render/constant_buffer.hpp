#pragma once

#include "constant_buffer.h"

#include <veer/core/math/mat.h>
#include <veer/core/math/vec.h>

namespace veer::display::render
{
    template <RenderDataType T>
    void constant_buffer::update_context::set_constant(const buffer_elem_info& _elem, T _val)
    {
        std::memcpy(m_constant_buffer.m_cpu_buffer.data() + _elem.get_offset(), &_val, _elem.get_size());
    }

    template <RenderDataType T, size_t ELEM_COUNT>
        requires veer::LessEqual<ELEM_COUNT, 4u>
    void constant_buffer::update_context::set_constant(const buffer_elem_info& _elem, math::vec<T, ELEM_COUNT> _val)
    {
        std::memcpy(m_constant_buffer.m_cpu_buffer.data() + _elem.get_offset(), &_val[0], _elem.get_size());
    }

    template <RenderDataType T, size_t ROW_COUNT, size_t COLUMN_COUNT>
        requires veer::LessEqual<ROW_COUNT, 4u> && veer::LessEqual<COLUMN_COUNT, 4u>
    void constant_buffer::update_context::set_constant(const buffer_elem_info& _elem, math::mat<T, ROW_COUNT, COLUMN_COUNT> _val)
    {
        // We need to iterate through rows of the mat since we know that they might NOT be stored contiguously
        for (size_t r = 0; r < ROW_COUNT; ++r)
        {
            const size_t cpu_row_size = render_data_type_traits<math::vec<T, COLUMN_COUNT>>::cpu_size;
            const size_t gpu_row_size = render_data_type_traits<math::vec<T, COLUMN_COUNT>>::gpu_size;
            std::memcpy(m_constant_buffer.m_cpu_buffer.data() + _elem.get_offset() + r * gpu_row_size, &_val[r][0], cpu_row_size);
        }
    }
} // namespace veer::display::render