#pragma once

#include <veer/core/core.h>
#include <veer/core/unique_ptr.h>

#include <veer/display/render/shader_source.h>

#if defined(D3D12_RENDER_BACKEND)
#include <veer/display/render/backends/dx12/dx12_descriptor_heap.h>
#include <veer/display/render/backends/dx12/dx12_pch.h>
#endif

namespace veer::display::window
{
    class window;
}

namespace veer::display::render
{
    class copy_command_queue;
    class compute_command_queue;
    class graphics_command_queue;
    class swap_chain;
    class graphics_technique;
    class compute_technique;
    class render_device_buffer;
    class buffer_desc;

    class render_device
    {
    public:
        render_device();
        ~render_device();

        copy_command_queue& get_copy_command_queue() const;
        compute_command_queue& get_compute_command_queue() const;
        graphics_command_queue& get_graphics_command_queue() const;

#include VEER_INL(render_device)
    };
} // namespace veer::display::render