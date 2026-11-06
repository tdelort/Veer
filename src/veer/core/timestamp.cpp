#include "timestamp.h"
#include "core/timestamp.h"

#include <veer/core/debug.h>
#include <veer/core/core.h>

#if !defined(VEER_OS_WINDOWS)
    #error not implemented
#endif // !defined(VEER_OS_WINDOWS)

namespace veer
{
    static tick_t s_init_performance_counter_frequency() noexcept
    {
        LARGE_INTEGER large_integer;
        if (QueryPerformanceFrequency(&large_integer))
            return static_cast<tick_t>(large_integer.QuadPart);
        return static_cast<tick_t>(0ll);
    }

    // In Hz
    static const tick_t s_performance_counter_frequency = s_init_performance_counter_frequency();

    timestamp::timestamp(tick_t _tick)
        : m_tick{_tick}
    {
    }

    timestamp timestamp::now()
    {
        LARGE_INTEGER large_integer;
        const bool res = QueryPerformanceCounter(&large_integer);
        // From the docs : On systems that run Windows XP or later, the function will always succeed when given valid parameters and will thus never return zero.
        VEER_ASSERT(res, "Hardware does not support performance counters !");
        return timestamp(static_cast<tick_t>(large_integer.QuadPart));
    }

    duration::duration()
        : m_delta{0}
    {
    }

    duration::duration(tick_t _delta)
        : m_delta{_delta}
    {
    }

    double duration::as_ns() const
    {
        static constexpr double s_ns_to_s_ratio = 1e9;
        static double s_tick_to_ns = s_ns_to_s_ratio/ static_cast<double>(s_performance_counter_frequency);

        return as(s_tick_to_ns);
    }

    double duration::as_us() const
    {
        static constexpr double s_us_to_s_ratio = 1e6;
        static double s_tick_to_us = s_us_to_s_ratio / static_cast<double>(s_performance_counter_frequency);

        return as(s_tick_to_us);
    }

    double duration::as_ms() const
    {
        static constexpr int64_t s_ms_to_s_ratio = 1e3;
        static double s_tick_to_ms = s_ms_to_s_ratio / static_cast<double>(s_performance_counter_frequency);

        return as(s_tick_to_ms);
    }

    double duration::as_s() const
    {
        static double s_tick_to_s = 1.0 / static_cast<double>(s_performance_counter_frequency);

        return as(s_tick_to_s);
    }

    double duration::as(double _ratio) const
    {
        return static_cast<double>(m_delta) * _ratio;
    }

    duration operator-(const timestamp& _lhs, const timestamp& _rhs)
    {
        return duration(_lhs.m_tick - _rhs.m_tick);
    }
} // namespace veer