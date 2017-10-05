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

		~Locker(void);

		void Lock(void);
		void Unlock(void);

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
		void notify(void);

	private:
		class impl;
		uptr<impl> pImpl;
	};

}

#endif