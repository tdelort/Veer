#if 0
private:
	struct dx12_command_list_handle
	{
		dx12_command_list_handle(const render_device& _device, ComPtr<ID3D12CommandAllocator>& _command_allocator, D3D12_COMMAND_LIST_TYPE _type)
		{
			_device.get_api_handle()->CreateCommandList(0, _type, _command_allocator.Get(), nullptr, IID_PPV_ARGS(&m_ptr));
		}
		
		~dx12_command_list_handle()
		{
			if (m_ptr != nullptr)
				m_ptr.Release();
		}

		ID3D12GraphicsCommandList* m_ptr{nullptr};
	};

#endif // 0
public:
	ID3D12GraphicsCommandList* alloc_api_command_list(D3D12_COMMAND_LIST_TYPE _type);
	void free_api_command_list(ID3D12GraphicsCommandList* _ptr);

private:
	using command_list_handle_pool_t = containers::resizable_array<ID3D12CommandList*>;
	command_list_handle_pool_t& get_command_list_handle_pool(D3D12_COMMAND_LIST_TYPE _type);

	ID3D12CommandAllocator* get_current_command_allocator();

private:
	ComPtr<ID3D12CommandAllocator> m_command_allocators[swap_chain::s_swap_chain_buffer_count];

	command_list_handle_pool_t m_graphics_command_list_handles;
	command_list_handle_pool_t m_compute_command_list_handles;
	command_list_handle_pool_t m_copy_command_list_handles;

#if 0
	freelist<dx12_command_list_handle> m_graphics_command_list_handles;
	freelist<dx12_command_list_handle> m_compute_command_list_handles;
	freelist<dx12_command_list_handle> m_copy_command_list_handles;
#endif // 0