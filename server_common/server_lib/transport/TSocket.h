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

			TSocket(const int socket_fd);    //������ڸ�TServerSocket�ṩ���������������ӵ�TSocket���󣬴Ӷ��������ݴ���ͨ�š�
			
			
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
			int socket_;             //�׽���������
			string host_;
			int port_;
			bool keepalive_;          //���ͼ��TCP���ĸ��Զ�,���Զ��Ƿ������������������ȷ������ر�
			int connectTimeout;    //�ͻ������ӳ�ʱʱ��,ͨ��poll������ʵ�ֳ�ʱ 
			int recvTimeout;      
			int sendTimeout;
			bool isOpen_;

	};

}}


#endif
