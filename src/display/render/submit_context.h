#pragma once

namespace veer::display::render
{
    class compute_technique;
    class graphics_technique;

    class submit_context
    {
    };

    class compute_submit_context : public submit_context
    {
      public:
        compute_submit_context(compute_technique &_technique);
    };

    class graphics_submit_context : public compute_submit_context
    {
      public:
        graphics_submit_context(graphics_technique &_technique);
    };
} // namespace veer::display::render