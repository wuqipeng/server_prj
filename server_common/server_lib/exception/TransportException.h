#ifndef TRANSPORTEXCEPTION_H_
#define TRANSPORTEXCEPTION_H_

#include "Exception.h"
#include <string>

using namespace std;
namespace server { namespace exception {

class TransportException:public Exception
{
	public:
		enum TransportExceptionType
		{	
			UN_KNOWN = 0,
			UN_OPEN,
			TIEM_OUT
		};

		TransportException():
			Exception(),
			type_(){

			}
			
		TransportException(const TransportExceptionType type):
			Exception(),
			type_(type){

			}
	
		TransportException(const string msg):
			Exception(msg),
			type_(UN_KNOWN){

			}

		TransportException(const TransportExceptionType type,const string msg):
			Exception(msg),
			type_(type){

			}

		virtual const char* what() const throw();

		TransportExceptionType GetExceptionType() throw();
		
	private:
		TransportExceptionType type_ ;
};


}}



#endif
