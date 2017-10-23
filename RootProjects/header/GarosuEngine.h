#pragma once
#ifndef __GAROSU_ENGINE_H__
#define __GAROSU_ENGINE_H__

#include <memory>
#include <GarosuEngineMessage.h>

namespace Garosu
{
	using EngineCallback = bool(int param1);

	class IEngine
	{
	public:
		virtual bool Initialize(void) = 0;
		virtual bool Finalize(void) = 0;

		virtual bool SendMessage(BaseMessage*) = 0;
		virtual bool RegisterCallback(EngineCallback* callback) = 0;
	};

	// TODO Make Callback
}

extern "C" {
	__declspec(dllexport) Garosu::IEngine* MakeGarosuEngine(void);
	__declspec(dllexport) bool DeleteGarosuEngine(Garosu::IEngine*);

	// IEngine Functions
	__declspec(dllexport) bool Initialize(Garosu::IEngine*);
	__declspec(dllexport) bool Finalize(Garosu::IEngine*);
	__declspec(dllexport) bool SendMessage(Garosu::IEngine*, Garosu::BaseMessage*);
	__declspec(dllexport) bool RegisterCallback(Garosu::IEngine*, Garosu::EngineCallback*);
}

#endif