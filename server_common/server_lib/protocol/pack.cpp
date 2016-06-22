#include "pack.h"

using namespace std;
using namespace server::protocol;

void Pack::writeByte(int8_t i8)
{
    buf_.append((char*)(&i8), 1);
}

void Pack::writeI16(int16_t i16)
{
    i16 = hton(i16);
    buf_.append((char*)(&i16), 2);
}

void Pack::writeI32(int32_t i32)
{
    i32 = hton(i32);
    buf_.append((char*)(&i32), 4);
}

void Pack::writeI64(int64_t i64)
{
    i64 = hton(i64);
    buf_.append((char*)(&i64), 8);
}

void Pack::writeString(std::string str)
{
    //字符串的序列化要先写入一个字符串的长度，这样反序列化的时候知道应该取出相应长度的字符串
    writeI32(str.size());
    buf_.append(str.data(), str.size());
}

template<typename T> 
void Pack::writeVector(std::vector<T>& vec)
{
    //先写入容器长度
    writeI32(vec.size());

    for(std::vector<T>::iterator it = vec.begin(); it != vec.end(); it++ )
    {
        (*this)<<(*it);
    }
}

template<typename T> 
void Pack::writeSet(std::set<T>& s)
{
    writeI32(s.size());

    for(std::set<T>::iterator it = s.begin(); it != s.end(); it++ )
    {
        (*this)<<(*it);
    }
}

template<typename T1, typename T2> 
void Pack::writeMap(std::map<T1, T2>& m)
{
    writeI32(m.size());

    for(std::map<T1, T2>::iterator it = m.begin(); it != m.end(); it++ )
    {
        (*this)<<(it->first)<<(it->second);
    }
}

inline Pack& Pack::operator<<(int8_t i8)
{
   writeByte(i8);
   return *this;
}

inline Pack& Pack::operator<<(int16_t i16)
{
    writeI16(i16);
    return *this;
}

inline Pack& Pack::operator<<(int32_t i32)
{
    writeI32(i32);
    return *this;
}

inline Pack& Pack::operator<<(int64_t i64)
{
    writeI64(i64);
    return *this;
}

inline Pack& Pack::operator<<(std::string str)
{
    writeString(str);
    return *this;
}

template<typename T> 
inline Pack& Pack::operator<<(std::vector<T>& vec)
{
    writeVector(vec);
    return *this;
}

template<typename T> 
inline Pack& Pack::operator<<(std::set<T>& s)
{
    writeSet(s);
    return *this;
}

template<typename T1, typename T2> 
inline Pack& Pack::operator<<(std::map<T1, T2>& m)
{
    writeMap(m);
    return *this;
}


