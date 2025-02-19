#ifndef DISPLAY_DX12_DX12_RENDER_DEVICE_RESOURCE_H_INCLUDED
#define DISPLAY_DX12_DX12_RENDER_DEVICE_RESOURCE_H_INCLUDED

#include "dx12_pch.h"

#include <Display/render_device_resource.h>

namespace veer
{
	class dx12_render_device_resource : public render_device_resource
	{
	public:
		using render_device_resource::render_device_resource;

		// either created or it is given to us (swap_chain)  
		//void create_resource( params );
		void fill_resource(ComPtr<ID3D12Resource> _dx12_resource, D3D12_CPU_DESCRIPTOR_HANDLE _cpu_handle);

		ComPtr<ID3D12Resource> get_resource();
		D3D12_CPU_DESCRIPTOR_HANDLE get_cpu_descriptor_handle();

		void reset();

		virtual ~dx12_render_device_resource();
	private:
		ComPtr<ID3D12Resource> m_resource;
		// TODO Not ideal
		D3D12_CPU_DESCRIPTOR_HANDLE m_cpu_handle;
	};
}

#endif // DISPLAY_DX12_DX12_RENDER_DEVICE_RESOURCE_H_INCLUDED
