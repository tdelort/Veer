#pragma once

#include "dx12_pch.h"

#include <core/core.h>
#include <core/containers/resizable_array.h>

namespace veer::display::render
{
	class dx12_render_device;

	struct dx12_descriptor
	{
		static constexpr size_t s_invalid_index = size_t( -1 );

		dx12_descriptor() : m_handle{}, m_index{ s_invalid_index } { }

		dx12_descriptor(D3D12_CPU_DESCRIPTOR_HANDLE _handle, size_t _index)
			: m_handle(_handle), m_index(_index)
		{ }

		bool is_valid() const { return m_index != s_invalid_index; }

		D3D12_CPU_DESCRIPTOR_HANDLE m_handle;

	private:
		size_t m_index;
		friend class dx12_descriptor_heap;
	};

	class dx12_descriptor_heap
	{
	public:
		dx12_descriptor_heap( dx12_render_device& _device, D3D12_DESCRIPTOR_HEAP_TYPE _type, D3D12_DESCRIPTOR_HEAP_FLAGS _flags, size_t _size);
		~dx12_descriptor_heap();

		dx12_descriptor acquire_descriptor();
		void release_descriptor( dx12_descriptor _descriptor );

	private:
		ComPtr<ID3D12DescriptorHeap> m_api_handle;
		UINT m_increment_size;

		size_t m_size;
		size_t m_first_available_descriptor_index;

		veer::containers::resizable_array<size_t> m_free_indices;
		size_t m_last_used_index;
	};
}