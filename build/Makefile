CC = g++
CXXFLAGS = -std=c++0x
#CFLAGS = -std=c++0x -O2 -Wall -g 

TARGET = main
#OBJS = ../code/skiplist.cpp  \
../stress-test/stress-test.cpp

OBJS = ../code/skiplist.cpp  \
../code/stress_bench.cpp  \
../code/main.cpp

all: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o ../bin/$(TARGET) --std=c++11  -pthread

clean:
	rm -rf ../bin/$(OBJS) $(TARGET)




