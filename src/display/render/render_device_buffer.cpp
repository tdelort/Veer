#include "render_device_buffer.h"
#include "resource_desc.h"

namespace veer::display::render
{
    render_device_buffer::render_device_buffer( render_device& _device, const buffer_desc& _desc )
        : render_device_resource( _device )
        , m_desc{ _desc }
    {

    }

    const buffer_desc& render_device_buffer::desc() const
    {
        return m_desc;   
    }
}