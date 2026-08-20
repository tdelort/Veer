#pragma once

namespace veer::display::render
{
    class submit_context
    {

    };

    class compute_submit_context : public submit_context
    {

    };

    class graphics_submit_context: public compute_submit_context
    {

    };
}