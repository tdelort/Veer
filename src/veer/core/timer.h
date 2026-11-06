#pragma once

#include <veer/core/timestamp.h>

namespace veer
{
    struct timer
    {
        duration get()
        {
            return timestamp::now() - m_start;
        }

    private:
        timestamp m_start{timestamp::now()};
    };
} // namespace veer