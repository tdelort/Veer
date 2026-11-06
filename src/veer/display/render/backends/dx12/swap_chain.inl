private:
    ComPtr<IDXGISwapChain4> m_api_swap_chain_handle;
    ComPtr<ID3D12DescriptorHeap> m_back_buffers_descritor_heap;

    containers::static_array<unique_ptr<dx12_render_device_backbuffer>, s_swap_chain_buffer_count> m_back_buffers_resources = {};