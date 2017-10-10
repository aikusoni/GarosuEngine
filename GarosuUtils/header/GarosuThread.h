#pragma once
#ifndef __GAROSU_THREAD_H__
#define __GAROSU_THREAD_H__

#include <iostream>

namespace Garosu
{
	
	class BaseThread
	{
	public:
		BaseThread(void);
		BaseThread(const BaseThread&) = delete;
		BaseThread& operator=(const BaseThread&) = delete;

		virtual ~BaseThread(void);

		void Start(void);
		void Join(void);

		virtual void DoWork(void) = 0;

	private:
		class impl;
		uptr<impl> pImpl;
	};

	class LoopThread : public BaseThread
	{
	public:
		LoopThread(void);
		LoopThread(const LoopThread&) = delete;
		LoopThread& operator=(const LoopThread&) = delete;

		virtual ~LoopThread(void);

		void Start(void);
		void Stop(void);

		virtual void DoWork(void);

		virtual void OnLoop(void) = 0;

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