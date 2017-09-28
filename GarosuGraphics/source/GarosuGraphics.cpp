#include <memory>

#include <SchedulerInterface.h>
#include "GarosuGraphics.h"

namespace Garosu {

	class Graphics::impl
	{
	public:
		impl(IScheduler* scheduler);
		virtual ~impl(void) {}

	private:
		IScheduler* mScheduler;
	};

	Graphics::impl::impl(IScheduler* scheduler) : mScheduler(scheduler) {}

	Graphics::Graphics(IScheduler* scheduler)
		: pImpl(std::make_unique<impl>(scheduler))
	{

	}

	Graphics::~Graphics(void)
	{

	}

	GraphicsError Graphics::Initialize(void)
	{

		return GraphicsError::OK;
	}
	GraphicsError Graphics::Start(void)
	{

		return GraphicsError::OK;
	}

	GraphicsError Graphics::Stop(void)
	{

		return GraphicsError::OK;
	}

	GraphicsError Graphics::SendMessage(GraphicsMessageId messageId)
	{

		return GraphicsError::OK;
	}

	GraphicsError Graphics::RegisterCallback(IGraphicsCallback* callback)
	{

		return GraphicsError::OK;
	}

}