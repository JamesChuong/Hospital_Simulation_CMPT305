## This is a simple Makefile

# Define what compiler to use and the flags.
#CC=cc
#CXX=g++
#CCFLAGS= -g -std=c++11 -Wall -Werror
#LDLIBS= -lm
#all: test_list
#all: main

# Compile all .c files into .o files
# % matches all (like * in a command)
# $< is the source file (.c file)
#%.o : %.cpp
#	$(CXX) -c $(CCFLAGS) $<

#EvaluationQueue: EvaluationQueue.o
#	$(CXX) -o EvaluationQueue EvaluationQueue.o  $(CCFLAGS) $(LDLIBS)

#PriorityQueue: PriorityQueue.o
#	$(CXX) -o PriorityQueue PriorityQueue.o  $(CCFLAGS) $(LDLIBS)

#RoomCleanUpQueue: RoomCleanUpQueue.o
#	$(CXX) -o RoomCleanUpQueue RoomCleanUpQueue.o  $(CCFLAGS) $(LDLIBS)

##	$(CXX) -o EventList EventList.o  $(CCFLAGS) $(LDLIBS)

###################################
# BEGIN SOLUTION
#main: main.o
#	$(CXX) -o main main.o  $(CCFLAGS) $(LDLIBS)


#clean:
#	rm -f core *.o main

# Compiler
CC = g++

# Compiler flags
CFLAGS = -std=c++11 -Wall

# Source files
SRCS = main.cpp EvaluationQueue.cpp PriorityQueue.cpp RoomCleanUpQueue.cpp EventList.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
EXEC = hw6

# Default target
all: $(EXEC)

# Compile source files into object files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up
clean:
	rm -f $(OBJS) $(EXEC)