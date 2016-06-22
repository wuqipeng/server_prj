#ifndef TRANSPORT_H_
#define TRANSPORT_H_

//传输抽象类，提供基础的传输接口


namespace server { namespace transport {

	class Transport 
	{
		public:

			virtual ~Transport() = 0;
			virtual void open() = 0;
			virtual int write(const char* buf, const int size) = 0;
			virtual int read(const char* buf, const int size) = 0; 
			virtual void close() = 0;
            virtual void flush(){ }
	};

//工厂类


}}








#endif
