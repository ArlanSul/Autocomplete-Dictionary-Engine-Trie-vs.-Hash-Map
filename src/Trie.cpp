#include "Trie.h"

Trie::Trie() : root(new Node()) {}

Trie::~Trie() {}

void Trie::insert(const std::string& /*word*/) {}

bool Trie::contains(const std::string& /*word*/) const {
    return false;
}

std::vector<std::string> Trie::autocomplete(const std::string& /*prefix*/) const {
    return {};
}

std::size_t Trie::estimatedMemoryBytes() const {
    return 0;
}

void Trie::collect(Node* /*n*/, std::string& /*cur*/, std::vector<std::string>& /*out*/) const {}
