#include "dx12_shader_compiler.h"

#include "display/render/shader_source.h"
#include "dx12_pch.h"
#include <cstring>

namespace veer::display::render
{
    dx12_shader_compiler::~dx12_shader_compiler()
    {

    }

    shader_code_memory_blob_t dx12_shader_compiler::compile(veer::containers::span<const char> _source, const char* _entry_point, const char* _debug_name, shader_stage_type _stage_type)
    {
#if defined(_DEBUG)
        // Enable better shader debugging with the graphics debugging tools.
        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
        UINT compileFlags = 0;
#endif

        veer::containers::static_array<const char*, static_cast<size_t>(shader_stage_type::COUNT)> s_targets =
        {
            "vs_5_0", // vertex 
            "ps_5_0", // pixel
            "cs_5_0"  // compute
        };
        static_assert(static_cast<size_t>(shader_stage_type::COUNT) == 3u, "You need to update s_targets array");

        shader_code_memory_blob_t shader_code;
        {
            ID3DBlob* code_blob;
            ID3DBlob* error_blob;
            HRESULT hr = D3DCompile(_source.data(), _source.size(), _debug_name, nullptr, nullptr, _entry_point, s_targets[static_cast<size_t>(_stage_type)], compileFlags, 0, &code_blob, &error_blob);
            VEER_ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader." ); 
            if(error_blob != nullptr)
            {
                VEER_LOG( "Shader compilation output : " << (const char*)error_blob->GetBufferPointer() );
                error_blob->Release();
            }

            if (code_blob != nullptr)
            {
                shader_code.resize(code_blob->GetBufferSize(), 0);
                std::memcpy(shader_code.data(), code_blob->GetBufferPointer(), code_blob->GetBufferSize());
                code_blob->Release();
            }
        }

        return shader_code;
    }
}