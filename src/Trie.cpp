#include "Trie.h"

Trie::Trie() : root(new Node()) {}

Trie::~Trie() {
    destroy(root);
}

// Post-order: free children before the node itself.
void Trie::destroy(Node* n) {
    if (!n) return;
    for (Node* child : n->children) destroy(child);
    delete n;
}

void Trie::insert(const std::string& word) {
    // Words should arrive normalized (a-z only). Reject anything else
    // before creating nodes, so a bad word leaves no partial path behind.
    if (word.empty()) return;
    for (char c : word) {
        if (c < 'a' || c > 'z') return;
    }

    Node* cur = root;
    for (char c : word) {
        Node*& next = cur->children[c - 'a'];
        if (!next) {
            next = new Node();
            ++nodeCount;
        }
        cur = next;
    }
    cur->isEnd = true;
}

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
