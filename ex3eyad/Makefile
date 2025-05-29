CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -Werror -pedantic -fPIC
QTFLAGS = $(shell pkg-config --cflags --libs Qt5Widgets)

# Define paths
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = test
BUILD_DIR = build

# Find all source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/Roles/*.cpp)
GUI_SOURCES = $(wildcard $(SRC_DIR)/GUI/*.cpp)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.hpp) $(wildcard $(INCLUDE_DIR)/Roles/*.hpp) $(wildcard $(INCLUDE_DIR)/GUI/*.hpp)
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)

# Define object files
SOURCES_NO_MAIN = $(filter-out $(SRC_DIR)/Main.cpp $(SRC_DIR)/Demo.cpp, $(SOURCES))
OBJECTS_NO_MAIN = $(SOURCES_NO_MAIN:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
GUI_OBJECTS = $(GUI_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEMO_OBJ = $(BUILD_DIR)/Demo.o
MAIN_OBJ = $(BUILD_DIR)/Main.o
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Define executable names
DEMO_EXEC = $(BUILD_DIR)/Demo
MAIN_EXEC = $(BUILD_DIR)/Main
GUI_EXEC = $(BUILD_DIR)/CoupGUI
TEST_EXEC = $(BUILD_DIR)/test

# MOC (Qt Meta-Object Compiler) command
MOC = moc

# Targets
.PHONY: all clean Demo Main GUI test valgrind

all: Main GUI test

# Create build directory structure
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/Roles
	mkdir -p $(BUILD_DIR)/GUI

# Generate meta-object code for Qt classes
$(BUILD_DIR)/moc_%.cpp: $(INCLUDE_DIR)/GUI/%.hpp | $(BUILD_DIR)
	$(MOC) $< -o $@

# Object file compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Build meta-objects for QObject derived classes
$(BUILD_DIR)/GUI/MainWindow.o: $(SRC_DIR)/GUI/MainWindow.cpp $(INCLUDE_DIR)/GUI/MainWindow.hpp $(BUILD_DIR)/moc_MainWindow.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(QTFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/moc_MainWindow.o: $(BUILD_DIR)/moc_MainWindow.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(QTFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(BUILD_DIR)/GUI/main.o: $(SRC_DIR)/GUI/main.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(QTFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Demo executable
Demo: $(OBJECTS_NO_MAIN) $(DEMO_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(DEMO_EXEC)
	@echo "Demo executable built successfully"
	@echo "Run with: $(DEMO_EXEC)"

# Main executable (command-line version)
Main: $(OBJECTS_NO_MAIN) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(MAIN_EXEC)
	@echo "Main executable built successfully"
	@echo "Run with: $(MAIN_EXEC)"

# GUI executable
GUI: $(OBJECTS_NO_MAIN) $(BUILD_DIR)/moc_MainWindow.o $(BUILD_DIR)/GUI/MainWindow.o $(BUILD_DIR)/GUI/main.o
	$(CXX) $(CXXFLAGS) $^ $(QTFLAGS) -o $(GUI_EXEC)
	@echo "GUI executable built successfully"
	@echo "Run with: $(GUI_EXEC)"

# Test executable
test: $(OBJECTS_NO_MAIN) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $(TEST_EXEC)
	@echo "Running tests..."
	@./$(TEST_EXEC)

# Valgrind check for memory leaks
valgrind: Demo
	valgrind --leak-check=full --error-exitcode=1 $(DEMO_EXEC)

# Clean up
clean:
	rm -rf $(BUILD_DIR)