# Render Graph  

## The goal  

```cpp
struct frame_context
{
    // Data like time, cursor, level, anything that can be use to get info for rendering
    // and also, the device
    render_device& m_device;
};

struct shader_parameter_id 
{
    // platform dependent ?
    // Root Signature param index on D3D12 basically
};

class my_render_node : public render_graph_node
{
private:
    compute_technique& m_my_custom_technique;
    texture& m_texture_output;
    buffer& m_buffer_input;

    shader_parameter_id m_parameter_id;
public:
    my_render_node(compute_technique& _my_custom_technique)
    {
        m_parameter_id = _my_custom_technique.get_constant_id("my_constant_buffer.parameter");
    };

    virtual void setup(frame_context& _device) override
    {
        // this is a purely hypothetical step at this point, but I will probably need to be able to 
        // declare here what resources usages I will do in the render function in order to generate the 
        // best resource transitions possible. In an engine with Begin/End resource transitions and async compute, 
        // this will be mandatory. 
    };

    virtual void render(frame_context& _ctx) override
    {
        command_queue& compute_queue = _ctx.m_device.get_command_queue(command_queue::type::compute);
        
        // holds temporary draw/submit data and wraps a command buffer
        compute_submit_context& submit_ctx = compute_submit_context(_ctx.m_render_thread);

        submit_ctx.set_constant("my_constant_buffer.parameter", 0.0f);
        // or
        submit_ctx.set_constant(m_parameter_id, 0.0f);

        submit_ctx.set_texture("my_read_write_texture", m_texture_output);
        submit_ctx.set_buffer("my_read_only_buffer", m_buffer_input);

        submit_ctx.submit();


        // we could also :
        compute_command_buffer& compute_command_buffer = compute_command_buffer(_ctx.m_render_thread);
        // to get finer control, but more verbose and error prone syntax
        // But useful for custom techniques (say I want to implement ray tracing or mesh shaders without having to make it fit in the above API)
        // Or when you need a command buffer for something else than rendering (uploading data for example)

        // command_buffer created, given a api command buffer, and then opened

        compute_command_buffer.clear_texture(m_texture_output, vec4f(0.f, 0.f, 0.f, 0.f));

        // command_buffer closed in here, executed, and api command buffer queue in queue 
        
        // steals and "executes" the buffer. (actually, we could store it and then "flush" multiple command buffers at once by calling ECL)
        // compute_queue::execute(command_buffer&& _buffer)
        compute_queue.execute(std::move(compute_command_buffer));
        
        // compute_command_buffer has been moved, so can't use it anymore 
        
        // later, when command buffer is actually executed (when fence value that was set during execute call is actually waited)
        // we can destroy the command buffer object. (the api object does not have this restriction thanks to the command allocator pattern
        // so we can yield it to the render thread / device after call to ECL, but the veer::command_buffer object needs to live long enough to see
        // it's callback on_execution be called) 
    };
};
```

## The steps  

Let's start with the submit context first

I'll try to keep it as platform independent as possible and cram as much as possible in the command list or
simply behind #if ... #endif clauses. 