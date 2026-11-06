#pragma once

#include <veer/display/render/render_device_resource.h>

namespace veer::display::render
{
    class render_device_texture_base : public render_device_resource
    {
    public:
        render_device_texture_base(render_device& _device, const char* _debug_name);
        virtual ~render_device_texture_base();

    public:
        virtual void upload(copy_command_buffer& _upload_command_buffer, upload_flags _upload_flags);

#include VEER_INL(render_device_texture_base)
    };
} // namespace veer::display::render