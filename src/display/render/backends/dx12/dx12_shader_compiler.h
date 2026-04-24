#pragma once

#include "display/render/shader_compiler.h"
#include "display/render/shader_source.h"

namespace veer::display::render
{
    class dx12_shader_compiler : public shader_compiler
    {
    public:
        virtual ~dx12_shader_compiler();
        shader_code_memory_blob_t compile( veer::containers::span<const char> _source, const char* _entry_point, const char* _debug_name, shader_stage_type _stage_type) override;
    };
}