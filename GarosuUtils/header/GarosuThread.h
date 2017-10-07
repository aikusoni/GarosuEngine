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

		virtual void Start(void) final;
		virtual void Join(void) final;

	protected:

	private:
		BaseWorker* mBaseWorker;

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