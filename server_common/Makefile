INC = -I ../ \
	  -I ../lib \
	  -I ../lib/log4cplus_1.1.3_lib/include/   

LIB = ../lib/log4cplus_1.1.3_lib/lib/liblog4cplus.a \

FLAG = -pthread -lrt

SRCS = TSocktet.cpp

OBJS := $(SRCS:.cpp=.o)


.SUFFIXES: .o .cpp
.cpp.o:
	$(CXX) -c -o $@ $< $(INC)

TARGET = test_d

all : $(TARGET)

$(TARGET) : $(OBJS) $(LIB) 
	g++ $(FLAG) -o $@ $(OBJS) $(LIB) 


.PHONY : clean
clean :
	-rm -f *.o  
	-rm -f $(TARGET)
