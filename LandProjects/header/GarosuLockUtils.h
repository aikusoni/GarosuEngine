#pragma once
#ifndef __GAROSU_LOCK_UTILS_H__
#define __GAROSU_LOCK_UTILS_H__

namespace Garosu
{

	class Locker
	{
	public:
		Locker(void);
		Locker(const Locker&) = delete;
		Locker& operator=(const Locker&) = delete;

		virtual ~Locker(void);

		virtual void Lock(void);
		virtual void Unlock(void);

	private:
		class impl;
		uptr<impl> pImpl;
	};

	class SingleLock
	{
	public:
		SingleLock(void);
		SingleLock(const SingleLock&) = delete;
		SingleLock& operator=(const SingleLock&) = delete;

		virtual ~SingleLock(void);

	private:
		class impl;
		uptr<impl> pImpl;
	};

	class Signal
	{
	public:
		Signal(void);
		Signal(const Signal&) = delete;
		Signal& operator=(const Signal&) = delete;

		virtual ~Signal(void);

		void wait(void);
		void wait_for(u32 microSeconds);
		void notify_one(void);
		void notify_all(void);

	private:
		class impl;
		uptr<impl> pImpl;
	};

}

#endif