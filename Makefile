# Makefile
CXX = g++
AS = as
# Added -pthread here. -no-pie is for the assembly link.
CXXFLAGS = -Wall -std=c++17 -no-pie -pthread
DEBUG_FLAGS = -g -DDEBUG_MODE
OPT_FLAGS = -O3

OBJS = main.o NetworkSim.o Basicio.o syscall.o

all: debug optimized

debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean_objs $(OBJS)
	$(CXX) $(CXXFLAGS) -o sim_debug $(OBJS)

optimized: CXXFLAGS += $(OPT_FLAGS)
optimized: clean_objs $(OBJS)
	$(CXX) $(CXXFLAGS) -o sim_opt $(OBJS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

NetworkSim.o: NetworkSim.cpp
	$(CXX) $(CXXFLAGS) -c NetworkSim.cpp

Basicio.o: Basicio.cpp
	$(CXX) $(CXXFLAGS) -c Basicio.cpp

syscall.o: syscall.s
	$(AS) -o syscall.o syscall.s

clean_objs:
	rm -f *.o

clean:
	rm -f *.o sim_debug sim_opt