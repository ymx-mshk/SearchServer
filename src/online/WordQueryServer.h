
#ifndef __MYSP_WORDQUERYSERVER_H__
#define __MYSP_WORDQUERYSERVER_H__

#include "Server.h"
#include "WordQuery.h"

namespace mysp
{

class WordQueryServer
: public Server{

	public:
		WordQueryServer();
		void start();

	private:
		void doTaskThread(const TcpConnectionPtr &, const string &);
		void onConnection(const TcpConnectionPtr &);
		void onMessage(const TcpConnectionPtr &);
		void onClose(const TcpConnectionPtr &);
	private:
		WordQuery _query;

};

}

#endif
