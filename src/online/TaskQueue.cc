
#include "TaskQueue.h"

using namespace mysp;

TaskQueue::TaskQueue(size_t size)
: _size(size)
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex){
}

bool TaskQueue::empty() const{
	return 0 == _que.size();	
}

bool TaskQueue::full() const{
	return _size == _que.size();	
}

void TaskQueue::push(Task value){
	{
		AutoLock lock(_mutex);

		while (full()){
			_notFull.wait();
		}

		_que.push(value);
	}

	_notEmpty.notify();
}

TaskQueue::Task TaskQueue::pop(){
	Task  value;
	{
		AutoLock lock(_mutex);

		while (empty()){
			_notEmpty.wait();
			if(_destroy){
				value = NULL;
				return value;
			}
		}

		value = _que.front();
		_que.pop();
	}

	_notFull.notify();
	return value;
}
		
void TaskQueue::setState(bool state){
	_destroy = state;
}

