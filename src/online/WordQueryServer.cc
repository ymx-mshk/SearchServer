
#include "Configuration.h"
#include "WordQueryServer.h"

using namespace std::placeholders;
#include <iostream>

namespace mysp
{

WordQueryServer::WordQueryServer()
:Server(getConfig("ip"),
		atoi(getConfig("port").c_str()),
		atoi(getConfig("listenFdNum").c_str()),
		atoi(getConfig("pthreadNum").c_str()),
		atoi(getConfig("taskQueueSize").c_str())){
}

void WordQueryServer::start(){
	_query.loadLibrary();
	_pthreadPool.start();
	_tcpServer.setAllCallBack(
			bind(&WordQueryServer::onConnection, this, _1),
			bind(&WordQueryServer::onMessage, this, _1),
			bind(&WordQueryServer::onClose, this, _1));
	_tcpServer.start();
}

void WordQueryServer::onConnection(const TcpConnectionPtr & conn){
		cout << conn->toString() << std::endl;
		//conn->send("hello, welcome to Spellcorrect server.\r\n");
}

void WordQueryServer::onMessage(const TcpConnectionPtr & conn){
		std::string s(conn->receive());

		cout << "recved querywords is " << s << endl;
		//在IO线程中，将任务的执行交给线程池处理
		_pthreadPool.addTask(std::bind(&WordQueryServer::doTaskThread,
					this, conn, s));
		cout << "> add task to threadpool" << endl;
}

void WordQueryServer::doTaskThread(const TcpConnectionPtr & conn, const string & msg){
	string s = _query.doQuery(msg);
	string s1 =to_string(s.size()).append("\n").append(s);

	conn->sendInLoop(s1);
}

void WordQueryServer::onClose(const TcpConnectionPtr & conn){
	cout << conn->toString() << " close" << endl;
}

}
