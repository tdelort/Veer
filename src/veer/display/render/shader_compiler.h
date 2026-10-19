#pragma once

#include <veer/core/containers/span.h>
#include <veer/core/core.h>

#include <veer/display/render/shader_source.h>

namespace veer::display::render
{
    class shader_compiler
    {
    public:
        virtual ~shader_compiler() = 0;

        // simple implem for now
        virtual shader_code_memory_blob_t compile(veer::containers::span<const char> _source, const char* _entry_point, const char* _debug_name, shader_stage_type _stage_type) = 0;
    };
} // namespace veer::display::render