CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = autocomplete
TEST_TARGET = correctness_test

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: correctness

correctness: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): tests/test_correctness.cpp src/Trie.cpp src/HashMapDict.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f src/*.o $(TARGET) $(TEST_TARGET)

bench: $(TARGET)
	./$(TARGET) --benchmark

.PHONY: all test clean
