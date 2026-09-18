#pragma once

#include <core/core.h>
#include <core/containers/string.h>

#include <d3d12.h>
#include <dxgi1_6.h>
// #include <d3dcompiler.h>

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif // defined(_DEBUG)

#include <wrl/client.h>
using namespace Microsoft::WRL;

#include <D3D12MemAlloc.h>


inline veer::containers::string HrToString(HRESULT hr)
{
    char s_str[64] = {};
    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
    veer::containers::string output;
    output = s_str;
    return output;
}