CXX = g++
CXXFLAGS = -std=c++20 -g
TARGET = run
SRC = testExpress.cpp
OBJ = $(SRC:.cpp=.o)

# Default target to build the project
all: $(TARGET)
# Rule to build the target executible
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile the source file into object
$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean