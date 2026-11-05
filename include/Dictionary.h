#pragma once
#include <string>
#include <vector>
#include <cstddef>


class Dictionary {
public:
    virtual ~Dictionary() = default;
    virtual void insert(const std::string& word) = 0;
    virtual bool contains(const std::string& word) const = 0;
    virtual std::vector<std::string> autocomplete(const std::string& prefix) const = 0;
    virtual std::size_t estimatedMemoryBytes() const = 0;
    virtual std::string name() const = 0;   // "Trie" or "HashMap"
};
