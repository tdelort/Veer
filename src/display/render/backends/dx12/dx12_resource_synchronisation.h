#ifndef DISPLAY_DX12_DX12_RESOURCE_SYNCHRONISATION_H_INCLUDED
#define DISPLAY_DX12_DX12_RESOURCE_SYNCHRONISATION_H_INCLUDED

#include "dx12_pch.h"

#include <d3d12.h>
#include <display/render/resource_synchronisation.h>

namespace veer
{
	static D3D12_RESOURCE_STATES s_convert(resource_sync_state _states)
	{
		static constexpr std::pair<D3D12_RESOURCE_STATES, resource_sync_state> s_conversionArray[] =
		{
			{ D3D12_RESOURCE_STATE_COMMON,						resource_sync_state::Common						},
			{ D3D12_RESOURCE_STATE_PRESENT,						resource_sync_state::Present					},
			{ D3D12_RESOURCE_STATE_RENDER_TARGET,				resource_sync_state::RenderTarget				},
			{ D3D12_RESOURCE_STATE_COPY_SOURCE,					resource_sync_state::CopySource					},
			{ D3D12_RESOURCE_STATE_COPY_DEST,					resource_sync_state::CopyDest					},
			{ D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,	resource_sync_state::VertexAndConstantBuffer	}
		};

		unsigned int input = (unsigned int)_states;
		D3D12_RESOURCE_STATES output{};

		for (size_t i = 0; i < VEER_STATIC_ARRAY_SIZE(s_conversionArray); ++i)
		{
			const std::pair<D3D12_RESOURCE_STATES, resource_sync_state> mapping = s_conversionArray[i];

			if (input & (unsigned int)mapping.second)
				output |= mapping.first;
		}

		return output;
	}
}

#endif // DISPLAY_DX12_DX12_RESOURCE_SYNCHRONISATION_H_INCLUDED
