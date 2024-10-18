#ifndef DISPLAY_COMMAND_BUFFER_H_INCLUDED
#define DISPLAY_COMMAND_BUFFER_H_INCLUDED

namespace veer
{
	class command_buffer
	{
	public:
		enum class type
		{
			Graphics,
			Compute,
			Copy,
		};

		command_buffer( command_buffer::type _type);

		virtual ~command_buffer() = 0 {};
	protected:
		type m_type;
	};
}

#endif // DISPLAY_COMMAND_BUFFER_H_INCLUDED
