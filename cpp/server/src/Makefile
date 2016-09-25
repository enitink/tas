CXX=g++
INC=../inc/
LIBS=-lpthread
CXXFLAGS=-g -std=c++11 -Wall -pedantic -I$(INC) -I../asio/asio-1.11.0/include -DASIO_STANDALONE 
BIN=server

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CXX) -o $(BIN) $^ $(LIBS)

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f *.o
	rm $(BIN)
