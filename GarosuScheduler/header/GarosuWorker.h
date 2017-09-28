#pragma once
#ifndef __GAROSU_WORKER_H__
#define __GAROSU_WORKER_H__

namespace Garosu
{

	class WorkerGroup final
	{
	public:
		WorkerGroup(void);
		WorkerGroup(int numWorker);
		WorkerGroup(const WorkerGroup&) = delete;
		WorkerGroup& operator=(const WorkerGroup&) = delete;

		virtual ~WorkerGroup(void);

		bool Initialize(void);
		bool SetNumWorker(int numWorker);

		bool Start(void); 
		bool Stop(void);

		bool Handover(BaseTask*);

	private:
		class impl;
		std::unique_ptr<impl> pImpl;
	};

}

#endif