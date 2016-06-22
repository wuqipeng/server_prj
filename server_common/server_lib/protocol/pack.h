#ifndef PACK_H_
#define PACK_H_

#include <string>
#include <vector>
#include <set>
#include <map>

namespace server{ namespace protocol {

struct Header
{
    uint32_t length;
    uint32_t url;
    int16_t  res_code;
};

class Pack
{
    public:
        Pack(int32_t blocks, int32_t max_blocks):buf_(blocks, max_blocks){}
        void writeByte(int8_t i8);
        void writeI16(int16_t i16);
        void writeI32(int32_t i32);
        void writeI64(int64_t i64);
        void writeString(std::string str);
        template<typename T> void writeVector(std::vector<T> vec);
        template<typename T> void writeSet(std::set<T> s);
        template<typename T1, typename T2> void writeMap(std::map<T1, T2> m);
        
        inline Pack& operator<<(int8_t i8);
        inline Pack& operator<<(int16_t i16);
        inline Pack& operator<<(int32_t i32);
        inline Pack& operator<<(int64_t i64);
        inline Pack& operator<<(std::string str);
        template<typename T> inline Pack& operator<<(std::vector<T> vec);
        template<typename T> inline Pack& operator<<(std::set<T> s);
        template<typename T1, typename T2> inline Pack& operator<<(std::map<T1, T2> m);

    private:
        Header h;
        BlockBuffer buf_;            
};

class Unpack
{
   public:
        Unpack(char* buf, uint32_t size):buf_(buf), size_(size){}
        int8_t  readByte();
        int16_t readI16();
        int16_t readI32();
        int16_t readI64();
        std::string readString();

        template<typename T> void readVector(std::vector<T>& vec);
        template<typename T> void readSet(std::set<T>& s);
        template<typename T1, typename T2> void readMap(std::map<T1, T2>& m);
        
        inline Pack& operator>>(int8_t& i8);
        inline Pack& operator>>(int16_t& i16);
        inline Pack& operator>>(int32_t& i32);
        inline Pack& operator>>(int64_t& i64);
        inline Pack& operator>>(std::string str);
        template<typename T> inline Pack& operator>>(std::vector<T>& vec);
        template<typename T> inline Pack& operator>>(std::set<T>& s);
        template<typename T1, typename T2> inline Pack& operator>>(std::map<T1, T2>& m);
    
    private:
        char* buf_;
        uint32_t size_;
};

}}
#endif
