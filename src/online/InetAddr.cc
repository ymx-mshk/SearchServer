
#include "InetAddr.h"
#include "Mylogger.h"

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace mysp
{

InetAddr::InetAddr(const std::string ip, port_t port){
	init(port, inet_addr(ip.c_str()));
}

InetAddr::InetAddr(port_t port){
	init(port);
}

InetAddr::InetAddr(addr_t addr)
: _addr(addr){
}

//第二个参数默认实参为INADDR_ANY
void InetAddr::init(port_t port, in_addr_t ip){
	::memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = ip;
}

InetAddr::port_t InetAddr::port() const {
	return ntohs(_addr.sin_port);
}

std::string InetAddr::ip() const {
	return inet_ntoa(_addr.sin_addr);
}

const InetAddr::addr_t * InetAddr::getSockAddrPtr() const {
	return &_addr;
}

InetAddr getLocalAddr(int fd){
	struct sockaddr_in local;
	socklen_t addrlen = sizeof(local);
	if (-1 == ::getsockname(fd
				,reinterpret_cast<struct sockaddr *>(&local)
				,&addrlen))
	{
		LogWarn("getLocalAddr error");
		perror("getLocalAddr error");
	}
	return InetAddr(local); 
}

InetAddr getPeerAddr(int fd){
	struct sockaddr_in peer;
	socklen_t addrlen = sizeof(peer);
	if (-1 == ::getpeername(fd
				,reinterpret_cast<struct sockaddr *>(&peer)
				,&addrlen))
	{
		LogWarn("getPeerAddr error");
		perror("getPeerAddr error");
	}
	return InetAddr(peer);
}

}
