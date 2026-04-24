#pragma once

#include "dx12_pch.h"

#include "dx12_descriptor_heap.h"

#include <d3d12.h>
#include <display/render/render_device_resource.h>
#include <display/render/render_device_texture.h>
#include <display/render/render_device_buffer.h>


namespace veer::display::render
{
	// needed ? no
	// class dx12_render_device_resource_view
	// {
	// 	dx12_descriptor m_cpu_handle;
	// 	std::optional<dx12_descriptor> m_gpu_handle;
	// };

	// little helper api to commonize code
	class dx12_render_device_resource
	{
	public:
		virtual D3D12_RESOURCE_DESC get_resource_desc() const = 0;
		virtual void update_views() = 0;
		
		ID3D12Resource* get_api_handle() const;
		void set_api_handle( ID3D12Resource* _val );
		
		const dx12_descriptor& get_cpu_handle() const;

	protected:
		dx12_descriptor m_cpu_descriptor{};

	private:
		ComPtr<ID3D12Resource> m_api_handle{ nullptr };
	};



	static D3D12_RESOURCE_FLAGS s_convert( buffer_desc::usage_flags _states );

	class dx12_render_device_buffer : public render_device_buffer, public dx12_render_device_resource
	{
	public:
		dx12_render_device_buffer( render_device& _device, const buffer_desc& _desc );
		virtual ~dx12_render_device_buffer() override; // TODO

		// from render_device_resource
		void upload( command_buffer& _upload_buffer ) override;
		virtual backend_handle as_backend() override { return static_cast<dx12_render_device_resource*>( this ); }

		// from dx12_render_device_resource
		D3D12_RESOURCE_DESC get_resource_desc() const override;
		virtual void update_views() override;
	};


	static D3D12_RESOURCE_FLAGS s_convert( texture_desc::usage_flags _states );

	class dx12_render_device_texture_2d : public render_device_texture_2d, public dx12_render_device_resource
	{
	public:
		dx12_render_device_texture_2d( render_device& _device, const texture_2d_desc& _desc );
		virtual ~dx12_render_device_texture_2d() override; // TODO

		// from render_device_resource
		virtual void upload( command_buffer& _upload_buffer ) override;
		virtual backend_handle as_backend() override { return static_cast<dx12_render_device_resource*>( this ); }

		// from dx12_render_device_resource
		virtual D3D12_RESOURCE_DESC get_resource_desc() const override;
		virtual void update_views() override;
	};

	class dx12_render_device_texture_3d : public render_device_texture_3d, public dx12_render_device_resource
	{
	public:
		dx12_render_device_texture_3d( render_device& _device, const texture_3d_desc& _desc );
		virtual ~dx12_render_device_texture_3d() override; // TODO

		// from render_device_resource
		virtual void upload( command_buffer& _upload_buffer ) override;
		virtual backend_handle as_backend() override { return static_cast<dx12_render_device_resource*>( this ); }

		// from dx12_render_device_resource
		D3D12_RESOURCE_DESC get_resource_desc() const override;
		virtual void update_views() override;
	};

	class dx12_render_device_backbuffer : public dx12_render_device_texture_2d
    {
	public:
		dx12_render_device_backbuffer( render_device& _device, const texture_2d_desc& _desc, ID3D12Resource* _backbuffer_resource );
		virtual ~dx12_render_device_backbuffer() override;

		// from render_device_resource
		virtual void upload( command_buffer& _upload_buffer ) override;

		// from dx12_render_device_resource
		virtual void update_views() override;
    };
}