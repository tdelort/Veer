#pragma once

#include <veer/core/core.h>

#if defined(D3D12_RENDER_BACKEND)
#include <veer/core/containers/static_array.h>

#include <veer/display/render/backends/dx12/dx12_pch.h>
#include <veer/display/render/swap_chain.h>
#endif

namespace veer::display::render
{
    class command_buffer;
    class render_device;

    class render_thread
    {
    public:
        render_thread(const render_device& _device);
        ~render_thread();

        void start_frame();
        void end_frame();

        uint64_t get_render_frame_index()
        {
            return m_current_render_frame_index;
        }

        const render_device& get_device() const
        {
            return m_device;
        }

        void on_close_command_list()
        {
            VEER_ASSERT(m_command_buffer_open, "on_close_command_list called while no command buffer owner by this render thread was open, fishy");
            m_command_buffer_open = false;
        }

    private:
        const render_device& m_device;
        size_t m_current_render_frame_index{0u};
        bool m_command_buffer_open{false};

#include VEER_INL(render_thread)
    };
} // namespace veer::display::render