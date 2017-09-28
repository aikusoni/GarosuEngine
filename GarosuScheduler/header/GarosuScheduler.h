#pragma once
#ifndef __GAROSU_SCHEDULER_H__
#define __GAROSU_SCHEDULER_H__

namespace Garosu
{

	class Scheduler
	{
	public:
		Scheduler(void);
		Scheduler(const Scheduler&) = delete;
		Scheduler& operator=(const Scheduler&) = delete;

		virtual ~Scheduler(void);

		bool Initialize(void);
		bool HandoverTask(std::unique_ptr<BaseTask> newTask);

	private:
		class impl;
		std::unique_ptr<impl> pImpl;
	};

}

#endif