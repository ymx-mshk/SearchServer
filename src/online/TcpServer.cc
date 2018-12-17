
#include "TcpServer.h"

namespace mysp
{

TcpServer::TcpServer(const string & ip, port_t port, int listenNum)
: _acceptor(ip, port, listenNum)
, _epoller(_acceptor){
}

TcpServer::TcpServer(port_t port, int listenNum)
: _acceptor(port, listenNum)
, _epoller(_acceptor){
}

void TcpServer::start(){
	_acceptor.ready();
	_epoller.loop();
}

void TcpServer::stop(){
	_epoller.unloop();
}

void TcpServer::setAllCallBack(CallBack cb1, CallBack cb2, CallBack cb3){
	_epoller.setAllCallBack(cb1, cb2, cb3);
}

}
