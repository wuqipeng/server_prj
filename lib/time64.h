/*
 * =====================================================================================
 *
 *       Filename:  time64.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/04/2012 06:31:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __TIME64_H__
#define __TIME64_H__

#include <time.h>

typedef long long time64_t;

time64_t mktime64(tm* time, long timezone = 8);

void localtime64( tm *time, time64_t re,long timezone = 8);

bool gmt_convert_local(const char* gmt, char* local);

#endif 
