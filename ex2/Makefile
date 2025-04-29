# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Source files and targets
MAIN_SRCS = Main.cpp SquareMat.cpp
TEST_SRCS = Test.cpp SquareMat.cpp

# Object files
MAIN_OBJS = $(MAIN_SRCS:.cpp=.o)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

# Targets
all: Main test

Main: $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	./$@

test: $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	./$@

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run valgrind memory leak check
valgrind: Main
	valgrind --leak-check=full --show-leak-kinds=all ./Main

# Clean up
clean:
	rm -f *.o Main test

.PHONY: all test valgrind clean
