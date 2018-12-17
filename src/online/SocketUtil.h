
#ifndef __MYSP_SOCKETUTIL_H__
#define __MYSP_SOCKETUTIL_H__

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace mysp
{

inline
void setNonblock(int fd){
	fcntl(fd, F_SETFL, 
			fcntl(fd, F_GETFL) | O_NONBLOCK
		 );
}

ssize_t recvPeek(int sockfd, char * buf, size_t len){
	int recvCount;
	do{
		recvCount = recv(sockfd, buf, len, MSG_PEEK);
	}while(recvCount == -1 && errno == EINTR);

	return recvCount;
}

}
#endif
