# Makefile for Graph Assignment

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

# Source files
MAIN_SRC = main.cpp

# Executables
MAIN_EXEC = main
TEST_EXEC = tests

# Default target
all: Main test

# Compile the main program
Main: $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -o $(MAIN_EXEC) $(MAIN_SRC)
	./$(MAIN_EXEC)

# Compile the test program
test: tests.cpp
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) tests.cpp
	./$(TEST_EXEC)

# Build main without running it (for valgrind)
build-main: $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -o $(MAIN_EXEC) $(MAIN_SRC)

# Build tests without running them (for valgrind)
build-test: tests.cpp
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) tests.cpp

# Run valgrind on the main program
valgrind: build-main
	$(VALGRIND) ./$(MAIN_EXEC)

# Run valgrind on the test program
valgrind-test: build-test
	$(VALGRIND) ./$(TEST_EXEC)

# Clean up
clean:
	rm -f $(MAIN_EXEC) $(TEST_EXEC)

.PHONY: all Main test build-main build-test valgrind valgrind-test clean