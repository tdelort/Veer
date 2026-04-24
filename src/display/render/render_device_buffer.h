#pragma once

#include "resource_desc.h"
#include "render_device.h"
#include "render_device_resource.h"

namespace veer::display::render
{
    class render_device_buffer : public render_device_resource
    {
    public:
        render_device_buffer( render_device& _device, const buffer_desc& _desc );

        const buffer_desc& desc() const;
		
    private:
        buffer_desc m_desc;
    };

    class render_device_index_buffer : public render_device_buffer
    {
    public:
        using render_device_buffer::render_device_buffer;
    };

    class render_device_vertex_buffer : public render_device_buffer
    {
    public:
        using render_device_buffer::render_device_buffer;
    };
}