FLAGS = -pthread -lrt

INC = -I ./ \
	  -I ./log4cplus_1.1.3_lib/include/

LIB = ./log4cplus_1.1.3_lib/lib/liblog4cplus.a

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

.SUFFIXES: .o .cpp
.cpp.o:
	g++ -c -o $@ $< $(INC)

TARGET = libcommon.a

all: $(TARGET)

$(TARGET) : $(OBJS) 
	ar crs $(TARGET) $(OBJS)

clean:
	rm -f *.o
	rm -f *.a
