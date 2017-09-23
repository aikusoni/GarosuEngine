#pragma once

namespace Garosu
{

	class WorkerGroup final
	{
	public:
		WorkerGroup(int numWorker);
		WorkerGroup(const WorkerGroup&) = delete;
		WorkerGroup& operator=(const WorkerGroup&) = delete;

		virtual ~WorkerGroup(void);

		bool Initialize(void);

		bool Start(void); 
		bool Stop(void);

	private:
		class impl;
		std::unique_ptr<impl> pImpl;
	};

}