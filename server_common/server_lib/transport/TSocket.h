#ifndef TSOCKET_H_
#define TSOCKET_H_

#include <string>
#include "transport.h"

using namespace std;

namespace server { namespace transport {

	class TSocket: public Transport
	{
		public:
			TSocket();

			TSocket(const string host, const int port);

			TSocket(const int socket_fd);    //这个用于给TServerSocket提供方法来创建已连接的TSocket对象，从而进行数据传输通信。
			
			
			void SetHost(const string host);
			void SetPort(const int port);

			void SetKeepalive(bool keepalive);
			void SetConnectTimeout(int s);
			void SetRecvTimeout(int s);
			void SetSendTimoout(int s);
			
			virtual void open();
			void openconnection(struct addrinfo *res);
			void setSocketBlock(bool flag);

			virtual int write(const char* buf, const int size);
			virtual int read(const char* buf, const int size);
			virtual void close();
	
			bool IsOpen();	
		private:
			int socket_;             //套接字描述符
			string host_;
			int port_;
			bool keepalive_;          //发送检测TCP报文给对端,检测对端是否主机崩溃或者重启等非正常关闭
			int connectTimeout;    //客户端连接超时时间,通过poll机制来实现超时 
			int recvTimeout;      
			int sendTimeout;
			bool isOpen_;

	};

}}


#endif
