#ifndef DISPLAY_RESOURCE_SYNCHRONISATION_H_INCLUDED
#define DISPLAY_RESOURCE_SYNCHRONISATION_H_INCLUDED

namespace veer
{
	// High level resource states to be converted in resource states transition / barriers / cache flushes by low level API
	// NOTE : not exhaustive in v0, will be updated each time we need a new state
	enum class resource_sync_state
	{
		Common					= 0,
		Present					= 1 << 0,
		RenderTarget			= 1 << 1,
		CopySource				= 1 << 2,
		CopyDest				= 1 << 3,
		VertexAndConstantBuffer	= 1 << 4,
	};
}

#endif // DISPLAY_RESOURCE_SYNCHRONISATION_H_INCLUDED
