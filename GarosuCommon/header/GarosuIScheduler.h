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

	enum class TaskSource
	{
		Undefined = 0,
		Graphics,
		Physics,
		Count,
	};

	class IScheduler
	{
	public:
		virtual SchedulerError Initialize(void) = 0;
		virtual SchedulerError Finalize(void) = 0;

		virtual SchedulerError HandoverTask(TaskSource taskSource, BaseTask* newTask) = 0;
	};

}

#endif