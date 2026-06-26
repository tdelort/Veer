#pragma once

#include "render_device_resource.h"

namespace veer::display::render
{
    class render_device_texture_base : public render_device_resource
    {
    public:
        render_device_texture_base(render_device& _device)
            : render_device_resource(_device)
        {

        }
    };
}