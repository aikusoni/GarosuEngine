#pragma once

namespace Garosu
{
	class Worker final
	{
	public:
		Worker(void);
		Worker(const Worker&) = delete;
		Worker& operator=(const Worker&) = delete;

		~Worker(void);

		void working(void);

	private:
		class impl;
		std::unique_ptr<impl> pImpl;
	};

	class WorkerPool final
	{
	public:
		WorkerPool(int num);
		WorkerPool(const WorkerPool&) = delete;
		WorkerPool& operator=(const WorkerPool&) = delete;

		~WorkerPool(void);

		bool Initialize(void);

	private:
		class impl;
		std::unique_ptr<impl> pImpl;
	};
}