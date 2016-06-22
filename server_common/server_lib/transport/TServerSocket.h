#ifndef TSERVERSOCkET_H_
#define TSERVERSOCkET_H_

#include "TSocket.h"

namespace server { namespace transport {
	
	class TServerSocket
	{
		public:
			TServerSocket(const int port);
			
			void SetPort(const int port); 
			void SetSendBuffer(cosnt int sendBuffer);
			void SetRecvBuffer(cosnt int recvBuffer);
			void SetAcceptBacklog(const int acceptBacklog);

			void listen();
			boost::shared_ptr<transport> accept();

		private:
			int serverSocket_ ;  //·þÎñ¼àÌýÌ×½Ó×Ö
			int port_;
			int sendBuffer_;
			int recvBuffer_;
			int	acceptBacklog_;

	};

}}

#endif
