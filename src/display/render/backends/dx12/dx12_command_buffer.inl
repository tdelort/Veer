// .inl to be injected inside class command_buffer header
public:
	static D3D12_COMMAND_LIST_TYPE s_convert(const command_buffer::type _type);

	ID3D12GraphicsCommandList* get_api_handle();

	// TODO : maybe other implems will need something similar
	void open(dx12_render_device& _device, ComPtr<ID3D12CommandAllocator>& _command_allocator);
	void close();

protected:
	// We might need another handle type on other platforms (XBox DMA CommandLists)
	ComPtr<ID3D12GraphicsCommandList> m_command_list_handle;