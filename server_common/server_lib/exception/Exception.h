#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <exception>
#include <string>	
using namespace std;

namespace server { namespace exception {
	

class Exception:public std::exception
{
	public:
		Exception(){}
		virtual ~Exception() throw() {}

		Exception(const string msg):msg_(msg){}

		virtual const char* what() const throw()    //后面的throw()告诉编译器这个函数一定不抛出异常
		{
			if(msg_.empty())
			{
				return "default exception.";
			}else
			{
				return msg_.c_str();
			}
		}

	protected:
		string msg_;		
};


}}


















#endif
