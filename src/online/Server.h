
#ifndef __MYSP_SERVER_H__
#define __MYSP_SERVER_H__

#include "TcpServer.h"
#include "PthreadPool.h"

namespace mysp
{

class Server{
	public:
		using CallBack = TcpServer::CallBack;
		using port_t = TcpServer::port_t;

		Server(const string &, port_t, int, int, int);
		//Server(port_t, int, int, int);

		virtual ~Server();

		virtual void start() = 0;
		void stop();

	protected:
		virtual void onConnection(const TcpConnectionPtr &) = 0;
		virtual void onMessage(const TcpConnectionPtr &) = 0;
		virtual void onClose(const TcpConnectionPtr &) = 0;
	protected:
		TcpServer _tcpServer;
		PthreadPool _pthreadPool;
};

}

#endif
