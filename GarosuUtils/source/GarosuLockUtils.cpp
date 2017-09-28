#include <memory>

#include "GarosuLockUtils.h"

#include <mutex>
#include <condition_variable>

namespace Garosu
{

	class Signal::impl
	{
	public:
		std::mutex mt;
		std::unique_lock<std::mutex> lck;
		std::condition_variable cv;
	};

	Signal::Signal(void)
		: pImpl(std::make_unique<impl>())
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