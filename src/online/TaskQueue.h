
#ifndef __MYSP_TASKQUEUE_H__
#define __MYSP_TASKQUEUE_H__

#include "MutexLock.h"
#include "Condition.h"
#include <queue>
#include <functional>
using namespace std;

namespace mysp
{

class TaskQueue{
	using Task = function<void()>;

	public:
		friend class PthreadPool;

		TaskQueue(size_t);

		bool empty() const;
		bool full() const;

		void push(Task);
		Task pop();

		void setState(bool);
	private:
		size_t _size;
		queue<Task> _que;
		MutexLock _mutex;
		Condition _notEmpty;
		Condition _notFull;
		bool _destroy;
};

}

#endif
