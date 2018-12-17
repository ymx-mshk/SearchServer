
#include "TcpConnection.h"
#include "EpollPoller.h"
#include <string.h>
using namespace std;

namespace mysp
{

TcpConnection::TcpConnection(int fd, EpollPoller * epoller)
: _isShutdownWrite(false)
, _peerSock(fd)
, _socketIO(fd)
, _epoller(epoller)
, _localAddr(getLocalAddr(fd))
, _peerAddr(getPeerAddr(fd)){
	_peerSock.nonblock();
}

TcpConnection::~TcpConnection(){
	shutDown();
}

string TcpConnection::receive(){
	char buf[65536];
	::memset(buf, 0, sizeof(buf));
	_socketIO.recvLine(buf, sizeof(buf));
	//buf包含换行，去掉
	return string(buf, strlen(buf) - 1);
}

void TcpConnection::send(const string & buf){
	_socketIO.sendAll(buf.c_str(), buf.size());
}

void TcpConnection::shutDown(){
	if (!_isShutdownWrite){
		_isShutdownWrite = true;
		_peerSock.shutdownWrite();	
	}
}

void TcpConnection::handleConnectionCallBack(){
	if (_onConnectionCb)
		_onConnectionCb(shared_from_this());
}

void TcpConnection::handleMessageCallBack(){
	if (_onMessageCb)
		_onMessageCb(shared_from_this());
}

void TcpConnection::handleCloseCallBack(){
	if (_onCloseCb)
		_onCloseCb(shared_from_this());
}

void TcpConnection::setAllCallBack(TcpConnectionCallBack cb1
		,TcpConnectionCallBack cb2
		,TcpConnectionCallBack cb3){

	_onConnectionCb = cb1;
	_onMessageCb = cb2;
	_onCloseCb = cb3;
}

string TcpConnection::toString()
{
	char str[100];
	snprintf(str, sizeof(str), "%s:%d -> %s:%d"
			,_localAddr.ip().c_str()
			,_localAddr.port()
			,_peerAddr.ip().c_str()
			,_peerAddr.port());
	return std::string(str);
}

void TcpConnection::sendInLoop(const string & msg){
	_epoller->runInLoop(std::bind(&TcpConnection::send, this, msg));
}

bool isConnectionClosed(int sockfd){
	char buf[16];
	int readNum = recvPeek(sockfd, buf, sizeof(buf));
	if(-1 == readNum)
	{
		perror("recvPeek:");
		return true;
	}
	return 0 == readNum;
}

}
