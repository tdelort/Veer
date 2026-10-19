// .inl to be injected inside class command_buffer header
public:
	static D3D12_COMMAND_LIST_TYPE s_convert(const command_buffer::type _type);

	ID3D12GraphicsCommandList* get_api_handle();

	ID3D12GraphicsCommandList* release_handle();

protected:
	// We might need another handle type on other platforms (XBox DMA CommandLists)
	ID3D12GraphicsCommandList* m_command_list_handle;