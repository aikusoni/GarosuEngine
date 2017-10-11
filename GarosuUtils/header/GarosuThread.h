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

		/*
		* OnBegin() : called at thread starting.
		* OnLoop() : called when every loop.
		* OnEnd() : called at thread ending.
		*/
		virtual bool OnBegin(void) { return true; } // if OnBegin() returns false, the loop will not work;
		virtual bool OnLoop(void) = 0; // if OnLoop() returns false, the loop will exit;
		virtual bool OnEnd(void) { return true; }

	private:
		class impl;
		uptr<impl> pImpl;
	};

	class ThreadUtils
	{
	public:
		static void SleepFor(u32 microSeconds);
		static u32 GetConcurrencyCount(void);
		static String GetThreadId(void);
	};

}

#endif