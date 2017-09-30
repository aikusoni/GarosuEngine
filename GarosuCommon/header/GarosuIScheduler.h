#pragma once
#ifndef __GAROSU_I_SCHEDULER_H__
#define __GAROSU_I_SCHEDULER_H__

namespace Garosu
{
	enum class SchedulerError
	{
		OK,
		ERROR
	};

	class IScheduler
	{
	public:
		virtual SchedulerError Initialize(void) = 0;
		virtual SchedulerError HandoverTask(uptr<BaseTask> newTask) = 0;
	};

}

#endif