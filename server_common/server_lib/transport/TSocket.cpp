
#include <netdb.h>
#include <string>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>

#include "TSocket.h"
#include "log.h"
#include "exception/TransportException.h"
#include "string_helper.h"


using namespace std;
using namespace server::log;
using namespace server::transport;
using namespace server::exception;

TSocket::TSocket():
	socket_(-1),
	host_(""),
	port_(0),
	keepalive_(false),
	connectTimeout(0),
	recvTimeout(0),
	sendTimeout(0),
	isOpen_(false){

}

TSocket::TSocket(const string host, const int port):
	socket_(-1),
	host_(host),
	port_(port),
	keepalive_(false),
	connectTimeout(0),
	recvTimeout(0),
	sendTimeout(0),
	isOpen_(false){

}

TSocket::TSocket(const int socket_fd):
	socket_(socket_fd),
	host_(""),
	port_(0),
	keepalive_(false),
	connectTimeout(0),
	recvTimeout(0),
	sendTimeout(0),
	isOpen_(true){

}


void TSocket::SetHost(const string host)
{
	host_ = host;
}


void TSocket::SetPort(const int port)
{
	port_ = port;
}

void TSocket::SetKeepalive(bool keepalive)
{
	keepalive_ = keepalive;
	if(socket_ == -1)
	{
		return;
	}

	int value = keepalive_;
	int ret = setsockopt(socket_,SOL_SOCKET,SO_KEEPALIVE,(int*)&value,sizeof(value));
	if( ret == -1)    //设置套接字选项错误，要不要抛异常
	{
		logger(LOG_ERROR, "set keepalive socket option failed!");

	}else
	{
		logger(LOG_INFO,"set keepalive socket option successful!");
	}
	
}

void TSocket::SetConnectTimeout(int s)
{
	connectTimeout = s;
}

void TSocket::SetRecvTimeout(int s)
{
	recvTimeout = s;
	if(socket_ == -1)
	{
		return;
	}

	timeval timeout;
	timeout.tv_sec = s;
	
	int ret = setsockopt(socket_,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));
	if( ret == -1)
	{
		logger(LOG_ERROR,"set recv timeout socket option failed!");
	}else
	{
		logger(LOG_INFO,"set recv timeout socket option successful!");
	}
}

void TSocket::SetSendTimoout(int s)
{
	sendTimeout = s;
	if(socket_ == -1)
	{
		return;
	}

	timeval timeout;
	timeout.tv_sec = s;
	if( -1 == setsockopt(socket_,SOL_SOCKET,SO_SNDTIMEO,(char*)&timeout,sizeof(timeval)))
	{
		logger(LOG_ERROR,"set send timeout socket option failed!");
	}else
	{
		logger(LOG_INFO,"set send timeout socket option successful!");
	}

}


bool TSocket::IsOpen()
{
	return isOpen_;
}

void TSocket::open()
{
	if(IsOpen())
	{
		return;
	}

	//检查主机IP和端口参数是否有效
	if(host_.empty()||port_ <= 0 || port_ > 65535)
	{
		logger(LOG_ERROR,"TSocket::open() : invalid host or port parameter.");
		throw TransportException(TransportException::UN_OPEN,"TSocket::open() : invalid host or port parameter.");
	}
	
	//根据主机名和服务名获取主机ip和服务端口号

	struct addrinfo hints, *result;
	memset(&hints,0,sizeof(addrinfo));
	hints.ai_flags    = 0;
	hints.ai_family   = AF_INET;   //地址族
	hints.ai_socktype = SOCK_STREAM; //套接字类型
	hints.ai_protocol = 0;          //协议类型
	result = NULL;

	char port[sizeof("65535")];
	sprintf(port,"%d",port_);    //整数port_转换成字符串，这个sprintf函数是ANSI标准兼容的

	int32_t ret = getaddrinfo(host_.c_str(),port ,&hints,&result);
	if(ret == -1)
	{
		
		logger(LOG_ERROR, GetErrorString("TSocket::open() getaddrinfo():",errno));
		throw TransportException(TransportException::UN_OPEN,"TSocket::open() : cannot get addr info!");
	}


	//创建连接
	for(struct addrinfo *res = result ; res ; res = res->ai_next)
	{
		try
		{
			openconnection(res);
			break;
		}catch(TransportException &e)
		{
			if(res->ai_next == NULL)
			{
				close();
				freeaddrinfo(result);
				throw;         //是不是抛的就是接受到的异常?
			}else
			{
				close();
			}
		}
	}

	freeaddrinfo(result);

}

void TSocket::openconnection(struct addrinfo *res)
{
	//创建套接字
	socket_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(socket_ == -1)
	{

		logger(LOG_ERROR, GetErrorString("TSocket::open() socket():",errno));
		throw TransportException(TransportException::UN_OPEN,"TSocket::open() : create socket failed!");
	}
	logger(LOG_INFO, "TSocket::open() socket():create socket successful");
	
	//设置套接字选项
	if(keepalive_)
	{
		SetKeepalive(keepalive_);
	}


	if(sendTimeout > 0)
	{
		SetSendTimoout(sendTimeout);		
	}

	if(recvTimeout > 0)
	{
		SetSendTimoout(recvTimeout);
	}

	//如果设置了connectTimeout,那么连接的时候，需要把设置套接字为非阻塞方法
	if(connectTimeout > 0)
	{
		setSocketBlock(false);
	}
	
	int32_t ret = connect(socket_,res->ai_addr, res->ai_addrlen);
	if(ret == -1 && errno != EINPROGRESS)     //connect出错
	{
		logger(LOG_ERROR, GetErrorString("TSocket::open() fcntl():",errno));
		throw TransportException(TransportException::UN_OPEN,"TSocket::open() : connect() failed!");
	}else if(ret == 0)   //connect成功
	{
		goto done;
	}

	//connect正在进行,把这个非阻塞套接字加入poll函数的监听事件集合
	pollfd fds[1];
	memset(fds,0,sizeof(pollfd));
	fds[0].fd     = socket_;
	fds[0].events = POLLOUT;

	ret = poll(fds, 1, connectTimeout);
	if(ret == 0 )
	{
		logger(LOG_ERROR, "TSocket::open() : poll timeout.");
		throw TransportException(TransportException::UN_OPEN,"TSocket::open() : poll timeout.");
	}else if(ret == -1)
	{
		logger(LOG_ERROR, GetErrorString("TSocket::open() : poll():",errno));
		throw TransportException(TransportException::UN_OPEN,"TSocket::open() : poll falied.");
	}
	//由于poll只是监控一个套接字事件，如果ret>0表示一定是这个套接字事件,所以不需要比较是哪个套接字就绪
	
	int val;
	socklen_t len;
	len = sizeof(int);
	ret = getsockopt(socket_,SOL_SOCKET, SO_ERROR, static_cast<int *>(&val), &len);
	if(ret == -1)
	{
		logger(LOG_ERROR, GetErrorString("TSocket::open() : getsockopt():",errno));
		throw TransportException(TransportException::UN_OPEN,"TSocket::open() : getsockopt falied.");
	}

	if(val == 0)   //connect进过poll之后返回值,0表示连接建立
	{
		goto done;
	}else
	{
		logger(LOG_ERROR, GetErrorString("TSocket::open() : connect():",errno));
		throw TransportException(TransportException::UN_OPEN,"TSocket::open() : connect falied.");
	}

done:
	setSocketBlock(true);
	isOpen_ = true;
	logger(LOG_INFO, "TSocket::open() open connection successful.");

}

void TSocket::setSocketBlock(bool flag)
{
	int32_t old_option = fcntl(socket_,F_GETFL,0);
	if(old_option == -1)
	{
		logger(LOG_ERROR, GetErrorString("TSocket::open() fcntl():",errno));
		throw TransportException(TransportException::UN_OPEN,"TSocket::open() : fcntl() failed!");
	}

	int32_t new_option = 0;
	if(flag)
	{	
		new_option = old_option &(~O_NONBLOCK);
	}else
	{
		new_option = old_option | O_NONBLOCK;
	}

	int32_t ret = fcntl(socket_, F_SETFL,new_option);
	if(ret == -1)
	{
		logger(LOG_ERROR, GetErrorString("TSocket::open() fcntl():",errno));
		throw TransportException(TransportException::UN_OPEN,"TSocket::open() : fcntl() failed!");
	}

}

int TSocket::write(const char* buf, const int size)
{
	if(!IsOpen())
	{
		logger(LOG_ERROR, "TSocket::write : connect is not open.");
		throw TransportException(TransportException::UN_OPEN, "TSocket::write failed.");
	}

	int32_t LeaveSize = size;
	char* wbuf = const_cast<char*>(buf);
	int32_t flags = 0;

try_again:

	while(LeaveSize > 0)
	{
		int32_t ret = send(socket_,wbuf,LeaveSize, flags);
		if(ret == 0)   //写的时候对端关闭了，一样是异常
		{
			logger(LOG_ERROR, "TSocket::write(): send return 0.");
			throw TransportException(TransportException::UN_KNOWN,"TSocket::write(): send return 0.");
		}else if(ret == -1 && errno == EINTR)
		{
			logger(LOG_DEBUG, GetErrorString("TSocket:::write() send():",errno));
			goto try_again;

		}else if(ret == -1 && errno == EAGAIN)  //阻塞和非阻塞套接字EAGAIN出错情况不同，这里只处理阻塞的情况，表示超时
		{
			logger(LOG_ERROR, GetErrorString("TSocket:::write() send():",errno));
			throw TransportException(TransportException::TIEM_OUT,"TSocket::write(): send timeout.");

		}else if(ret == -1 && errno == EWOULDBLOCK )
		{
			logger(LOG_DEBUG, GetErrorString("TSocket:::write() send():",errno));
			throw TransportException(TransportException::UN_KNOWN,"TSocket::write(): send would block.");

		}else if(ret == -1 )
		{
			logger(LOG_ERROR, GetErrorString("TSocket:::write() send():",errno));
			throw TransportException(TransportException::UN_KNOWN,"TSocket::write(): send falied.");

		}

		LeaveSize = LeaveSize - ret;
		wbuf = wbuf + ret;
	}
	return size;
}

int TSocket::read(const char* buf, const int size)
{
	if(!IsOpen())
	{
		logger(LOG_ERROR, "TSocket::read : connect is not open.");
		throw TransportException(TransportException::UN_OPEN, "TSocket::read failed.");
	}

	int32_t LeaveSize = size;
	char* rbuf = const_cast<char*>(buf);
	int32_t flags = 0;

try_again:

	while(LeaveSize > 0)
	{
		int32_t ret = recv(socket_,rbuf,LeaveSize, flags);
		if(ret == 0)   //读的时候，返回0表示对端正常关闭.
		{
			logger(LOG_INFO, "TSocket::read(): recv return 0.");
			return (size - LeaveSize);

		}else if(ret == -1 && errno == EINTR)
		{
			logger(LOG_DEBUG, GetErrorString("TSocket:::read() recv():",errno));
			goto try_again;

		}else if(ret == -1 && errno == EAGAIN)  //阻塞和非阻塞套接字EAGAIN出错情况不同，这里只处理阻塞的情况，表示超时
		{
			logger(LOG_ERROR, GetErrorString("TSocket:::read() recv():",errno));
			throw TransportException(TransportException::TIEM_OUT,"TSocket::read(): send timeout.");

		}else if(ret == -1 && errno == EWOULDBLOCK )
		{
			logger(LOG_DEBUG, GetErrorString("TSocket:::read() recv():",errno));
			throw TransportException(TransportException::UN_KNOWN,"TSocket::read(): recv would block.");

		}else if(ret == -1 )
		{
			logger(LOG_ERROR, GetErrorString("TSocket::read() recv():",errno));
			throw TransportException(TransportException::UN_KNOWN,"TSocket::read(): recv falied.");

		}

		LeaveSize = LeaveSize - ret;
		rbuf = rbuf + ret;
	}

	return size;
}

void TSocket::close()
{
	isOpen_ = false;
	if(socket_ == -1)
	{
		return;
	}
	
	shutdown(socket_,SHUT_RDWR);
	socket_ = -1;
}

