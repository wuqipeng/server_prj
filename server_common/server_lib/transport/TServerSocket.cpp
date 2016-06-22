#include <string>

#include "TServerSocket.h"
#include "TSocket.h"
#include "log.h"
#include "exception/TransportException.h"
#include "string_helper.h"

using namespace std;
using namespace server::log;
using namespace server::transport;
using namespace server::exception;

TServerSocket::TServerSocket(const int port):
	serverSocket_(-1),
	port_(port),
	sendBuffer_(0),
	recvBuffer_(0),
	acceptBacklog_(0){

}

void TServerSocket::SetPort(const int port)
{
	if(port < 0 || port > 65535)
	{
		logger(LOG_ERROR, "TServerSocket::SetPort():prams invalid.");
		throw TransportException(TransportException::UN_KNOWN,"TServerSocket::SetPort():prams invalid.");
		return ;
	}

	port_ = port;
}

void TServerSocket::SetSendBuffer(cosnt int sendBuffer)
{
	sendBuffer_ = sendBuffer;
	if(port_ == -1)
	{
		return;
	}

	if(-1 == setsockopt(serverSocket_, SOL_SOCKET, SO_RCVBUF, SO_SNDBUF, &sendBuffer_, sizeof(sendBuffer_)))
	{
		logger(LOG_ERROR, "TServerSocket::SetSendBuffer():set send buff size falied.");
	}

}

void TServerSocket::SetRecvBuffer(cosnt int recvBuffer)
{

}

void TServerSocket::SetAcceptBacklog(const int acceptBacklog)
{

}

void TServerSocket::listen()
{

}

boost::shared_ptr<transport> TServerSocket::accept()
{

}


