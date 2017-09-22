#pragma once
#include <iostream>

namespace Garosu
{

	class BaseTask
	{
	public:
		virtual void DoWork(void) { std::cout << "this is base worker." << std::endl; };
	};

	class BaseThread
	{
	public:
		BaseThread(BaseTask&);
		BaseThread(const BaseThread&) = delete;
		BaseThread& operator=(const BaseThread&) = delete;

		virtual ~BaseThread(void);

		virtual void Start(void) final;
		virtual void Join(void) final;

	protected:

	private:
		BaseTask& mBaseWorker;

		class impl;
		std::unique_ptr<impl> pImpl;
	};

	class ThreadUtils
	{
	public:
		static void SleepFor(int nanoSeconds);
	};

}