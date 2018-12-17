
#include "EpollPoller.h"
#include "Acceptor.h"
#include "Mylogger.h"

#include <unistd.h>
#include <sys/eventfd.h>
#include <iostream>

namespace mysp
{

EpollPoller::EpollPoller(Acceptor & acceptor)
: _epfd(createEpollFd())
, _listenFd(acceptor.fd())
, _eventFd(createEventFd()) 
, _acceptor(acceptor)
, _isLooping(false)
, _events(64){
}

EpollPoller::~EpollPoller(){
	close(_epfd);
}

void EpollPoller::loop(){
	addEpollFdRead(_epfd, _listenFd);
	addEpollFdRead(_epfd, _eventFd);
	_isLooping = true;
	LogInfo("Loop start");
	while (_isLooping){
		waitEpollFd();
	}
}

void EpollPoller::unloop(){
	_isLooping = false;
	LogInfo("Loop stop");
}

void EpollPoller::waitEpollFd(){
	int readyNum;
	do {
		readyNum = ::epoll_wait(_epfd
				,&(*_events.begin())
				,_events.size()
				,5000);
	}while (-1 == readyNum && errno == EINTR);

	if (-1 == readyNum){
		perror("epoll_wait:");
		LogError("Epoll_wait error");
		exit(EXIT_FAILURE);
	}else if (0 == readyNum){
		std::cout << "epoll_wait timeout" << std::endl;
	}
	else {
		if (static_cast<size_t>(readyNum) == _events.size()){
			_events.resize(_events.size() * 2);
		}

		for (int idx = 0; idx != readyNum; ++idx){
			if (_events[idx].data.fd == _listenFd){
				if (_events[idx].events & EPOLLIN)
					handleNewConnection();	
			}else if (_events[idx].data.fd == _eventFd){
				if (_events[idx].events & EPOLLIN){
					handleRead();
					doPendingFunctors();
				}
			}else {
				if (_events[idx].events & EPOLLIN)
					handleMessage(_events[idx].data.fd);
			}
		}
	}
}

void EpollPoller::handleNewConnection(){
	int peerfd = _acceptor.accept();
	if (-1 == peerfd){
		return;
	}

	TcpConnectionPtr conn(new TcpConnection(peerfd, this));
	conn->setAllCallBack(_onConnectionCb
			,_onMessageCb
			,_onCloseCb);

	addEpollFdRead(_epfd, peerfd);
	_connMap.emplace(peerfd, conn);

	conn->handleConnectionCallBack();
}

void EpollPoller::handleMessage(int peerfd){
	auto it = _connMap.find(peerfd);
	if (it == _connMap.end())
		return;
	bool isClosed = isConnectionClosed(peerfd);
	if (isClosed){
		it->second->handleCloseCallBack();
		delEpollFd(_epfd, peerfd);
		_connMap.erase(peerfd);
	}else {
		it->second->handleMessageCallBack();
	}
}

void EpollPoller::runInLoop(Functor && cb){
	{
		AutoLock lock(_mutex);
		_pendingFunctors.push_back(std::move(cb));
	}
	wakeup();
}

void EpollPoller::doPendingFunctors(){
	vector<Functor> tmp;
	{
		AutoLock lock(_mutex);
		std::swap(tmp, _pendingFunctors);
	}
	for (auto & ele : tmp){
		ele();
	}
}

void EpollPoller::wakeup(){
	uint64_t u;
	if (sizeof(u) != write(_eventFd, &u, sizeof(u))){
		LogWarn("Write event error");
		perror("write event");
	}
}

void EpollPoller::handleRead(){
	uint64_t u;
	if (sizeof(u) != read(_eventFd, &u, sizeof(u))){
		LogWarn("Read event error");
		perror("read event:");
	}
}

void EpollPoller::setAllCallBack(EpollPollerCallBack cb1
		,EpollPollerCallBack cb2
		,EpollPollerCallBack cb3){
	
	_onConnectionCb = cb1;
	_onMessageCb = cb2;
	_onCloseCb = cb3;
}

int createEpollFd(){
	int epfd = epoll_create1(0);
	if (-1 == epfd){
		perror("create epollfd:");
		LogError("Create epollfd error");
		exit(EXIT_FAILURE);
	}
	return epfd;
}

void addEpollFdRead(int epfd, int readFd){
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = readFd;

	if (-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, readFd, & event)){
		perror("add epollFd read:");
		LogError("Add epollfd read error");
		exit(EXIT_FAILURE);
	}
}

void delEpollFd(int epfd, int delFd){
	if (-1 == epoll_ctl(epfd, EPOLL_CTL_DEL, delFd, NULL)){
		perror("delete epollFd:");
		LogError("Delete epollfd error");
		exit(EXIT_FAILURE);
	}
}

int createEventFd(){
	int eventFd = eventfd(0, 0);
	if (-1 == eventFd){
		perror("create eventfd:");
		LogError("create eventfd error");
		exit(EXIT_FAILURE);
	}

	return eventFd;
}

}
