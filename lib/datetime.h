/// \file datetime.h
/// \brief Declares classes to add SQL-compatible date and time
/// types to C++'s type system.

/***********************************************************************
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

#if !defined(MYLIB_DATETIME_H)
#define MYLIB_DATETIME_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <sys/time.h>
#include "time64.h"

namespace mylib {

/// \brief C++ form of SQL's DATETIME type.
///
/// This object exists primarily for conversion purposes.  You can
/// initialize it in several different ways, and then convert the object
/// to SQL string form, extract the individual y/m/d h:m:s values,
/// convert it to C's time_t, etc.

class DateTime
{
public:
	/// \brief Default constructor
	DateTime() :
	year_(0),
	month_(0),
	day_(0),
	hour_(0),
	minute_(0),
	second_(0),
	now_(true)
	{
	}

	/// \brief Initialize object from discrete y/m/d h:m:s values.
	///
	/// \param y year, 1000-9999
	/// \param mon month, 1-12
	/// \param d day of month, 1-31
	/// \param h hour, 0-23
	/// \param min minute, 0-59
	/// \param s second, 0-59
	DateTime(unsigned short y, unsigned char mon, unsigned char d,
			unsigned char h, unsigned char min, unsigned char s) :
	year_(y),
	month_(mon),
	day_(d),
	hour_(h),
	minute_(min),
	second_(s),
	now_(false)
	{
	}
	
	/// \brief Initialize object as a copy of another Date
	DateTime(const DateTime& other) :
	year_(other.year_),
	month_(other.month_),
	day_(other.day_),
	hour_(other.hour_),
	minute_(other.minute_),
	second_(other.second_),
	now_(other.now_)
	{
	}

	/// \brief Initialize object from a C string containing a SQL
	/// date-and-time string
	///
	/// String must be in the HH:MM:SS format.  It doesn't have to be
	/// zero-padded.
	explicit DateTime(const char* str) { convert(str); }
	
	/// \brief Initialize object from a C++ string containing a
	/// SQL date-and-time string
	///
	/// This works with any stringish class that declares a c_str()
	/// member function: std::string, mysqlpp::String...
	///
	/// \sa DateTime(const char*)
	template <class Str>
	explicit DateTime(const Str& str)
	{
		convert(str.c_str());
	}

	/// \brief Initialize object from a \c time_t
	explicit DateTime(time_t t);

	//zcw
	/// \brief Initialize object from a \c time64_t
	explicit DateTime(time64_t t);

	/// \brief Compare this object to another.
	///
	/// Returns < 0 if this object is before the other, 0 of they are
	/// equal, and > 0 if this object is after the other.
	int compare(const DateTime& other) const;

	/// \brief Parse a SQL date and time string into this object.
	const char* convert(const char*);

	/// \brief Get the date/time value's day part, 1-31
	unsigned char day() const { return day_; }

	/// \brief Change the date/time value's day part, 1-31
	void day(unsigned char d) { day_ = d; now_ = false; }

	/// \brief Get the date/time value's hour part, 0-23
	unsigned char hour() const { return hour_; }

	/// \brief Change the date/time value's hour part, 0-23
	void hour(unsigned char h) { hour_ = h; now_ = false; }

	/// \brief Returns true if object will evaluate to SQL "NOW()" on
	/// conversion to string.
	bool is_now() const { return now_; }

	/// \brief Get the date/time value's minute part, 0-59
	unsigned char minute() const { return minute_; }

	/// \brief Change the date/time value's minute part, 0-59
	void minute(unsigned char m) { minute_ = m; now_ = false; }

	/// \brief Get the date/time value's month part, 1-12
	unsigned char month() const { return month_; }

	/// \brief Change the date/time value's month part, 1-12
	void month(unsigned char m) { month_ = m; now_ = false; }

	/// \brief Factory to create an object instance that will convert
	/// to SQL "NOW()" on insertion into a query
	///
	/// This is just syntactic sugar around the default ctor
	static DateTime now() { return DateTime(); }

	/// \brief Convert to std::string
	operator std::string() const;

	/// \brief Convert to time_t
	operator time_t() const;

	/// \brief Convert to time64_t
	operator time64_t() const;

	/// \brief Get the date/time value's second part, 0-59
	unsigned char second() const { return second_; }

	/// \brief Change the date/time value's second part, 0-59
	void second(unsigned char s) { second_ = s; now_ = false; }

	/// \brief Return our value in std::string form
	std::string str() const { return *this; }

	/// \brief Return NumberString with no space
	std::string ToNumberString()
	{
		char buf[128] = {0};
		sprintf(buf, "%d%02d%02d%02d%02d%02d", year_, month_, day_, hour_, minute_, second_);
		return std::string(buf);
	}

	/// \brief Get the date/time value's year part
	///
	/// There's no trickery here like in some date/time implementations
	/// where you have to add 1900 or something like that.  It simply
	/// returns the year in natural form, in the range 1000-9999.
	unsigned short year() const { return year_; }

	/// \brief Change the date/time value's year part
	///
	/// Pass the year value normally; we don't optimize the value by
	/// subtracting 1900 like some other date/time implementations.
	void year(unsigned short y) { year_ = y; now_ = false; }

	void InitDateTime(time_t uTime);

	void InitDateTime(time64_t time);

	void IncDays(int n);
	time_t getDate() const;
	time64_t getDate64() const;
	std::string getDateStr() const;
	bool sameDate(time64_t t) const;
	
	time_t firstDateInWeek() const;
	time_t ChinaLastDateInWeek() const;
	time_t firstDateInMonth() const;
	time_t firstDateInNextMonth() const;

private:
	unsigned short year_;	///< the year, as a simple integer
	unsigned char month_;	///< the month, 1-12
	unsigned char day_;		///< the day, 1-31
	unsigned char hour_;	///< the hour, 0-23 (not 0-255 as in Time!)
	unsigned char minute_;	///< the minute, 0-59
	unsigned char second_;	///< the second, 0-59

	bool now_;	///< true if object not initialized with explicit value
};


/// \brief Returns a DateTime object that, when inserted into query
/// will yield a SQL "NOW()" function call.
inline DateTime NOW() { return DateTime(); }


/// \brief Inserts a DateTime object into a C++ stream in a
/// SQL-compatible format.
///
/// The date and time are inserted into the stream, in that order,
/// with a space between them.
///
/// \param os stream to insert date and time into
/// \param dt date/time object to insert into stream
std::ostream& operator <<(std::ostream& os,
		const DateTime& dt);


/// \brief C++ form of SQL's DATE type.
///
/// Objects of this class can be inserted into streams, and
/// initialized from SQL DATE strings.
class Date
{
public:
	/// \brief Default constructor
	Date() : year_(0), month_(0), day_(0) { }

	/// \brief Initialize object
	///
	/// \param y year, 1000-9999
	/// \param m month, 1-12
	/// \param d day of month, 1-31
	Date(unsigned short y, unsigned char m, unsigned char d) :
	year_(y),
	month_(m),
	day_(d)
	{
	}
	
	/// \brief Initialize object as a copy of another Date
	Date(const Date& other) :
	year_(other.year_),
	month_(other.month_),
	day_(other.day_)
	{
	}

	/// \brief Initialize object from date part of date/time object
	Date(const DateTime& other) :
	year_(other.year()),
	month_(other.month()),
	day_(other.day())
	{
	}

	/// \brief Initialize object from a C string containing a date
	///
	/// String must be in the YYYY-MM-DD format.  It doesn't have to be
	/// zero-padded.
	explicit Date(const char* str) { convert(str); }
	
	/// \brief Initialize object from a C++ string containing a date
	///
	/// This works with any stringish class that declares a c_str()
	/// member function: std::string, mysqlpp::String...
	///
	/// \sa Date(const char*)
	template <class Str>
	explicit Date(const Str& str) { convert(str.c_str()); }

	/// \brief Initialize object from a \c time_t
	///
	/// Naturally, we throw away the "time" part of the \c time_t.  If
	/// you need to keep it, you want to use DateTime instead.
	explicit Date(time_t t);

	/// zcw
	/// \brief Initialize object from a \c time64_t
	explicit Date(time64_t t);

	/// \brief Compare this date to another.
	///
	/// Returns < 0 if this date is before the other, 0 of they are
	/// equal, and > 0 if this date is after the other.
	int compare(const Date& other) const;

	/// \brief Parse a SQL date string into this object.
	const char* convert(const char*);

	/// \brief Get the date's day part, 1-31
	unsigned char day() const { return day_; }

	/// \brief Change the date's day part, 1-31
	void day(unsigned char d) { day_ = d; }

	/// \brief Get the date's month part, 1-12
	unsigned char month() const { return month_; }

	/// \brief Change the date's month part, 1-12
	void month(unsigned char m) { month_ = m; }

	/// \brief Convert to std::string
	operator std::string() const;

	/// \brief Convert to time_t
	///
	/// The "time" part of the \c time_t is "now"
	operator time_t() const;

	//zcw
	/// \brief Convert to time64_t
	/// The "time" part of the \c time_t is "now"
	operator time64_t() const;

	/// \brief Return our value in std::string form
	std::string str() const { return *this; }

	/// \brief Get the date's year part
	///
	/// There's no trickery here like in some date implementations
	/// where you have to add 1900 or something like that.
	unsigned short year() const { return year_; }

	/// \brief Change the date's year part
	///
	/// Pass the year value normally; we don't optimize the value by
	/// subtracting 1900 like some other date implementations.
	void year(unsigned short y) { year_ = y; }

private:
	unsigned short year_;	///< the year, as a simple integer, 1000-9999
	unsigned char month_;	///< the month, 1-12
	unsigned char day_;		///< the day, 1-31
};

/// \brief Inserts a Date object into a C++ stream
///
/// The format is YYYY-MM-DD, zero-padded.
///
/// \param os stream to insert date into
/// \param d date to insert into stream
std::ostream& operator <<(std::ostream& os,
		const Date& d);


/// \brief C++ form of SQL's TIME type.
///
/// Objects of this class can be inserted into streams, and
/// initialized from SQL TIME strings.
class Time
{
public:
	/// \brief Default constructor
	Time() : hour_(0), minute_(0), second_(0) { }

	/// \brief Initialize object
	/// \param h hour, 0-255 (yes, > 1 day is legal in SQL!)
	/// \param m minute, 0-59
	/// \param s second, 0-59
	Time(unsigned char h, unsigned char m, unsigned char s) :
	hour_(h),
	minute_(m),
	second_(s)
	{
	}

	/// \brief Initialize object as a copy of another Time
	Time(const Time& other) :
	hour_(other.hour_),
	minute_(other.minute_),
	second_(other.second_)
	{
	}

	/// \brief Initialize object from time part of date/time object
	Time(const DateTime& other) :
	hour_(other.hour()),
	minute_(other.minute()),
	second_(other.second())
	{
	}

	/// \brief Initialize object from a C string containing a SQL
	/// time string
	///
	/// String must be in the HH:MM:SS format.  It doesn't have to be
	/// zero-padded.
	explicit Time(const char* str) { convert(str); }

	/// \brief Initialize object from a C++ string containing a
	/// SQL time string
	///
	/// This works with any stringish class that declares a c_str()
	/// member function: std::string, mysqlpp::String...
	///
	/// \sa Time(const char*)
	template <class Str>
	explicit Time(const Str& str) { convert(str.c_str()); }

	/// \brief Initialize object from a \c time_t
	///
	/// Naturally, we throw away the "date" part of the \c time_t.  If
	/// you need to keep it, you want to use DateTime instead.
	explicit Time(time_t t);

	/// \brief Initialize object from a \c time64_t
	///
	/// Naturally, we throw away the "date" part of the \c time_t.  If
	/// you need to keep it, you want to use DateTime instead.
	explicit Time(time64_t t);

	/// \brief Compare this time to another.
	///
	/// Returns < 0 if this time is before the other, 0 of they are
	/// equal, and > 0 if this time is after the other.
	int compare(const Time& other) const;

	/// \brief Parse a SQL time string into this object.
	const char* convert(const char*);

	/// \brief Get the time's hour part, 0-255
	unsigned char hour() const { return hour_; }

	/// \brief Change the time's hour part, 0-255
	void hour(unsigned char h) { hour_ = h; }

	/// \brief Get the time's minute part, 0-59
	unsigned char minute() const { return minute_; }

	/// \brief Change the time's minute part, 0-59
	void minute(unsigned char m) { minute_ = m; }

	/// Convert to std::string
	operator std::string() const;

	/// \brief Convert to time_t
	///
	/// The "date" part of the \c time_t is "today"
	operator time_t() const;

	/// \brief Convert to time64_t
	///
	/// The "date" part of the \c time_t is "today"
	operator time64_t() const;

	/// \brief Get the time's second part, 0-59
	unsigned char second() const { return second_; }

	/// \brief Change the time's second part, 0-59
	void second(unsigned char s) { second_ = s; }

	/// Return our value in std::string form
	std::string str() const { return *this; }

private:
	unsigned char hour_;	///< the hour, 0-255 (yes, > 1 day is legal SQL!)
	unsigned char minute_;	///< the minute, 0-59
	unsigned char second_;	///< the second, 0-59
};

/// \brief Inserts a Time object into a C++ stream in a SQL-compatible
/// format.
///
/// The format is HH:MM:SS, zero-padded.
///
/// \param os stream to insert time into
/// \param t time to insert into stream
std::ostream& operator <<(std::ostream& os,
		const Time& t);

unsigned int DiffDays(const DateTime &dt1,const DateTime &dt2);
int DiffDaysInt(const DateTime& begin, const DateTime& end);

void AddMonths(DateTime& dt, unsigned int monthAdd);
uint64_t getCurrentTimeUs();

} // end namespace mysqlpp

#endif // !defined(MYSQLPP_DATETIME_H)

