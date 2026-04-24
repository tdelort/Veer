#include "render_device_texture.h"
#include "resource_desc.h"

namespace veer::display::render
{
    // render_device_texture_base

    render_device_texture_base::render_device_texture_base( render_device& _device )
        : render_device_resource( _device )
    {
        
    }

    // render_device_texture_2d

    render_device_texture_2d::render_device_texture_2d( render_device& _device, const texture_2d_desc& _desc )
        : render_device_texture_base( _device )
        , m_desc{ _desc }
    {
    
    }

    const texture_2d_desc& render_device_texture_2d::desc() const
    {
        return m_desc;
    }

    // render_device_texture_3d

    render_device_texture_3d::render_device_texture_3d( render_device& _device, const texture_3d_desc& _desc )
        : render_device_texture_base( _device )
        , m_desc{ _desc }
    {
        
    }

    const texture_3d_desc& render_device_texture_3d::desc() const
    {
        return m_desc;       
    }
}