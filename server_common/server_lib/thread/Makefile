FLAG = -g -pthread -lrt  

LIB = ../../../lib/libcommon.a \
	  ../../../lib/log4cplus_1.1.3_lib/lib/liblog4cplus.a \
	  /usr/lib64/libboost_thread.a
#	  ../../../bin/libboost_thread-mt.so

INC = -I ../../../ \
	  -I ../../../lib/ \
	  -I ../../../lib/log4cplus_1.1.3_lib/include/  \

SRCS = $(wildcard *.cpp) \

OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	g++ $(FLAG) -c -o $@ $<  $(INC)

TARGET = test_d

all:$(TARGET)

$(TARGET) : $(OBJS)
	g++ $(FLAG) -o $@ $^ $(LIB)  $(INC)

clean :
	rm -f *.o $(TARGET)


