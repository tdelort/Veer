protected:
	virtual D3D12_RESOURCE_DESC get_resource_desc() const override;
	virtual void update_views() override; 

public:
	const D3D12_INDEX_BUFFER_VIEW& get_index_buffer_view() const;
	const D3D12_VERTEX_BUFFER_VIEW& get_vertex_buffer_view() const;

protected:
	dx12_descriptor m_srv_cpu_descriptor{};
	dx12_descriptor m_uav_cpu_descriptor{};
	dx12_descriptor m_cbv_cpu_descriptor{};
	D3D12_INDEX_BUFFER_VIEW m_index_buffer_view{};
	D3D12_VERTEX_BUFFER_VIEW m_vertex_buffer_view{};