	/***********************************************************************
 datetime.cpp - Implements date and time classes compatible with MySQL's
	various date and time column types.

 Copyright (c) 1998 by Kevin Atkinson, (c) 1999-2001 by MySQL AB, and
 (c) 2004-2008 by Educational Technology Resources, Inc.  Others may
 also hold copyrights on code in this file.  See the CREDITS.txt file
 in the top directory of the distribution for details.

 This file is part of MySQL++.

 MySQL++ is free software; you can redistribute it and/or modify it
 under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 MySQL++ is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with MySQL++; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
 USA
***********************************************************************/

#include "datetime.h"
#include "stream2string.h"
#include <sys/time.h>
#include <iomanip>

#include <stdlib.h>
//#include <time.h>

#define HAVE_LOCALTIME_R

using namespace std;

namespace mylib {

static void
safe_localtime(struct tm* ptm, const time_t t)
{
#if defined(HAVE_LOCALTIME_R)
	// autoconf detected POSIX's localtime_r() on this system
	localtime_r(&t, ptm);
#else
	// No explicitly thread-safe localtime() replacement found.  This
	// may still be thread-safe, as some C libraries take special steps
	// within localtime() to get thread safety, such as TLS.
	memcpy(ptm, localtime(&t), sizeof(tm));
#endif
}


std::ostream& operator <<(std::ostream& os, const Date& d)
{
	char fill = os.fill('0');
	ios::fmtflags flags = os.setf(ios::right);
	os		<< setw(4) << d.year() << '-'
			<< setw(2) << static_cast<int>(d.month()) << '-'
			<< setw(2) << static_cast<int>(d.day());
	os.flags(flags);
	os.fill(fill);
	return os;
}


std::ostream& operator <<(std::ostream& os, const Time& t)
{
	char fill = os.fill('0');
	ios::fmtflags flags = os.setf(ios::right);
	os		<< setw(2) << static_cast<int>(t.hour()) << ':'
			<< setw(2) << static_cast<int>(t.minute()) << ':'
			<< setw(2) << static_cast<int>(t.second());
	os.flags(flags);
	os.fill(fill);
	return os;
}


std::ostream& operator <<(std::ostream& os, const DateTime& dt)
{
	if (dt.is_now()) {
		return os << "NOW()";
	}
	else {
		operator <<(os, Date(dt));
		os << ' ';
		return operator <<(os, Time(dt));
	}
}


Date::Date(time_t t)
{
	struct tm tm;
	safe_localtime(&tm, t);

	year_ = tm.tm_year + 1900;
	month_ = tm.tm_mon + 1;
	day_ = tm.tm_mday;
}


Date::Date(time64_t t) {
	struct tm tm;
	localtime64(&tm, t);

	year_ = tm.tm_year + 1900;
	month_ = tm.tm_mon + 1;
	day_ = tm.tm_mday;
}


DateTime::DateTime(time_t t)
{
	InitDateTime(t);
}

DateTime::DateTime(time64_t t) {
	InitDateTime(t);
}


Time::Time(time_t t)
{
	struct tm tm;
	safe_localtime(&tm, t);

	hour_ = tm.tm_hour;
	minute_ = tm.tm_min;
	second_ = tm.tm_sec;
}

Time::Time(time64_t t) {
	struct tm tm;
	localtime64(&tm, t);

	hour_ = tm.tm_hour;
	minute_ = tm.tm_min;
	second_ = tm.tm_sec;
}

const char*
Date::convert(const char* str)
{
	char num[5];

	num[0] = *str++;
	num[1] = *str++;
	num[2] = *str++;
	num[3] = *str++;
	num[4] = 0;
	year_ = static_cast<unsigned short>(strtol(num, 0, 10));
	if (*str == '-') str++;

	num[0] = *str++;
	num[1] = *str++;
	num[2] = 0;
	month_ = static_cast<unsigned char>(strtol(num, 0, 10));
	if (*str == '-') str++;

	num[0] = *str++;
	num[1] = *str++;
	num[2] = 0;
	day_ = static_cast<unsigned char>(strtol(num, 0, 10));

	return str;
}


const char*
Time::convert(const char* str)
{
	char num[5];

	num[0] = *str++;
	num[1] = *str++;
	num[2] = 0;
	hour_ = static_cast<unsigned char>(strtol(num,0,10));
	if (*str == ':') str++;

	num[0] = *str++;
	num[1] = *str++;
	num[2] = 0;
	minute_ = static_cast<unsigned char>(strtol(num,0,10));
	if (*str == ':') str++;

	num[0] = *str++;
	num[1] = *str++;
	num[2] = 0;
	second_ = static_cast<unsigned char>(strtol(num,0,10));

	return str;
}


const char*
DateTime::convert( const char* str )
{
	Date d;
	str = d.convert(str);
	year_ = d.year();
	month_ = d.month();
	day_ = d.day();
	
	if (*str == ' ') ++str;

	Time t;
	str = t.convert(str);
	hour_ = t.hour();
	minute_ = t.minute();
	second_ = t.second();

	now_ = false;
	
	return str;
}


int
Date::compare(const Date& other) const
{
	if (year_ != other.year_) return year_ - other.year_;
	if (month_ != other.month_) return month_ - other.month_;
	return day_ - other.day_;
}


int
Time::compare(const Time& other) const
{
	if (hour_ != other.hour_) return hour_ - other.hour_;
	if (minute_ != other.minute_) return minute_ - other.minute_;
	return second_ - other.second_;
}


int
DateTime::compare(const DateTime& other) const
{
	if (now_ && other.now_) {
		return 0;
	}
	else {
		Date d(*this), od(other);
		Time t(*this), ot(other);

		if (int x = d.compare(od)) {
			return x;
		}
		else {
			return t.compare(ot);
		}
	}
}


Date::operator std::string() const
{
	return stream2string(*this);
}


DateTime::operator std::string() const
{
	return stream2string(*this);
}


Time::operator std::string() const
{
	return stream2string(*this);
}


Date::operator time_t() const
{
	struct tm tm;
	safe_localtime(&tm, time(0));

	tm.tm_mday = day_;
	tm.tm_mon = month_ - 1;
	tm.tm_year = year_ - 1900;
	tm.tm_isdst = -1;

	return mktime(&tm);
}

Date::operator time64_t() const {
	struct tm tm;
	safe_localtime(&tm, time(0));

	tm.tm_mday = day_;
	tm.tm_mon = month_ - 1;
	tm.tm_year = year_ - 1900;
	tm.tm_isdst = -1;

	return mktime64(&tm);
}


DateTime::operator time_t() const
{
	if (now_) {
		// Many factors combine to make it almost impossible for this
		// case to return the same value as you'd get if you used this
		// in a query.  But, you gotta better idea than to return the
		// current time for an object initialized with the value "now"?
		return time(0);
	}
	else {
		struct tm tm;
		tm.tm_sec = second_;
		tm.tm_min = minute_;
		tm.tm_hour = hour_;
		tm.tm_mday = day_;
		tm.tm_mon = month_ - 1;
		tm.tm_year = year_ - 1900;
		tm.tm_isdst = -1;

		return mktime(&tm);
	}
}


DateTime::operator time64_t() const {
	if (now_) {
		// Many factors combine to make it almost impossible for this
		// case to return the same value as you'd get if you used this
		// in a query.  But, you gotta better idea than to return the
		// current time for an object initialized with the value "now"?
		return static_cast<time64_t>( time(0) );
	}
	else {
		struct tm tm;
		tm.tm_sec = second_;
		tm.tm_min = minute_;
		tm.tm_hour = hour_;
		tm.tm_mday = day_;
		tm.tm_mon = month_ - 1;
		tm.tm_year = year_ - 1900;
		tm.tm_isdst = -1;

		return mktime64(&tm);
	}
}

Time::operator time_t() const
{
	struct tm tm;
	safe_localtime(&tm, time(0));

	tm.tm_sec = second_;
	tm.tm_min = minute_;
	tm.tm_hour = hour_;
	tm.tm_isdst = -1;

	return mktime(&tm);
}

Time::operator time64_t() const {
	struct tm tm;
	safe_localtime(&tm, time(0));

	tm.tm_sec = second_;
	tm.tm_min = minute_;
	tm.tm_hour = hour_;
	tm.tm_isdst = -1;

	return mktime64(&tm);
}

unsigned int DiffDays(const DateTime &dt1,const DateTime &dt2)
{
    DateTime dtend(dt2.year(), dt2.month(), dt2.day(), 0,0,0);
    DateTime dtstart(dt1.year(), dt1.month(), dt1.day(), 0,0,0);
    if(((time64_t)dtend) > ((time64_t)dtstart))
    {
        return (((time64_t)dtend) - ((time64_t)dtstart))/86400;
    }
    else
    {
        return (((time64_t)dtstart) - ((time64_t)dtend))/86400;
    }
}

int DiffDaysInt(const DateTime& begin, const DateTime& end)
{
	DateTime dtStart(begin.year(), begin.month(), begin.day(), 0, 0, 0);
	DateTime dtEnd(end.year(), end.month(), end.day(), 0, 0, 0);

	return ((time64_t)dtEnd - (time64_t)dtStart) / 86400;
}

void AddMonths(DateTime& dt, unsigned int monthAdd)
{
    unsigned int newmonth =  dt.month() + monthAdd;
    unsigned short newyear =  dt.year();
    while(newmonth > 12)
    {
        newmonth-=12;
        ++newyear;
    }

    dt.year(newyear);
    dt.month(static_cast<unsigned char>(newmonth));
}

uint64_t getCurrentTimeUs()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (uint64_t)((uint64_t)tv.tv_sec*1000000ull + tv.tv_usec);
}

void DateTime::IncDays(int n)
{
	time64_t tmpDate = (*this);
	tmpDate += 86400 * n;

	InitDateTime(tmpDate);
}

time_t DateTime::getDate() const
{
	 time_t d = DateTime(year_, month_, day_, 0, 0, 0);  
	 return d; 
}

time64_t DateTime::getDate64() const {

	time64_t d = DateTime(year_, month_, day_, 0, 0, 0);  
	return d;
}

time_t DateTime::firstDateInMonth() const
{
	return (time_t)DateTime(year_, month_, 1, 0, 0, 0);
}

time_t DateTime::firstDateInNextMonth() const
{
	if (month_ == 12)
		return (time_t) DateTime(year_+1, 1, 1, 0, 0, 0);
	return DateTime(year_, month_ + 1, 1, 0, 0, 0);
}

time_t DateTime::firstDateInWeek() const
{
	struct tm tm;
	safe_localtime(&tm, getDate());
	int i = 0 - tm.tm_wday;
	DateTime tmp(*this);
	tmp.IncDays(i);
	return tmp.getDate();
}

time_t DateTime::ChinaLastDateInWeek() const
{
	struct tm tm;
	safe_localtime(&tm, getDate());
	int i = tm.tm_wday==0?tm.tm_wday:7-tm.tm_wday;
	DateTime tmp(*this);
	tmp.IncDays(i);
	return tmp.getDate();
}

std::string DateTime::getDateStr() const
{
	Date date(year_, month_, day_);
	return (string)date;
}

bool DateTime::sameDate(time64_t t) const
{
	DateTime st(t);
	return (year_ == st.year() && month_ == st.month() && day_ == st.day());
}

void DateTime::InitDateTime(time_t t)
{
	struct tm tm;
	safe_localtime(&tm, t);

	year_ = tm.tm_year + 1900;
	month_ = tm.tm_mon + 1;
	day_ = tm.tm_mday;
	hour_ = tm.tm_hour;
	minute_ = tm.tm_min;
	second_ = tm.tm_sec;

	now_ = false;
}

void DateTime::InitDateTime(time64_t time) {
	struct tm tm;
	localtime64( &tm ,time );
	year_ = tm.tm_year + 1900;
	month_ = tm.tm_mon + 1;
	day_ = tm.tm_mday;
	hour_ = tm.tm_hour;
	minute_ = tm.tm_min;
	second_ = tm.tm_sec;
	now_ = false;
}

} // end namespace mysqlpp

