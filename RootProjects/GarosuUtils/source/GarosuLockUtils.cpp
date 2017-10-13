#include <GarosuTypedef.h>

#include "GarosuLockUtils.h"

#include <mutex>
#include <chrono>
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

	class SingleLock::impl
	{
	public:
		impl(void);
		impl(const impl&) = delete;
		impl& operator=(const impl&) = delete;

		virtual ~impl(void);

		std::mutex mt;
	};

	SingleLock::impl::impl(void) {}
	SingleLock::impl::~impl(void) {}

	SingleLock::SingleLock(void)
		: pImpl(mk_uptr<impl>())
	{
		pImpl->mt.lock();
	}

	SingleLock::~SingleLock(void)
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
	}

	Signal::~Signal(void)
	{

	}

	void Signal::wait(void)
	{
		auto lck = std::unique_lock<std::mutex>(pImpl->mt);
		pImpl->cv.wait(lck);
		lck.unlock();
	}

	void Signal::wait_for(u32 microSeconds)
	{
		auto lck = std::unique_lock<std::mutex>(pImpl->mt);
		pImpl->cv.wait_for(lck, std::chrono::microseconds(microSeconds));
		lck.unlock();
	}

	void Signal::notify_one(void)
	{
		pImpl->cv.notify_one();
	}

	void Signal::notify_all(void)
	{
		pImpl->cv.notify_all();
	}

}