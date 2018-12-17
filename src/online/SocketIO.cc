
#include "SocketIO.h"
#include "Mylogger.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>

namespace mysp
{

#define SENDRECV_FAILURE -1

SocketIO::SocketIO(int fd)
	: _fd(fd){
	}

ssize_t SocketIO::sendAll(const char * buf, size_t len){
	//const char * pbuf = buf;
	size_t sendCount = 0;
	while (sendCount < len){
		ssize_t ret = ::send(_fd, buf, len - sendCount, 0);
		LogDebug("Send %d bytes", ret);
		if(-1 == ret){
			if(errno == EINTR)
				continue;
			LogError("Send error");
			return SENDRECV_FAILURE;
		}
		sendCount += ret;
		buf += ret;
	}

	return sendCount;
}

ssize_t SocketIO::sendUint32(const unsigned int value){
	ssize_t ret;
	do{
		ret = send(_fd, &value, sizeof(value), 0);
	}while(-1 == ret && errno == EINTR);
	if (-1 == ret){
		LogError("Send error");
		return SENDRECV_FAILURE;
	}

	return ret;
}

ssize_t SocketIO::recvAll(char * buf, size_t len){
	size_t recvCount = 0;
	while (recvCount < len){
		ssize_t ret = ::recv(_fd, buf, len - recvCount, 0);
		LogDebug("Receive %d bytes", ret);
		if (-1 == ret){
			if (errno == EINTR)
				continue;
			LogError("Recv error");
			return SENDRECV_FAILURE;
		}else if (0 == ret){
			break;
		}
		recvCount += ret;
		buf += ret;
	}

	return recvCount;
}

ssize_t SocketIO::recvLine(char * buf, size_t len){
	size_t recvCount = 0;
	//如果len == 0,下面这个条件会一直成立
	while (recvCount < len - 1){
		ssize_t ret = recvPeek(_fd, buf, len - 1);
		LogDebug("RecvPeek %d bytes", ret);
		if (-1 == ret){
			LogError("Recv error");
			return SENDRECV_FAILURE;
		}else if (0 == ret){
			return 0;
		}

		for (ssize_t idx = 0; idx != ret; ++idx){
			if (buf[idx] == '\n'){
				ssize_t size = idx + 1;
				if (size != recvAll(buf, size)){
					LogError("Recv error");
					return SENDRECV_FAILURE;
				}
				buf += size;
				recvCount += size;
				*buf = 0;
				return recvCount;
			}
		}
		if (ret != recvAll(buf, ret)){
			LogError("Recv error");
			return SENDRECV_FAILURE;
		}
		buf += ret;
		recvCount += ret;
	}
	*buf = 0;
	return recvCount;
}

}
