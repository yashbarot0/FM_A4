# Compiler
CXX = g++

# Compiler flags (C++11 standard, warnings, optimizations)
CXXFLAGS = -std=c++11 -Wall -O2

# Target executable name
TARGET = asian_option

# Source file
SRC = asian_option.cpp

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $<

# Clean up build artifacts
clean:
	rm -f $(TARGET)

.PHONY: all clean