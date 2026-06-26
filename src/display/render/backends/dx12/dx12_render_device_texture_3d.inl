protected:
	virtual D3D12_RESOURCE_DESC get_resource_desc() const override;
	virtual void update_views() override; 

public:
	const dx12_descriptor& get_render_target_view() const;

protected:
	dx12_descriptor m_render_target_cpu_descriptor{};