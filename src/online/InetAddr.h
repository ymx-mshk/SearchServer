
#ifndef __MYSP_INETADDR_H__
#define __MYSP_INETADDR_H__

#include <netinet/in.h>
#include <string>

namespace mysp
{

class InetAddr{
	public:
		using port_t = unsigned short;
		using addr_t = struct sockaddr_in;

		explicit InetAddr(port_t);
		InetAddr(const std::string, port_t);
		InetAddr(addr_t);

		port_t port() const;
		std::string ip() const;
		const addr_t * getSockAddrPtr() const;

	private:
		void init(port_t, in_addr_t ip = INADDR_ANY);
	private:
		sockaddr_in _addr;
};

InetAddr getLocalAddr(int);
InetAddr getPeerAddr(int);

}

#endif
