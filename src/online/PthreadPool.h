
#ifndef __MYSP_PTHREADPOOL_H__
#define __MYSP_PTHREADPOOL_H__

#include "TaskQueue.h"
#include <memory>
#include <vector>
using namespace std;

namespace mysp
{

class Pthread;

class PthreadPool{
	public:
		using Task = TaskQueue::Task;

		PthreadPool(size_t, size_t);
		~PthreadPool();

		void start();
		void stop();

		void addTask(Task);
	private:
		Task getTask();
		void threadFunc();

	private:
		size_t _threadNum;
		size_t _taskSize;
		vector<shared_ptr<Pthread>> _threads;
		TaskQueue _que;
		bool _isExit;
};

}

#endif
