#pragma once

namespace Garosu
{

	class MathUtils
	{
	public:
		template <typename T>
		inline static T Clamp(T n, T min, T max)
		{
			return (n > min ? (n < max ? n : max) : min);
		}
	};

}