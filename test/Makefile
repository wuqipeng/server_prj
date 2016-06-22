INC = -I ../ \
	  -I ../lib \
	  -I ../lib/log4cplus_1.1.3_lib/include/   

LIB = ../lib/libcommon.a \
	  ../lib/log4cplus_1.1.3_lib/lib/liblog4cplus.a \
      /usr/lib64/libboost_thread-mt.so

FLAG = -g -pthread -lrt
# comm
SRCS = main.cpp 

OBJS := $(SRCS:.cpp=.o)


.SUFFIXES: .o .cpp
.cpp.o:
	g++ $(FLAG) -c -o $@ $< $(INC)

TARGET = test_d

all : $(TARGET)

$(TARGET) : $(OBJS) $(LIB) 
	g++ $(FLAG) -o $@ $(OBJS) $(LIB) 


.PHONY : clean
clean :
	-rm -f *.o  
	-rm -f $(TARGET)
