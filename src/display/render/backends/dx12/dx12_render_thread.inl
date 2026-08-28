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
	template<typename T>
	struct command_list_type_tuple  
	{
		T m_graphics;
		T m_compute;
		T m_copy;

		T* get_from_type(D3D12_COMMAND_LIST_TYPE _type)
		{
			switch (_type) 
			{
			case D3D12_COMMAND_LIST_TYPE_DIRECT:
				return &m_graphics;
			case D3D12_COMMAND_LIST_TYPE_COMPUTE: 
				return &m_compute;
			case D3D12_COMMAND_LIST_TYPE_COPY:
				return &m_copy;
			default:
				break;
			}
			return nullptr;
		}
	};

	using command_list_handle_pool_t = containers::resizable_array<ID3D12GraphicsCommandList*>;
	using command_list_handle_pools_t = command_list_type_tuple<command_list_handle_pool_t>;

	using command_list_allocators_t = command_list_type_tuple<ID3D12CommandAllocator*>;
	
private:
	command_list_allocators_t& get_current_command_allocator();

private:
	containers::static_array<command_list_allocators_t, swap_chain::s_swap_chain_buffer_count> m_command_list_allocators;
	command_list_handle_pools_t m_command_list_pools;

#if 0
	freelist<dx12_command_list_handle> m_graphics_command_list_handles;
	freelist<dx12_command_list_handle> m_compute_command_list_handles;
	freelist<dx12_command_list_handle> m_copy_command_list_handles;
#endif // 0