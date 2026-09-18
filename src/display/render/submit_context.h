#pragma once

#include <core/concepts.h>
#include <core/containers/span.h>
#include <core/containers/static_array.h>
#include <core/math/vec.h>

#include <display/render/base_types.h>
#include <display/render/constant_buffer.h>

namespace veer::display::render
{
    class compute_technique;
    class graphics_technique;
    struct shader_parameter_id;

    class command_buffer;
    class copy_command_buffer;
    class compute_command_buffer;
    class graphics_command_buffer;

    class render_device_texture_base;
    class render_device_texture_2d;
    class render_device_buffer;
    class render_device_resource;

    // TODO : We could probably need a copy_submit_context, but if this is the case, there is a few things that would
    // benefit from a schmol refactor

    class submit_context
    {
    public:
        submit_context(command_buffer& _command_buffer);

    protected:
        command_buffer& m_command_buffer;
    };

    class copy_submit_context : public submit_context
    {
    public:
        copy_submit_context(copy_command_buffer& _command_buffer);

        // TODO : add copies

    private:
        copy_command_buffer& get_copy_command_buffer();
    };

    // I know this feels like a non owning command_buffer wrapper, (because it is :3), but its goal is to hide and
    // simplify some boiler plate code that 90% of users won't care about (profile markers, resource transition,
    // constant buffer handling, etc). And in the case you want the extra control, you can still get/use the underlying
    // command buffer
    class compute_submit_context : public submit_context
    {
    public:
        compute_submit_context(compute_command_buffer& _command_buffer, compute_technique& _technique);

    public:
        // TODO : add copies
        void clear_texture(render_device_texture_base& _resource, math::vec4f _val);
        void clear_texture(render_device_texture_base& _resource, math::vec4u _val);
        void clear_buffer(render_device_buffer& _resource, float _val);
        void clear_buffer(render_device_buffer& _resource, uint32_t _val);

        void set_constant_buffer(constant_buffer& _constant_buffer, constant_buffer_type _type);

        void dispatch(size_t _x, size_t _y, size_t _z);

    private:
        compute_command_buffer& get_compute_command_buffer();
    };

    class graphics_submit_context : public submit_context
    {
    public:
        graphics_submit_context(graphics_command_buffer& _command_buffer, graphics_technique& _technique);

    public:
        // TODO : add copies
        // TODO : add clears

        // TODO : For submit_contexts, this might be a bit too much control ?
        void set_render_output(render_device_texture_2d* _depth, render_device_texture_2d* _color);
        void set_render_output(render_device_texture_2d* _depth, containers::span<render_device_texture_2d*> _colors);
        void set_viewports(containers::span<viewport> _viewports);
        void set_scissors(containers::span<rect> _scissor_rects);

        void clear_render_target(render_device_texture_2d& _render_target_resource, math::vec4f _color);
        void clear_depth_stencil(render_device_texture_2d& _render_target_resource, float _depth, uint8_t _stencil);

        // TODO : for now, my meshes are just that (index + vertex buffer). BUT replace this when we actually have a
        // mesh construct
        // TODO : vertex and/or index buffer are actually optional. Change API to support this
        void set_mesh(render_device_buffer& _index_buffer, render_device_buffer& _vertex_buffer);

        void set_constant_buffer(constant_buffer& _constant_buffer, constant_buffer_type _type);

        void draw_instanced(size_t _vertex_count, size_t _instance_count);
        void draw_indexed_instanced(size_t _index_count, size_t _instance_count);
        void draw_indirect(render_device_buffer& _buffer, size_t _arg_index);

    private:
        void apply_context();

    private:
        graphics_command_buffer& get_graphics_command_buffer();

    private:
        containers::static_array<viewport, s_max_viewport_and_scissors_count> m_viewports;
        size_t m_viewports_count;

        containers::static_array<rect, s_max_viewport_and_scissors_count> m_scissors;
        size_t m_scissors_count;

        render_device_texture_2d* m_depth_render_output;
        containers::static_array<render_device_texture_2d*, s_max_color_render_targets> m_color_render_outputs;
        size_t m_color_render_outputs_count;

        render_device_buffer* m_index_buffer;
        render_device_buffer* m_vertex_buffer;
    };
} // namespace veer::display::render