#pragma once

namespace Garosu
{
	class Task
	{
	public:
		virtual void DoWork(void) = 0;
	};

	class Scheduler
	{
	public:
		Scheduler(void);
		Scheduler(const Scheduler&) = delete;
		Scheduler& operator=(const Scheduler&) = delete;

		virtual ~Scheduler(void);

		bool Initialize(void);
		bool PutTask(std::unique_ptr<Task> newTask);

	private:
		class impl;
		std::unique_ptr<impl> pImpl;
	};

}