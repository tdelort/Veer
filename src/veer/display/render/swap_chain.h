#pragma once

#include <veer/core/math/vec.h>

#include <veer/display/render/render_device_texture_2d.h>

#if defined(D3D12_RENDER_BACKEND) 
#include <veer/core/containers/static_array.h>
#include <veer/display/render/backends/dx12/dx12_render_device_backbuffer.h>
#endif // defined(D3D12_RENDER_BACKEND) 

namespace veer::display::window
{
    class window;
}

namespace veer::display::render
{
    class render_device;
    class render_device_resource;

    class swap_chain
    {
    public:
        swap_chain(render_device& _device, veer::display::window::window& _window);
        ~swap_chain();

        render_device_texture_2d& get_current_backbuffer();

        size_t get_backbuffer_index();

        void present();

        static constexpr size_t s_swap_chain_buffer_count = 2u;

    protected:
        bool m_tearing_allowed;

#include VEER_INL(swap_chain)
    };
} // namespace veer::display::render