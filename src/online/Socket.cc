
#include "SocketUtil.h"
#include "Socket.h"
#include "Mylogger.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

namespace mysp
{

Socket::Socket()
: _sockFd(createSockFd()){

}

Socket::Socket(int sockFd)
: _sockFd(sockFd){
}

Socket::~Socket(){
	close(_sockFd);
}

int Socket::fd() const{
	return _sockFd;
}

void Socket::nonblock(){
	setNonblock(_sockFd);
}

void Socket::shutdownWrite(){
	if(-1 == ::shutdown(_sockFd, SHUT_WR)){
		perror("shutdown write:");
		LogWarn("shutdown write error");
	}
}

int createSockFd(){
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd){
		perror("socket create:");
		LogError("socket create error");
		exit(EXIT_FAILURE);
	}

	return sockfd;
}

}
