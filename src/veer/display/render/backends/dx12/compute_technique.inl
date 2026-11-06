public:
    ID3D12PipelineState* get_pipeline_state_object() const;
    ID3D12RootSignature* get_root_signature() const;
    
private:
    ComPtr<ID3D12RootSignature> m_root_signature;
    ComPtr<ID3D12PipelineState> m_pso;