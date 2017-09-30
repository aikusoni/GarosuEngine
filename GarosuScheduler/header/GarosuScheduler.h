#pragma once
#ifndef __GAROSU_SCHEDULER_H__
#define __GAROSU_SCHEDULER_H__

namespace Garosu
{

	enum class SchedulerError
	{
		OK,
		ERROR
	};

	class Scheduler : public IScheduler
	{
	public:
		Scheduler(void);
		Scheduler(const Scheduler&) = delete;
		Scheduler& operator=(const Scheduler&) = delete;

		virtual ~Scheduler(void);

		SchedulerError Initialize(void);
		bool HandoverTask(uptr<BaseTask> newTask);

	private:
		class impl;
		uptr<impl> pImpl;
	};

}

#endif