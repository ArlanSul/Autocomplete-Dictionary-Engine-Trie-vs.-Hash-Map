#pragma once
#include "Dictionary.h"
#include <array>

class Trie : public Dictionary {
    struct Node {
        std::array<Node*, 26> children{};   // all nullptr
        bool isEnd = false;
    };
    Node* root;
    std::size_t nodeCount = 1;              // root

    void collect(Node* n, std::string& cur, std::vector<std::string>& out) const;
    static void destroy(Node* n);

public:
    Trie();
    ~Trie() override;                        // free all nodes (post-order delete)

    // Nodes are raw pointers, so a copy would double-free.
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    void insert(const std::string& word) override;
    bool contains(const std::string& word) const override;
    std::vector<std::string> autocomplete(const std::string& prefix) const override;
    std::size_t estimatedMemoryBytes() const override;
    std::string name() const override { return "Trie"; }
};
