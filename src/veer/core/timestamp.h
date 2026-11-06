#pragma once

#include <veer/core/core.h>

namespace veer
{
    struct duration;

    using tick_t = int64_t;

    struct timestamp
    {
        static timestamp now();

    private:
        timestamp(tick_t _tick);

        tick_t m_tick;

        friend duration operator-(const timestamp& _from, const timestamp& _to);
    };

    struct duration
    {
        duration();

    private:
        duration(tick_t m_delta);

    public:
        double as_ns() const;
        double as_us() const;
        double as_ms() const;
        double as_s() const;
    
    private:
        double as(double _ratio) const;

    private:
        tick_t m_delta;

        friend duration operator-(const timestamp& _from, const timestamp& _to);
    };

    duration operator-(const timestamp& _to, const timestamp& _from);
} // namespace veer