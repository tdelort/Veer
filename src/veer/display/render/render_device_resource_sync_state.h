#pragma once

namespace veer::display::render
{
    // High level resource states to be converted in resource states transition / barriers / cache flushes by low level
    // API NOTE : not exhaustive in v0, will be updated each time we need a new state
    // TODO : most resource state API are migrating toward a lower level more clear and intuitive system
    //        based on a pair of 'location' (say compute vs graphics pipe) and layout (depth render target, UAV, SRV,
    //        etc) I should update my API in order to be able to infer both of these from each enum value (even if it
    //        means having 2 enums)
    // 		  This is relatively high priority but will probably wait the Vulkan backend implementation
    enum class render_device_resource_sync_state
    {
        // sorry for the very dx12 names
        Common = 0,
        PixelShaderResource = 1 << 0,
        NonPixelShaderResource = 1 << 1,
        UnorderedAccessView = 1 << 2,
        Present = 1 << 3,
        RenderTarget = 1 << 4,
        DepthWrite = 1 << 5,
        DepthRead = 1 << 6,
        CopySource = 1 << 7,
        CopyDest = 1 << 8,
        VertexAndConstantBuffer = 1 << 9,
        IndexBuffer = 1 << 10,
        IndirectArgs = 1 << 11,
    };

    // V0 mono threaded API :
    // We can simply issue transition when we want since we don't have to worry about another thread creating command
    // lists Thus state tracking can be solely on the resource
    class render_device_resource_sync_state_traking
    {
    public:
        void set_resource_state(render_device_resource_sync_state _new_state)
        {
            m_state = _new_state;
        }
        render_device_resource_sync_state get_resource_state() const
        {
            return m_state;
        }

        render_device_resource_sync_state m_state{render_device_resource_sync_state::Common};
    };
} // namespace veer::display::render