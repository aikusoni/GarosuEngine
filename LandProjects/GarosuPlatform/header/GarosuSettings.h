#pragma once
#ifndef __GAROSU_SETTINGS_H__
#define __GAROSU_SETTINGS_H__

namespace Garosu
{

	class Settings
	{
	public:
		static void SetAppPath(String);
		static String GetLogPath(void);
	};

}

#endif