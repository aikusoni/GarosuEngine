#include <GarosuTypedef.h>

#include "GarosuSettings.h"

namespace Garosu
{
	static String appPath = "";
	static String logFileName = "garosu.log";

	void Settings::SetAppPath(String filePath)
	{
		appPath = filePath;
	}

	String Settings::GetLogPath(void)
	{
		return appPath + logFileName;
	}

}