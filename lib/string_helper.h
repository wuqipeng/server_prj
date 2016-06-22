#ifndef STRING_HELPER_H_
#define STRING_HELPER_H_

#include <string.h>

using namespace std;

inline string GetErrorString(const string info, const int error)
{
	char *c = strerror(error);
	return info + string(c) ;
}

inline string uToStr(uint32_t u)
{
    stringstream tmp;
    tmp << u;
    return tmp.str();
}

#endif
