#include "Exception.h"
#include <iostream>
#include "TransportException.h"
#include <stdlib.h>

using namespace std;
using namespace server::exception;

int main(int argv, char **args)
{
	try
	{
		if(argv != 2)
		{
			throw Exception("arg error");
		}
		int type = atoi(args[1]);
		cout<< type<<endl;
		if(type == 0)
		{
			throw TransportException(TransportException::UNKNOWN);
		}else if(type == 1)
		{
			throw TransportException(TransportException::UN_CONNECT,"un connect error.");
		}
	}
	catch(exception &e)
	{
		cout<<e.what()<<endl;
	}
}
