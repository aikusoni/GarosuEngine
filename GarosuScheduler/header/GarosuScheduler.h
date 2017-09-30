#pragma once
#ifndef __GAROSU_SCHEDULER_H__
#define __GAROSU_SCHEDULER_H__

namespace Garosu
{

	class SchedulerFactory
	{
	public:
		static IScheduler* MakeDefaultScheduler(u32 numThread);
	};

}

#endif