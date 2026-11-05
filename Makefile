CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Iinclude

SRC = src/main.cpp src/Trie.cpp src/HashMap.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = dictionary_bench

all: $(TARGET)

$(TARGET): $(OBJ)
    $(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    rm -f src/*.o $(TARGET)
