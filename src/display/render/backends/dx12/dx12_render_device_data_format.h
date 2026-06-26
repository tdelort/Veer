#pragma once

#include "core/core.h"
#include "dx12_pch.h"
#include <display/render/render_device_data_format.h>
#include <dxgiformat.h>

namespace veer::display::render
{
    static inline DXGI_FORMAT s_convert(render_device_data_format _format)
    {
        DXGI_FORMAT s_conversionTable[] =  
        {
            DXGI_FORMAT_UNKNOWN,                // unknown,

            // 8bit
            DXGI_FORMAT_R8_UNORM, 			    // r8_unorm,
            DXGI_FORMAT_R8_SNORM, 		    	// r8_snorm,
            DXGI_FORMAT_R8_UINT, 			    // r8_uint,
            DXGI_FORMAT_R8_SINT, 			    // r8_sint,
            DXGI_FORMAT_R8G8_UNORM, 		    // r8g8_unorm,
            DXGI_FORMAT_R8G8_SNORM, 		    // r8g8_snorm,
            DXGI_FORMAT_R8G8_UINT, 			    // r8g8_uint,
            DXGI_FORMAT_R8G8_SINT, 			    // r8g8_sint,
            DXGI_FORMAT_R8G8B8A8_UNORM, 	    // r8g8b8a8_unorm,
            DXGI_FORMAT_R8G8B8A8_SNORM, 	    // r8g8b8a8_snorm,
            DXGI_FORMAT_R8G8B8A8_UINT, 		    // r8g8b8a8_uint,
            DXGI_FORMAT_R8G8B8A8_SINT, 		    // r8g8b8a8_sint,
            DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,    // r8g8b8a8_unorm_srgb,

            // 16bit
            DXGI_FORMAT_R16_FLOAT, 			// r16_float,
            DXGI_FORMAT_D16_UNORM, 			// d16_unorm,
            DXGI_FORMAT_R16_UNORM, 			// r16_unorm,
            DXGI_FORMAT_R16_SNORM, 			// r16_snorm,
            DXGI_FORMAT_R16_UINT, 			// r16_uint,
            DXGI_FORMAT_R16_SINT, 			// r16_sint,
            DXGI_FORMAT_R16G16_FLOAT, 		// r16g16_float,
            DXGI_FORMAT_R16G16_UNORM, 		// r16g16_unorm,
            DXGI_FORMAT_R16G16_SNORM, 		// r16g16_snorm,
            DXGI_FORMAT_R16G16_UINT, 		// R16g16_uint,
            DXGI_FORMAT_R16G16_SINT, 		// R16g16_sint,
            DXGI_FORMAT_R16G16B16A16_FLOAT, // r16g16b16a16_float,
            DXGI_FORMAT_R16G16B16A16_UNORM, // r16g16b16a16_unorm,
            DXGI_FORMAT_R16G16B16A16_SNORM, // r16g16b16a16_snorm,
            DXGI_FORMAT_R16G16B16A16_UINT, 	// r16g16b16a16_uint,
            DXGI_FORMAT_R16G16B16A16_SINT, 	// r16g16b16a16_sint,

            // 32bit
            DXGI_FORMAT_D32_FLOAT, 			// d32_float,
            DXGI_FORMAT_R32_FLOAT, 			// r32_float,
            DXGI_FORMAT_R32_UINT, 			// r32_uint,
            DXGI_FORMAT_R32_SINT, 			// r32_sint,
            DXGI_FORMAT_R32G32_FLOAT, 		// r32g32_float,
            DXGI_FORMAT_R32G32_UINT, 		// r32g32_uint,
            DXGI_FORMAT_R32G32_SINT, 		// r32g32_sint,
            DXGI_FORMAT_R32G32B32_FLOAT, 	// r32g32b32_float,
            DXGI_FORMAT_R32G32B32_UINT, 	// r32g32b32_uint,
            DXGI_FORMAT_R32G32B32_SINT, 	// r32g32b32_sint,
            DXGI_FORMAT_R32G32B32A32_FLOAT, // r32g32b32a32_float,
            DXGI_FORMAT_R32G32B32A32_UINT, 	// r32g32b32a32_uint,
            DXGI_FORMAT_R32G32B32A32_SINT, 	// r32g32b32a32_sint,
            
            // BCn
            DXGI_FORMAT_BC1_UNORM, 		// bc1_unorm,
            DXGI_FORMAT_BC1_UNORM_SRGB, // bc1_unorm_srgb,
            DXGI_FORMAT_BC2_UNORM, 		// bc2_unorm,
            DXGI_FORMAT_BC2_UNORM_SRGB, // bc2_unorm_srgb,
            DXGI_FORMAT_BC3_UNORM, 		// bc3_unorm,
            DXGI_FORMAT_BC3_UNORM_SRGB, // bc3_unorm_srgb,
            DXGI_FORMAT_BC4_UNORM, 		// bc4_unorm,
            DXGI_FORMAT_BC4_SNORM, 		// bc4_snorm,
            DXGI_FORMAT_BC5_UNORM, 		// bc5_unorm,
            DXGI_FORMAT_BC5_SNORM, 		// bc5_snorm,
            DXGI_FORMAT_BC6H_SF16, 		// bc6h_float,
            DXGI_FORMAT_BC6H_UF16, 		// bc6h_ufloat,
            DXGI_FORMAT_BC7_UNORM, 		// bc7_unorm,
            DXGI_FORMAT_BC7_UNORM_SRGB, // bc7_unorm_srgb,

            // misc
            DXGI_FORMAT_D24_UNORM_S8_UINT,  // d24_unorm_s8_stencil,
            DXGI_FORMAT_R11G11B10_FLOAT, 	// r11g11b10_float,
            DXGI_FORMAT_R10G10B10A2_UNORM,  // r10g10b10a2_unorm,
            DXGI_FORMAT_R10G10B10A2_UINT, 	// r10g10b10a2_uint,
            DXGI_FORMAT_R9G9B9E5_SHAREDEXP, // r9g9b9e5_sharedexp
        };
        static_assert( VEER_STATIC_ARRAY_SIZE( s_conversionTable ) == static_cast<size_t>( render_device_data_format::COUNT ) );

        return s_conversionTable[static_cast<size_t>(_format)];
    };
}