#pragma once
#ifndef __GAROSU_ENGINE_H__
#define __GAROSU_ENGINE_H__

#include <memory>
#include <GarosuEngineMessage.h>

namespace Garosu
{

	class IEngine
	{
	public:
		virtual bool Initialize(void) = 0;
		virtual bool Finalize(void) = 0;

		virtual bool SendMessage(BaseMessage*) = 0;
	};

	// TODO Make Callback
}

extern "C" {
	__declspec(dllexport) Garosu::IEngine* MakeGarosuEngine(void);
}

#endif