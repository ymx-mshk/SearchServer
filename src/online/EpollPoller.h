
#ifndef __MYSP_EPOLLPOLLER_H__
#define __MYSP_EPOLLPOLLER_H__

#include "MutexLock.h"
#include "TcpConnection.h"
#include "sys/epoll.h"
#include <vector>
#include <map>
using namespace std;

namespace mysp
{

class Acceptor;

class EpollPoller{
	public:
		using EpollPollerCallBack = TcpConnection::TcpConnectionCallBack;
		using Functor = function<void()>;

		EpollPoller(Acceptor &);
		~EpollPoller();

		void loop();
		void unloop();
		void setAllCallBack(EpollPollerCallBack
				,EpollPollerCallBack
				,EpollPollerCallBack);

		void wakeup();
		void runInLoop(Functor &&);
	private:
		void waitEpollFd();
		void handleNewConnection();
		void handleMessage(int);

		void handleRead();
		void doPendingFunctors();
	private:
		int _epfd;
		int _listenFd;
		int _eventFd;
		Acceptor & _acceptor;
		bool _isLooping;
		MutexLock _mutex;
		vector<Functor> _pendingFunctors;
		map<int, TcpConnectionPtr> _connMap;
		vector<struct epoll_event> _events;
		
		EpollPollerCallBack _onConnectionCb;
		EpollPollerCallBack _onMessageCb;
		EpollPollerCallBack _onCloseCb;
};

int createEpollFd();
void addEpollFdRead(int, int);
void delEpollFd(int, int);

int createEventFd();

}

#endif
