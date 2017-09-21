#pragma once

namespace Garosu
{

	class Signal
	{
	public:
		Signal(void);
		Signal(const Signal&) = delete;
		Signal& operator=(const Signal&) = delete;

		virtual ~Signal(void);

		void wait(void);
		void notify(void);

	private:
		class impl;
		std::unique_ptr<impl> pImpl;
	};

}