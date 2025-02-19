#include "dx12_pch.h"
#include "dx12_render_device_resource.h"

namespace veer
{
	void dx12_render_device_resource::fill_resource(ComPtr<ID3D12Resource> _dx12_resource, D3D12_CPU_DESCRIPTOR_HANDLE _cpu_handle)
	{
		m_resource = _dx12_resource;
		m_cpu_handle = _cpu_handle;
	}

	ComPtr<ID3D12Resource> dx12_render_device_resource::get_resource()
	{
		return m_resource;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE dx12_render_device_resource::get_cpu_descriptor_handle()
	{
		return m_cpu_handle;
	}

	dx12_render_device_resource::~dx12_render_device_resource()
	{
		reset();
	}

	void dx12_render_device_resource::reset()
	{
		m_resource.Reset();
		m_cpu_handle = {};
	}
}
