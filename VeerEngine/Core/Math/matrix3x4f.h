#pragma once

namespace veer
{
	class matrix3x4f
	{
	public:
		matrix3x4f();
		explicit matrix3x4f(float _value);
		matrix3x4f(const float (& _list) [12]);
        matrix3x4f(const float (& _list) [3] [4]);

        float at(size_t _row, size_t _column) const;
        float& at(size_t _row, size_t _column);

		void inverse();
		void transpose();

		friend std::ostream& operator<<(std::ostream& _os, const matrix3x4f& _matrix);
	private:
        float m_data[12];
	};
}

