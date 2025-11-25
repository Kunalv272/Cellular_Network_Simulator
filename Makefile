# Compiler and tools
CXX       = g++
CXXFLAGS  = -std=c++17 -O2 -pthread
NASM      = nasm
NASMFLAGS = -f elf64

# Target
TARGET = sim_opt

# Sources and objects
SRCS = main.cpp NetworkSim.cpp basicIO.cpp
OBJS = $(SRCS:.cpp=.o) syscall.o

.PHONY: all clean

all: $(TARGET)

# Link with custom _start from syscall.o
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -nostartfiles $(OBJS) -o $(TARGET)

# C++ compilation
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Assemble syscall.s with NASM
syscall.o: syscall.s
	$(NASM) $(NASMFLAGS) $< -o $@

clean:
	rm -f *.o $(TARGET)
