public:
	ComPtr<ID3D12CommandQueue> get_api_handle();
private:
	// in SoA instead of AoS since we need the contiguous ID3D12CommandList* array for ExecuteCommandLists
	containers::resizable_array<ID3D12CommandList*> m_queued_command_lists;
	containers::resizable_array<display::render::render_thread*> m_queued_command_lists_owner_thread;

	ComPtr<ID3D12CommandQueue> m_command_queue_api_handle;
	ComPtr<ID3D12Fence> m_fence;