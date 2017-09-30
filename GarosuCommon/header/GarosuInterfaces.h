#pragma once
#ifndef __SCHEDULER_INTERFACE_H__
#define __SCHEDULER_INTERFACE_H__

namespace Garosu
{

	class IScheduler
	{
	public:
		virtual bool HandoverTask(uptr<BaseTask> newTask) = 0;		
	};

}

#endif