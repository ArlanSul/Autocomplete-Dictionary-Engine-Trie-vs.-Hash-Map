#include "Trie.h"
#include <algorithm>

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

bool Trie::contains(const std::string& word) const {
    const Node* cur = root;
    for (char c : word) {
        if (c < 'a' || c > 'z') return false;
        cur = cur->children[c - 'a'];
        if (!cur) return false;
    }
    return cur->isEnd;
}

std::vector<std::string> Trie::autocomplete(const std::string& prefix) const {
    Node* cur = root;
    for (char c : prefix) {
        if (c < 'a' || c > 'z') return {};
        cur = cur->children[c - 'a'];
        if (!cur) return {};
    }

    std::vector<std::string> out;
    std::string word = prefix;
    collect(cur, word, out);
    std::sort(out.begin(), out.end());   // contract: sorted ascending
    return out;
}

std::size_t Trie::estimatedMemoryBytes() const {
    return 0;
}

// DFS below n; cur holds the letters on the path from the root to n.
void Trie::collect(Node* n, std::string& cur, std::vector<std::string>& out) const {
    if (n->isEnd) out.push_back(cur);
    for (int i = 0; i < 26; ++i) {
        if (!n->children[i]) continue;
        cur.push_back(static_cast<char>('a' + i));
        collect(n->children[i], cur, out);
        cur.pop_back();
    }
}
