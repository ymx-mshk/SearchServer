
#ifndef __MYSP_SOCKET_H__
#define __MYSP_SOCKET_H__

#include "Uncopyable.h"

namespace mysp
{

class Socket
: Uncopyable{
	public:
		Socket();
		explicit Socket(int);
		~Socket();

		int fd() const;
		void shutdownWrite();
		void nonblock();
		
	private:
		int _sockFd;
};

int createSockFd();

}

#endif
