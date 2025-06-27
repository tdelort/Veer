#ifndef DISPLAY_DX12_DX12PCH_H_INCLUDED
#define DISPLAY_DX12_DX12PCH_H_INCLUDED

#include <core/core.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif // defined(_DEBUG)

#include <wrl/client.h>
using namespace Microsoft::WRL;

#include <D3D12MemAlloc.h>

#endif // DISPLAY_DX12_DX12PCH_H_INCLUDED
