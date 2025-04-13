# Compiler and flags
CXX 	 := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -I./src
LDFLAGS  := -lcrypto

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN 	:= dlmscli

# List of source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

# Corresponding object files will be placed in an "obj" directory
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

.PHONY: all clean

# Default target: build the executable
all: $(BIN)

# Link the object files to produce the final executable
$(BIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile .coo files to .o files and place them in the obj/ folder
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(BIN)