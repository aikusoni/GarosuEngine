#pragma once
#ifndef __GAROSU_ENGINE_H__
#define __GAROSU_ENGINE_H__

#include <memory>
#include <GarosuEngineMessage.h>

namespace Garosu
{

	class Engine final
	{
	public:
		Engine(void);
		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;

		virtual ~Engine(void);

		bool SendMessage(BaseEngineMessage*);

	private:
		class impl;
		std::unique_ptr<impl> pImpl;
	};

}

#endif