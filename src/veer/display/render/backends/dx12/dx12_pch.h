#pragma once

#include <veer/core/containers/string.h>
#include <veer/core/core.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <dxgiformat.h>
// #include <d3dcompiler.h>

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif // defined(_DEBUG)

#include <wrl/client.h>
using namespace Microsoft::WRL;

// #if defined(_DEBUG)
// #define D3D12MA_DEBUG_LOG(format, ...)                                                                                                                                             \
//     do                                                                                                                                                                             \
//     {                                                                                                                                                                              \
//         char buffer[512u];                                                                                                                                                         \
//         std::sprintf(buffer, format, __VA_ARGS__);                                                                                                                                 \
//         VEER_LOG_DEBUG(buffer);                                                                                                                                                    \
//     } while (false)
// #endif // defined(_DEBUG)

#include <D3D12MemAlloc.h>

#include <ostream>
namespace veer
{
    struct hr
    {
        hr(HRESULT _hr)
            : m_value(_hr)
        {
        }

        friend std::ostream& operator<<(std::ostream& os, const veer::hr& _hr)
        {
            char s_str[64] = {};
            sprintf_s(s_str, "0x%08X", static_cast<UINT>(_hr.m_value));
            return os << s_str;
        }

        bool succeeded() const
        {
            return SUCCEEDED(m_value);
        }

        HRESULT m_value;
    };
} // namespace veer

inline veer::containers::string HrToString(HRESULT hr)
{
    char s_str[64] = {};
    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
    veer::containers::string output;
    output = s_str;
    return output;
}