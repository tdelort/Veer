#include "dx12_pch.h"
#include "dx12_render_device_resource.h"
#include "dx12_descriptor_heap.h"

namespace veer
{
	void dx12_render_device_resource::fill_resource(ComPtr<ID3D12Resource> _dx12_resource, dx12_descriptor _descriptor)
	{
		m_resource = _dx12_resource;
		m_descriptor = _descriptor;
	}

	ComPtr<ID3D12Resource> dx12_render_device_resource::get_resource()
	{
		return m_resource;
	}

	dx12_descriptor& dx12_render_device_resource::get_cpu_descriptor_handle()
	{
		return m_descriptor;
	}

	dx12_render_device_resource::~dx12_render_device_resource()
	{
		reset();
	}

	void dx12_render_device_resource::reset()
	{
		m_resource.Reset();
		m_descriptor = {};
	}
}
