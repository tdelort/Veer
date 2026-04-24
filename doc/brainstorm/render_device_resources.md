# Render Device Resources

White board to think about Textures / Buffers / Shaders etc resources architecture

## High level usage

```c++

// 2 most likely use cases :

// Either a texture read from disk containing Read Only data
veer::texture_2d m_octahedral_envmap;

// Or a texture used during rendering (here for example we compute at runtime a mip chain from the above texture read from disk)
veer::texture_2d m_octahedral_envmap_mips;

std::unique_ptr<veer::compute_shader> m_compute_mip_chains;

// Creation (later)
void foo( render_device& _device )
{
    // I want something like this
    // m_compute_mip_chains = new veer::compute_shader( "path_to_compiled_shader_with_metadata" );
    // But i know this is not possible since I need the render device
    m_shader_source = veer::shader_source( (big_struct){ "source code compiled or not", { ... }, { ... }, etc } ); 
    m_compute_mip_chains = _device.alloc<veer::compute_shader>( m_shader_source ); // Here we build the backend specific stuff
    
    // check if I need device for this ?
    m_octahedral_envmap = _device.alloc<veer::texture_2d>( "path_to_texture_on_disk" );
    m_octahedral_envmap_mips = _device.alloc<veer::texture_2d>( params, to, build, texture );
}

// Usage (at this point, we know that all mips needed have been allocated and that the mip0 resolution matches the source texture)
void bar()
{
    m_compute_mip_chains.set_texture_readonly( "Source", m_octahedral_envmap );

    size_t mip_count = min( m_octahedral_envmap_mips.get_mip_count(), s_compute_mip_chains_max_mips );
    for( size_t i = 0; i < mip_count; ++i )
    {
        std::string mip_target_name;
        mip_target_name.format( "Mip%u", i );
        m_compute_mip_chains.set_texture_readwrite( "", m_octahedral_envmap_mips.get_mip( i ) );
    }
    
    m_compute_mip_chains.set_int( "MipCount", mip_count );
}
```

I speak about textures here but read "resources"

Something that (i think) makes a lot of sense is to have a type for the gpu texture (that wraps the backend resource alloc) 
And to have a higher level type that will be the only type know to the render graph.
You could probably get away with only one type, but this allows thinner hardware abstraction layer and decoupling the lifetime of the "high" level texture from actual gpu residency of the texture (you might want a handle to a texture without actually uploading it yet (to batch uploads) or keeping the gpu data alive longer than the handle for whatever reasons)
But these will not be implemented in veer.
There might actually be a need for other types of texture for gameplay purpose (say I want to expose a Texture type to the gameplay so that it can do basic stuff with it like render a camera to a world space texture).

```c++

class texture_2d
{
    // nullptr until explicitly created / uploaded to GPU
    render_device_texture_2d* m_gpu_texture;
    
}

```

I want to have a way to know if a specific device resource is ready to render. Say I have a high level class representing a mesh with materials and texture and all. I want to have a simple way to ask all render_device_resources if they are ready to render.  


I might need to implement a resource handling interface. Something as simple as this :

```c++
class 
{
    
};
```
