#ifndef BUFFER_TRANSPORT_H_
#define BUFFER_TRANSPORT_H_

#include "BufferBase.h"

namespace server { namespace transport {

class TsocketBuffer : public BuffBase
{
    public:
        TsocketBuffer(int32_t block_size_, int32_t blocks_, boost::shared_ptr<Transport> transport_);
        virtual int32_t readSlow(char *p, int32_t size);
        virtual int32_t writeSlow(char* p, int32_t size);
    
    private:
        boost::shared_ptr<Transport> transport;
};

class BufferTransport:public Transport
{
    public:
        BufferTransport(int32_t block_size_, int32_t blocks_, boost::shared_ptr<Transport>  transport_);

        virtual int read(const char* buf, const int size)
        {
            return input_buffer.readBuff(buf, size);
        }

        virtual int write(const char* buf, const int size)
        {
            return out_buffer.writeBuff(buf, size);
        }

        virtual void flush();

        virtual void open()
        {
            transport->open();
        }

        virtual void close()
        {
            flush();
            transport->close();
        }

        boost::shared_ptr<Transport> getUnderlyingTransport()
        {
            return transport;
        }

    private:
        boost::shared_ptr<Transport> transport;
        TsocketBuffer input_buffer;
        TsocketBuffer out_buffer;
};

}}
#endif
