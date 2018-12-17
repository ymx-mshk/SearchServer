
#include "PthreadPool.h"
#include "Pthread.h"

#include "Mylogger.h"
#include <unistd.h>

using namespace mysp;


PthreadPool::PthreadPool(size_t threadNum, size_t taskSize)
: _threadNum(threadNum)
, _taskSize(taskSize)
, _que(taskSize)
, _isExit(false){
}

PthreadPool::~PthreadPool(){
	if (!_isExit){
		stop();
	}
}

void PthreadPool::start(){
	for (size_t i = 0; i != _threadNum; ++i){
		_threads.push_back(
				shared_ptr<Pthread>
				(new Pthread(std::bind(&PthreadPool::threadFunc, this), to_string(i))));
	}

	for (auto & thread : _threads){
		thread->start();
	}

	LogInfo("PthreadPool start");
}

void PthreadPool::stop(){
	if (!_isExit){
		while (!_que.empty()){
			sleep(1);
		}
		_isExit = true;
		_que.setState(true);

		_que._notEmpty.notifyall();
		for (auto & thread : _threads){
			thread->join();
		}
	}
	LogInfo("PthreadPool stop");
}

void PthreadPool::addTask(Task task){
	_que.push(task);	
}

PthreadPool::Task PthreadPool::getTask(){
	return _que.pop();
}

void PthreadPool::threadFunc(){
	while (!_isExit){
		Task task = getTask();
	
		if (task)
			task();
	}
}
