public:
	ComPtr<ID3D12Device2> get_api_handle() const;
	D3D12MA::Allocator* get_allocator() const;
	ComPtr<IDXGIFactory4> get_dxgi_factory() const;

	dx12_descriptor_heap& get_rtv_descriptor_heap() const;
	dx12_descriptor_heap& get_dsv_descriptor_heap() const;
	dx12_descriptor_heap& get_srv_uav_cbv_descriptor_heap() const;
	dx12_descriptor_heap& get_sampler_descriptor_heap() const;

	void check_errors() const;

private:
	void create_descriptor_heaps();
	void create_allocator();

private:
	// non platform specific types, but keep them in the dx12_render_device to control when they are created
	// (and some platform might want to use only on queue for all 3 types of queue)
	unique_ptr<graphics_command_queue> m_graphics_queue{nullptr};
	unique_ptr<compute_command_queue> m_compute_queue{nullptr};
	unique_ptr<copy_command_queue> m_copy_queue{nullptr};

	ComPtr<ID3D12Device2> m_api_device_handle;
	ComPtr<IDXGIAdapter1> m_adapter;
	ComPtr<IDXGIFactory4> m_dxgi_factory;

	unique_ptr<dx12_descriptor_heap> m_rtv_descriptor_heap{nullptr};
	unique_ptr<dx12_descriptor_heap> m_dsv_descriptor_heap{nullptr};
	unique_ptr<dx12_descriptor_heap> m_srv_uav_cbv_descriptor_heap{nullptr};
	unique_ptr<dx12_descriptor_heap> m_sampler_descriptor_heap{nullptr};

	D3D12MA::Allocator* m_allocator;
#if defined(_DEBUG)
	ComPtr<IDXGIInfoQueue> m_info_queue;
#endif // defined(_DEBUG)