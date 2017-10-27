#include <GarosuTypedef.h>

#include <GarosuTask.h>
#include <GarosuIScheduler.h>
#include <GarosuIGraphics.h>

#include "GarosuGraphics.h"

namespace Garosu {

	class Graphics : public IGraphics
	{
	public:
		Graphics(IScheduler*);
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;

		~Graphics(void);

		virtual GraphicsError Initialize(void);
		virtual GraphicsError Finalize(void);

		virtual GraphicsError SendMessage(GraphicsMessage* message);
		virtual GraphicsError RegisterCallback(GraphicsCallback* callback);

		IScheduler* mScheduler;
	};

	Graphics::Graphics(IScheduler* scheduler)
		: mScheduler(scheduler)
	{

	}

	Graphics::~Graphics(void)
	{

	}

	GraphicsError Graphics::Initialize(void)
	{

		return GraphicsError::OK;
	}

	GraphicsError Graphics::Finalize(void)
	{
		
		return GraphicsError::OK;
	}

	GraphicsError Graphics::SendMessage(GraphicsMessage* message)
	{
		switch (message->mEvtId)
		{
		case GraphicsMessageId::None:
			return GraphicsError::OK;
			
		case GraphicsMessageId::SetVideoHandle:
		{
			// TODO
		}
			return GraphicsError::OK;
			
		}

		return GraphicsError::ERROR_MESSAGE_NOT_HANDLED;
	}

	GraphicsError Graphics::RegisterCallback(GraphicsCallback* callback)
	{

		return GraphicsError::OK;
	}

	IGraphics* GraphicsFactory::MakeDefaultGraphics(IScheduler* scheduler)
	{
		return new Graphics(scheduler);
	}

}