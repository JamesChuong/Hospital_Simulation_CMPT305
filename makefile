## This is a simple Makefile

# Define what compiler to use and the flags.
CC=cc
CXX=g++
CCFLAGS= -g -std=c++11 -Wall -Werror
LDLIBS= -lm
#all: test_list
all: main

# Compile all .c files into .o files
# % matches all (like * in a command)
# $< is the source file (.c file)
%.o : %.cpp
	$(CXX) -c $(CCFLAGS) $<


###################################
# BEGIN SOLUTION
main: main.o EvaluationQueue.o PriorityQueue.o RoomCleanUpQueue.o EventList.o
	$(CXX) -o main main.o  $(CCFLAGS) $(LDLIBS)


clean:
	rm -f core *.o main