
#include "Acceptor.h"
#include "Mylogger.h"
#include <unistd.h>

namespace mysp
{

Acceptor::Acceptor(InetAddr::port_t port, int listenNum)
: _listenNum(listenNum)
, _addr(port)
, _listenFd(){
}

Acceptor::Acceptor(const std::string & ip, InetAddr::port_t port, int listenNum)
: _listenNum(listenNum)
, _addr(ip, port)
, _listenFd(){
}

Acceptor::Acceptor(const InetAddr & addr, int listenNum)
: _listenNum(listenNum)
, _addr(addr)
, _listenFd(){
}

int Acceptor::fd() const{
	return _listenFd.fd();
}

int Acceptor::accept(){
	int peerfd = ::accept(_listenFd.fd(), NULL, NULL);
	if (-1 == peerfd){
		perror("accept:");
		LogError("Accept error");
	}

	LogInfo("Accept success, peer addr:%s, peer port: %u",
			getPeerAddr(peerfd).ip().c_str(), getPeerAddr(peerfd).port());
	return peerfd;
}

void Acceptor::ready(){
	setReuseAddr(true);
	setReusePort(true);
	bind();
	listen();
}

void Acceptor::bind(){
	if (-1 == ::bind(_listenFd.fd()
			,reinterpret_cast<const struct sockaddr *>(_addr.getSockAddrPtr())
			,sizeof(InetAddr)))
	{
		perror("bind:");
		LogError("Bind error");
		errorProcess();
	}
}

void Acceptor::listen(){
	if (-1 == ::listen(_listenFd.fd(), _listenNum))
	{
		perror("listen:");
		LogError("Listen error");
		errorProcess();
	}
}

void Acceptor::setReuseAddr(bool flag){
	int value = flag ? 1 : 0;
	if (-1 == setsockopt(_listenFd.fd()
				,SOL_SOCKET
				,SO_REUSEADDR
				,static_cast<const void *>(&value)
				,static_cast<socklen_t>(sizeof value)))
	{
		perror("reuse addr:");
		LogError("Reuse addr error");
		errorProcess();
	}
}

void Acceptor::setReusePort(bool flag){
	int value = flag ? 1 : 0;
	if (-1 == setsockopt(_listenFd.fd()
			,SOL_SOCKET
			,SO_REUSEPORT
			,static_cast<const void *>(&value)
			,static_cast<socklen_t>(sizeof value)))
	{
		perror("reuse port:");
		LogError("Reuse port error");
		errorProcess();
	}
}

void Acceptor::errorProcess(){
		::close(_listenFd.fd());
		::exit(EXIT_FAILURE);
}

}
