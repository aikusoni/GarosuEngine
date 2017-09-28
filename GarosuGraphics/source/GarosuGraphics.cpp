#include <memory>
#include "GarosuGraphics.h"

namespace Garosu {

	class Graphics::impl
	{

	};

	Graphics::Graphics(void)
		: pImpl(std::make_unique<impl>())
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