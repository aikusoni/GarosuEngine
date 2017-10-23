#include <GarosuTypedef.h>

#include <ostream>
#include <chrono>

#include "GarosuStringUtils.h"

#include <iomanip>

namespace Garosu
{

	std::ostream& operator<<(std::ostream& os, std::chrono::system_clock::time_point& tp)
	{
		auto microSec = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch());
		std::time_t ct = std::chrono::system_clock::to_time_t(tp);

		struct tm t;
		localtime_s(&t, &ct);

		os << std::put_time(&t, "%F %T") << "." << std::setfill('0') << std::setw(6) << (microSec.count() % 1000000);

		return os;
	}

}