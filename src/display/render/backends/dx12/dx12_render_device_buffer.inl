protected:
	virtual D3D12_RESOURCE_DESC get_resource_desc() const override;
	virtual void update_views() override; 

public:
	const D3D12_INDEX_BUFFER_VIEW& get_index_buffer_view() const;
	const D3D12_VERTEX_BUFFER_VIEW& get_vertex_buffer_view() const;
	const dx12_descriptor& get_constant_buffer_view() const;

private:
	D3D12_INDEX_BUFFER_VIEW m_index_buffer_view{};
	D3D12_VERTEX_BUFFER_VIEW m_vertex_buffer_view{};
	dx12_descriptor m_constant_buffer_cpu_descriptor{};