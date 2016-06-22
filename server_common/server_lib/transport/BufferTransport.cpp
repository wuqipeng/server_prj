#include "BufferTransport.h"
#include <string.h>
#include "transport.h"

using namespace server::transport;

TsocketBuffer::TsocketBuffer(int32_t block_size_, int32_t blocks_, boost::shared_ptr<Transport> transport_):
    BuffBase(block_size_, blocks),
    transport(transport_){
   
    }


int32_t TsocketBuffer::readSlow(char *p, int32_t size)
{
    char* p_data  = getData(); 
    char* p_read  = getReadData();
    char* r_bound = getReadBound();
    uint32_t have = static_cast<uint32_t>(r_bound - p_read);
    assert((have < size));
    
    //如果数据不够，则这次读就只返回剩余缓存数据
    if(have > 0)
    {
        memcpy(p,p_read,have);
        setReadData(p_data, 0);
        return have;
    }

    //当缓存为0时，则从套接字里面读取，填满缓存区(有可能没填满)
    setReadData(p_data, transport->read(p_data, input_buffer.getSize()) );
}

//底层transport->write接口能保证100%写入要求的数据量嘛？

int32_t TsocketBuffer::writeSlow(char* p, int32_t size)
{
    char* p_data  = getData();
    char* p_write = getWriteData();
    char* w_bound = getWriteBound();
    
    uint32_t free_space = static_cast<uint32_t>(w_bound - p_write);
    assert((free_space < size));

    uint32_t have = static_cast<uint32_t>(p_write - p_data);
    //为了减少系统调用次数，分情况来write
    //首先，如果写缓存中已有的数据量+size超过了2倍的写缓存的容量，那么可能需要两次系统调用
    
    if((have+size) >= (2*getSize()) || (have==0))
    {
        //如果写缓存中已有的数据量+size超过了2倍的写缓存的容量，首先把缓存数据flush掉，然后再直接底层write
        if(have > 0)
        {
            transport->write(p_data, have); 
            setWriteData(p_data);
        }

        //如果是单纯的have = 0,而且size超过了写缓存空间，直接底层write
        transport->write(p, size);
        return;
    }

    //如果这次待写数据量没有超过两倍，先把这次的要写的数据填满写缓存区，然后底层write掉，最后把剩下代写的数据直接copy到写缓存，这样只是进行了一次系统调用。

    memcpy(p_write, p, free_space);
    transport->write(p_data, getSize());

    char* rest_buf = p+free_space;
    uint32_t rest_size = size-free_space;
    memcpy(p_data, rest_buf, rest_size);
    setWriteData(p_data+rest_size);
}


BufferTransport::BufferTransport(int32_t block_size_, int32_t blocks_, boost::shared_ptr<Transport> transport_) : 
    transport(transport_),
    input_buffer(block_size_, blocks_, transport_),
    out_buffer(block_size_, blocks_, transport_){

}

void BufferTransport::flush()
{
    char* p_data  = out_buffer.getData();
    char* p_write = out_buffer.getWriteData();
    uint32_t have = static_cast<uint32_t>(p_write - p_data);
    transport->write(p_data,size);

    out_buffer.setWriteData(p_data);
}


