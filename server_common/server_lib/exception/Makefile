INCLUDE := -l ./

SOURCE:= TransportException.cpp main.cpp 
OBJ := $(SOURCE:.cpp=.o)

PROGRAM = test

.SUFFIXES: .o .cpp
.cpp.o:
	g++  $(INCLUDE) -c -o $@ $<

all: $(PROGRAM)

$(PROGRAM): $(OBJ) 
	g++ -o $@ $(OBJ)

clean:
	rm -rf $(PROGRAM) $(OBJ)
