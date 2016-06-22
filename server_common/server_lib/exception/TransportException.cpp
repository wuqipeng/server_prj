#include "TransportException.h"

using namespace std;
using namespace server::exception;

const char* TransportException::what() const throw()
{
	if(msg_.empty())
	{
		switch(type_)
		{
			case UNKNOWN:
				return "UNKNOWN Exception:   ";
			case UN_CONNECT:
				return "UN_CONNECT Exception: ";
		}

	}else
	{
		return msg_.c_str();
	}

}

TransportException::TransportExceptionType TransportException::GetExceptionType() throw()
{
	return type_;
}


