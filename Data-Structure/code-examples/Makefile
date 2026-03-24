# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Iinclude -g -std=c++17

# Folders
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Source files (include main.cpp in root)
SRCS = main.cpp $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(notdir $(SRCS)))

# Executable
TARGET = $(BUILD_DIR)/my_program

all: $(BUILD_DIR) $(TARGET)

# Build executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile .cpp -> .o
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Make sure build dir exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)

.PHONY: all clean