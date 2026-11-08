CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = autocomplete

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TARGET)
	@echo "Running tests..."
	./$(TARGET)

clean:
	rm -f src/*.o $(TARGET)

bench: $(TARGET)
	./$(TARGET) --benchmark

.PHONY: all test clean
