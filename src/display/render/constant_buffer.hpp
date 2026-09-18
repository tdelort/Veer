#pragma once

#include "constant_buffer.h"
    
namespace veer::display::render
{
    template <RenderDataType T>
    void constant_buffer::update_context::set_constant(const buffer_elem_info& _elem, T _val)
    {
        memcpy(m_constant_buffer.m_cpu_buffer.data() + _elem.get_offset(), &_val, _elem.get_size());
    }

    template <RenderDataType T, size_t ELEM_COUNT>
    void constant_buffer::update_context::set_constant(const buffer_elem_info& _elem, math::vec<T, ELEM_COUNT> _val)
        requires veer::LessEqual<ELEM_COUNT, 4>
    {
        memcpy(m_constant_buffer.m_cpu_buffer.data() + _elem.get_offset(), &_val[0], _elem.get_size());
    }
}