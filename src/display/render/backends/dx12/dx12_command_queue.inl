public:
	ComPtr<ID3D12CommandQueue> get_api_handle();
private:
	containers::resizable_array<ID3D12CommandList*> m_queued_command_lists;
	ComPtr<ID3D12CommandQueue> m_command_queue_api_handle;
	ComPtr<ID3D12Fence> m_fence;