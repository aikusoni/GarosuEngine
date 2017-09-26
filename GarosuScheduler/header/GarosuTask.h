#pragma once

namespace Garosu
{

	class BaseTask
	{
	public:
		virtual ~BaseTask(void) = 0;
		virtual void DoTask(void) = 0;
	};

	class LambdaTask : public BaseTask
	{
	public:
		using LambdaFnc = std::function<void(void)>;

		LambdaTask(LambdaFnc lambda)
			: mLambda(lambda)
		{

		}

		virtual ~LambdaTask(void) {}

		virtual void DoTask(void)
		{
			mLambda();
		}

		LambdaFnc mLambda;
	};

}