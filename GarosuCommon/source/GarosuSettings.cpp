#include "GarosuTypedef.h"
#include "GarosuSettings.h"

namespace Garosu
{

	const String& Settings::GetLogPath(void)
	{
		const static String logPath = "garosu.log";
		return logPath;
	}

}