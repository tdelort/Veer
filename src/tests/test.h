#pragma once

#include <core/debug.h>

#include <functional>
#include <vector>

namespace veer::tests
{
    enum class result
    {
        succeeded = 0,
        failed,
    };
} // namespace veer::tests

#define VEER_RUN_TEST(_expr)                                                                                           \
    if ((_expr) == veer::tests::result::failed)                                                                        \
    {                                                                                                                  \
        VEER_LOG_ERROR("Test run failed : " << #_expr << "!");                                                         \
        VEER_BREAKPOINT();                                                                                             \
        return veer::tests::result::failed;                                                                            \
    }

#define VEER_TEST_ASSERT(_expr)                                                                                        \
    if (!(_expr))                                                                                                      \
    {                                                                                                                  \
        VEER_LOG_ERROR("Test assert failed : " << #_expr);                                                             \
        VEER_BREAKPOINT();                                                                                             \
        return veer::tests::result::failed;                                                                            \
    }