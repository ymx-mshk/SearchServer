
#ifndef __MYSP_CONDITION_H__
#define __MYSP_CONDITION_H__

#include "Uncopyable.h"
#include "MutexLock.h"
#include <pthread.h>

namespace mysp
{

class Condition
: private Uncopyable{
	public:
		Condition(MutexLock & mutex)
		: _mutex(mutex){
			pthread_cond_init(&_cond, NULL);
		}

		~Condition(){
			pthread_cond_destroy(&_cond);
		}

		void wait(){
			pthread_cond_wait(&_cond, _mutex.getMutexPtr());
		}

		void notify(){
			pthread_cond_signal(&_cond);
		}

		void notifyall(){
			pthread_cond_broadcast(&_cond);
		}

	private:
		pthread_cond_t _cond;
		MutexLock & _mutex;
};

}

#endif
