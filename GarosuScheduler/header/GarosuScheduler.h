#pragma once
#ifndef __GAROSU_SCHEDULER_H__
#define __GAROSU_SCHEDULER_H__

namespace Garosu
{

	class SchedulerFactory
	{
	public:
		static SchedulerError MakeDefaultScheduler(IScheduler** scheduler, u32 numThread);
		static SchedulerError MakeBalancedScheduler(IScheduler** scheduler, u32 numThread);
	};

}

#endif