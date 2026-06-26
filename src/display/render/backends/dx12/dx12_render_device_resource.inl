public:
	static D3D12_RESOURCE_FLAGS s_convert(buffer_desc::usage_flags _states);
	static D3D12_RESOURCE_FLAGS s_convert(texture_desc::usage_flags _states);

protected:
	virtual D3D12_RESOURCE_DESC get_resource_desc() const = 0;
	virtual void update_views() = 0;
	
	void alloc();
	void copy_to_gpu(copy_command_buffer& _upload_buffer, veer::containers::span<const byte_t> _data);

public:
	ID3D12Resource* get_api_handle() const;
	void set_api_handle(ID3D12Resource* _resource);

private:
	D3D12MA::Allocation* m_resource_alloc{nullptr};
	ID3D12Resource* m_api_handle{nullptr};