
#include "Server.h"

namespace mysp
{

Server::Server(const string & ip, port_t port, int listenFdNum,
		int threadNum, int taskNum)
: _tcpServer(ip, port, listenFdNum)
, _pthreadPool(threadNum, taskNum){
}

/*
Server::Server(port_t port, int listenFdNum,
		int threadNum, int taskNum)
: _tcpServer(port, listenFdNum)
, _pthreadPool(threadNum, taskNum){
}*/

Server::~Server(){
	stop();
}

void Server::stop(){
	_pthreadPool.stop();
	_tcpServer.stop();
}

}
