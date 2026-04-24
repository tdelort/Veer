#pragma once

#include "resource_desc.h"
#include "render_device_resource.h"

namespace veer::display::render
{
    class render_device_texture_base : public render_device_resource
    {
    public:
        render_device_texture_base( render_device& _device );
    };

    class render_device_texture_2d : public render_device_texture_base
    {
    public:
        render_device_texture_2d( render_device& _device, const texture_2d_desc& _desc );

        const texture_2d_desc& desc() const;
		
    private:
        texture_2d_desc m_desc{};
    };

    class render_device_texture_3d : public render_device_texture_base
    {
    public:
        render_device_texture_3d( render_device& _device, const texture_3d_desc& _desc );

        const texture_3d_desc& desc() const;
    private:
        texture_3d_desc m_desc{};
    };
}