#ifndef BUFFER_BASE_H_
#define BUFFER_BASE_H_

#include <string.h>
#include <stdint.h>
namespace server { namespace transport {

class BufferBase
{
    public:
        BufferBase(int32_t block_size_, int32_t blocks_):
            block_size(block_size_),
            blocks(blocks_){
                p_data  = new char[blocks*block_size]; 
                p_read  = p_data;
                r_bound = p_data;
                p_write = p_data;
                w_bound = p_data + blocks*block_size;
            }
        virtual ~BufferBase()
        {
            delete [] p_data;
        }
   
        int32_t readBuff(char *p, int32_t size);
        virtual int32_t readSlow(char *p, int32_t size) = 0;
        
        int32_t writeBuff(char *p, int32_t size);
        virtual int32_t writeSlow(char* p, int32_t size) = 0;

        char* getData()
        {
            return p_data;
        }

        char* getReadData()
        {
            return p_read;
        }

        char* getReadBound()
        {
            return r_bound;
        }
        
        void setReadData(const char* p, int32_t size)
        {
            p_read  = p;
            r_bound = p + size;
        }

        char* getWriteData()
        {
            return p_write;
        }

        char* getWriteBound()
        {
            return w_bound;
        }

        void setWriteData(const char* p)
        {
            p_write = p;
        }
        
        int32_t getSize()
        {
            return block_size * blocks;
        }

    private:
        int32_t block_size;
        int32_t blocks;
        char* p_data;
        
        char* p_read;
        char* r_bound;
        char* p_write;
        char* w_bound;
       
};

int32_t BufferBase::readBuff(char *p, int32_t size)
{
    int32_t data_size = r_bound - p_read;
    if(data_size >= size)
    {
        memcpy(p,p_read,size);
        p_read += size;
        return size;
    }

    return readSlow(p,size);
}

int32_t BufferBase::writeBuff(char *p, int32_t size)
{
    int32_t free_size = w_bound - p_write;
    if(free_size >= size)
    {
        memcpy(p_write,p,size);
        p_write += size;
    }

    return writeSlow(p, size);
}

}}

#endif
