protected:
	virtual D3D12_RESOURCE_DESC get_resource_desc() const override;
	virtual void update_views() override; 

public:
	const dx12_descriptor& get_render_target_view() const;
	const dx12_descriptor& get_depth_stencil_view() const;

protected:
	dx12_descriptor m_rtv_cpu_descriptor{};
	dx12_descriptor m_dsv_cpu_descriptor{};
	dx12_descriptor m_srv_cpu_descriptor{};
	dx12_descriptor m_uav_cpu_descriptor{};