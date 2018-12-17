
#ifndef __MYSP_ACCEPTOR_H__
#define __MYSP_ACCEPTOR_H__

#include "Socket.h"
#include "InetAddr.h"
#include <string>

namespace mysp
{

class Acceptor{
	public:
		Acceptor(InetAddr::port_t, int);
		Acceptor(const std::string &, InetAddr::port_t, int);
		Acceptor(const InetAddr &, int);

		void ready();
		int accept();
		int fd() const;

	private:
		void bind();
		void listen();
		void setReuseAddr(bool);
		void setReusePort(bool);

		void errorProcess();
	private:
		int _listenNum;
		InetAddr _addr;
		Socket _listenFd;
};

}

#endif
