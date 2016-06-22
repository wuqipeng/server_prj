/*
 * =====================================================================================
 *
 *       Filename:  time64.cpp
 *
 *    Description:  G
 *
 *        Version:  1.0
 *        Created:  09/05/2012 10:29:27 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <string.h>
#include <stdio.h>
#include "time64.h"
#include <stdlib.h>
#include <string>

const time64_t kSecPerMin = 60;
const time64_t kSecPerHour = 60*60;
const time64_t kSecPerDay= 60*60*24;
const time64_t kMonthesPerYear = 12;
const time64_t kDaysPerYear = 365;
const time64_t kDaysPerLeapYear = 366;
const time64_t kTimeBegin1970 = 719499; //不要纠结这里

int isleap64(unsigned short year) {
	return (((year%4==0)&&(year%100!=0))||(year%400==0));
}


time64_t mktime64(tm* time, long timezone ) {
	time64_t rt_time64;
	// 1..12 -> 11,12,1..10, Puts Feb last since it has leap day
	int32_t month = time->tm_mon + 1;
	int32_t year = time->tm_year + 1900;

	if (month <= 2) {
		month += 10;
		year -= 1;
	} else {
		month -= 2;
	}

	rt_time64 = (time64_t)(year/4 - year/100 + year/400) //leap year total days
		+ 367*month/12 //
		+ time->tm_mday //
		+ year*365 - kTimeBegin1970;

	rt_time64 = ((rt_time64*24 + time->tm_hour )*60 
			  + time->tm_min )*60 
			  + time->tm_sec;            // finally seconds

	rt_time64 -= timezone * 60 * 60;
	return rt_time64;
}

void localtime64(tm *time, time64_t rt_time64,long timezone ) {
	const int monthLengths[2][13] = {
		{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
		{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366},
	};
	const int yearLengths[2] = { 365, 366 };
	int year(0);
	int month(0);
	int minMonth(0);
	int maxMonth(0);

	int days(0);
	int clock(0);
	int isLeap(0);

	rt_time64 += timezone*kSecPerHour;

	days = rt_time64 / kSecPerDay;
	clock = rt_time64 % kSecPerDay;

	/**/////////////////////////////////////////////////////////////////////////////
	// Calcaulate year, 11323=0x3A4FC880/86400; 13879=0x47798280/86400
	/**////////////////////////////////////////////////////////////////////////////    
	if(days >= 0)
	{
		year = days/366;
		days -= year*365 + (year+1)/4 - (year+69)/100 + (year+369)/400;

		for(time->tm_year = year + 1970; ; time->tm_year++)
		{
			isLeap = isleap64(time->tm_year);
			if(days < yearLengths[isLeap])
			{
				break;
			}
			days -= yearLengths[isLeap];
		}
	}

	/**///////////////////////////////////////////////////////////////////////////
	// compute month and day, use the half search save time
	/**////////////////////////////////////////////////////////////////////////////
	minMonth = 0;
	maxMonth = 12;
	for(month = 5; month < kMonthesPerYear && month > 0; month = (minMonth + maxMonth) / 2)
	{
		// days between monthLengths[month]<=days<monthLengths[month+1]
		if(days < monthLengths[isLeap][month])    //too big
		{
			maxMonth = month;
		}
		else if(days >= monthLengths[isLeap][month + 1])    //too small
		{
			minMonth = month;
		}
		else    //so it is
		{
			break;
		}
	}
	days -= monthLengths[isLeap][month];
	time->tm_mon = month;

	time->tm_mday = days + 1;


	/**///////////////////////////////////////////////////////////////////////////
	// Calcaulate hour minute and second
	/**///////////////////////////////////////////////////////////////////////////
	time->tm_hour = clock / kSecPerHour;        //3600s one hour
	clock = clock % kSecPerHour;
	time->tm_min = clock / kSecPerMin;        //60s one minute
	time->tm_sec = clock % kSecPerMin;        //ms
	time->tm_year -= 1900;            //   
	
}


/*========================================================================
 * Function name        : gmt_convert_local
 * Description            : 将格林尼治时间转换为本地时间
 * Return type                : bool 
 * Argument         : 
 *                  : const char* gmt 格林尼治时间 20091105082121
 *                  : char* local        20091105/162121
 ========================================================================*/
bool gmt_convert_local(const char* gmt, char* local)
{
	if ( strlen(gmt) != 14 )
		return false;
	//格林尼治时间转换为本地时间
	char year[5];
	char month[3];
	char day[3];
	char hour[3];
	char minute[3];
	char second[3];

	sscanf(gmt, "%4s%2s%2s%2s%2s%2s", year, month, day, hour, minute, second);

	tm utc_time;
	tm local_time;

	utc_time.tm_year = atoi(year);
	utc_time.tm_mon = atoi(month);
	utc_time.tm_mday = atoi(day);
	utc_time.tm_hour = atoi(hour);
	utc_time.tm_min = atoi(minute);
	utc_time.tm_sec = atoi(second);

	time64_t time_second = mktime64(&utc_time, 0);//utc时间， 0 时区

	localtime64(&local_time, time_second,8);

	return true;
}

