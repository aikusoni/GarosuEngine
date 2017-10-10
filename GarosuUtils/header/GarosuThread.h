#pragma once
#ifndef __GAROSU_THREAD_H__
#define __GAROSU_THREAD_H__

#include <iostream>

namespace Garosu
{
	
	class BaseWorker
	{
	public:
		virtual void DoWork(void) = 0;
	};

	class BaseThread
	{
	public:
		BaseThread(BaseWorker*);
		BaseThread(const BaseThread&) = delete;
		BaseThread& operator=(const BaseThread&) = delete;

		virtual ~BaseThread(void);

		virtual void Start(void);
		virtual void Join(void);

	private:
		BaseWorker* mBaseWorker;

		class impl;
		uptr<impl> pImpl;
	};

	class LoopThread : public BaseThread
	{
	public:
		LoopThread(BaseWorker* onLoop);
		LoopThread(const LoopThread&) = delete;
		LoopThread& operator=(const LoopThread&) = delete;

		virtual ~LoopThread(void);

		virtual void Start(void);
		virtual void Stop(void);
		
	private:
		class impl;
		uptr<impl> pImpl;
	};

	class ThreadUtils
	{
	public:
		static void SleepFor(u32 microSeconds);
		static u32 GetConcurrencyCount(void);
	};

}

#endif