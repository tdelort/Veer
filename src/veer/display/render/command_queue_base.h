
#include <veer/core/containers/resizable_array.h>
#include <veer/core/containers/span.h>
#include <veer/core/unique_ptr.h>

#include <veer/display/render/command_buffer.h>

namespace veer::display::render
{
    // common code between all backends
    class command_queue_base
    {
    public:
        virtual void enqueue(command_buffer&& _command_buffers);

        virtual void signal(uint64_t _value);
        virtual void wait_for_value(uint64_t _value);

        virtual ~command_queue_base() = 0;

    protected:
        uint64_t m_last_signaled_fence_value{0u};

    private:
        struct executed_buffer
        {
            uint64_t m_fence_value_execution;
            command_buffer m_buffer;
        };

        containers::resizable_array<executed_buffer> m_executed_buffers;
    };
} // namespace veer::display::render