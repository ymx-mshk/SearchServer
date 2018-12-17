
#ifndef __MYSP_SOCKETIO_H__
#define __MYSP_SOCKETIO_H__

#include "Uncopyable.h"
#include <sys/types.h>

namespace mysp
{

ssize_t recvPeek(int, char *, size_t);

class SocketIO{
	public:
		explicit SocketIO(int);

		ssize_t recvAll(char *, size_t);
		ssize_t sendAll(const char *, size_t);
		ssize_t sendUint32(unsigned int);
		ssize_t recvLine(char *, size_t);

	private:
		int _fd;
};

}

#endif
