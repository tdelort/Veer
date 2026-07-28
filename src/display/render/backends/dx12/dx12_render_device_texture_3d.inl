protected:
	virtual D3D12_RESOURCE_DESC get_resource_desc() const override;
	virtual void update_views() override; 

protected:
	dx12_descriptor m_srv_cpu_descriptor{};
	dx12_descriptor m_uav_cpu_descriptor{};