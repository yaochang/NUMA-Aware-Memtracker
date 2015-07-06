CXX=g++
CXXFLAGS=-c -O3 -g -w -fmessage-length=0 -pthread -std=c++11 -lrt -lnuma
CC=$(CXX) $(CXXFLAGS)

OBJS=MemTrack.o MemoryManager.o
all:
test: TEST
TEST:  $(OBJS) TEST.o
	$(CXX) -o TEST  $(OBJS) TEST.o -lpthread -lrt	-lnuma

#execution
MemTrack.o:
	$(CC) MemTrack.cpp -o MemTrack.o
MemoryManager.o:
	$(CC) MemoryManager.cpp -o MemoryManager.o

#test
TEST.o:
	$(CC) TEST.cpp -o TEST.o

clean:
	rm -rf *.o
	rm TEST
