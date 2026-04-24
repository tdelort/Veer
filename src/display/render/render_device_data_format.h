#pragma once

namespace veer::display::render
{
	// very DX12 but I like how they name them
	// (things like RGBA_FLOAT16 would be more concise but when you introduceformats with different 
	// sizes per channel like R11G11B10 or R10G10B10A2, you lose the consistency)
	// Not all listed
	// TODO : check device capabilities
	// TODO : check other api types
	enum class render_device_data_format
	{
		unknown,

		// 8bit
		r8_unorm,
		r8_snorm,
		r8_uint,
		r8_sint,
		r8g8_unorm,
		r8g8_snorm,
		r8g8_uint,
		r8g8_sint,
		r8g8b8a8_unorm,
		r8g8b8a8_snorm,
		r8g8b8a8_uint,
		r8g8b8a8_sint,
		r8g8b8a8_unorm_srgb,

		// 16bit
		r16_float,
		d16_unorm,
		r16_unorm,
		r16_snorm,
		r16_uint,
		r16_sint,
		r16g16_float,
		r16g16_unorm,
		r16g16_snorm,
		r16g16_uint,
		r16g16_sint,
		r16g16b16a16_float,
		r16g16b16a16_unorm,
		r16g16b16a16_snorm,
		r16g16b16a16_uint,
		r16g16b16a16_sint,

		// 32bit
		d32_float,
		r32_float,
		r32_uint,
		r32_sint,
		r32g32_float,
		r32g32_uint,
		r32g32_sint,
		r32g32b32_float,
		r32g32b32_uint,
		r32g32b32_sint,
		r32g32b32a32_float,
		r32g32b32a32_uint,
		r32g32b32a32_sint,
		
		// bcn
		bc1_unorm,
		bc1_unorm_srgb,
		bc2_unorm,
		bc2_unorm_srgb,
		bc3_unorm,
		bc3_unorm_srgb,
		bc4_unorm,
		bc4_snorm,
		bc5_unorm,
		bc5_snorm,
		bc6h_float,
		bc6h_ufloat,
		bc7_unorm,
		bc7_unorm_srgb,

		// misc
		d24_unorm_s8_stencil,
		r11g11b10_float,
		r10g10b10a2_unorm,
		r10g10b10a2_uint,
		r9g9b9e5_sharedexp,

		// useful for static_asserts
		COUNT,
	};
}