
#include "Pthread.h"

namespace mysp
{

__thread const char * pthreadName;

struct PthreadData{
	using PthreadCallBack = Pthread::PthreadCallBack;

	PthreadData(const std::string & name, const PthreadCallBack & cb)
	: _name(name)
	, _cb(cb){
	}

	void runInLoop(){
		pthreadName = _name.size() ? _name.c_str() : "myThread";
		if (_cb){
			_cb();
		}
	}

	std::string _name;
	PthreadCallBack _cb;
};

Pthread::Pthread(PthreadCallBack && cb, const std::string & name)
: _pthid(0)
, _name(name)
, _isRunning(false)
, _cb(std::move(cb)){
}

void Pthread::start(){
	PthreadData * pdata = new PthreadData(_name, _cb);
	if (!_isRunning){
		pthread_create(&_pthid, NULL, pthreadFunc, pdata);
		_isRunning = true;
	}
}

void Pthread::join(){
	if (_isRunning){
		pthread_join(_pthid, NULL);
		_isRunning = false;
	}
}

Pthread::~Pthread(){
	if (_isRunning){
		pthread_detach(_pthid);
		_isRunning = false;
	}
}

void * Pthread::pthreadFunc(void * arg){
	PthreadData *pdata = static_cast<PthreadData *>(arg);	
	if (pdata)
		pdata->runInLoop();

	delete pdata;
	return NULL;
}

}
