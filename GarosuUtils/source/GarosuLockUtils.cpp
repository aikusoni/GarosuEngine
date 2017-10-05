#include <GarosuTypedef.h>

#include "GarosuLockUtils.h"

#include <mutex>
#include <condition_variable>

namespace Garosu
{

	class Locker::impl
	{
	public:
		impl(void);
		impl(const impl&) = delete;
		impl& operator=(const impl&) = delete;

		virtual ~impl(void);

		std::mutex mt;
	};

	Locker::impl::impl(void)
	{
		
	}

	Locker::impl::~impl(void)
	{

	}

	Locker::Locker(void)
		: pImpl(mk_uptr<impl>())
	{
	}

	Locker::~Locker(void)
	{

	}

	void Locker::Lock(void)
	{
		pImpl->mt.lock();
	}

	void Locker::Unlock(void)
	{
		pImpl->mt.unlock();
	}

	class Signal::impl
	{
	public:
		impl(void);
		impl(const impl&) = delete;
		impl& operator=(const impl&) = delete;

		virtual ~impl(void);

		std::mutex mt;
		std::unique_lock<std::mutex> lck;
		std::condition_variable cv;
	};

	Signal::impl::impl(void)
	{

	}

	Signal::impl::~impl(void)
	{

	}

	Signal::Signal(void)
		: pImpl(mk_uptr<impl>())
	{
		pImpl->lck = std::unique_lock<std::mutex>(pImpl->mt);
	}

	Signal::~Signal(void)
	{

	}

	void Signal::wait(void)
	{
		pImpl->cv.wait(pImpl->lck);
	}

	void Signal::notify(void)
	{
		pImpl->cv.notify_all();
	}

}