#include "../include/HashMapDict.h"
#include <algorithm>
#include <stdexcept>

HashMapDict::HashMapDict(size_t bucketsCount) : numBuckets(bucketsCount), numItems(0) {
    if (numBuckets == 0) {
        throw std::invalid_argument("HashMapDict requires at least one bucket");
    }
    buckets.resize(numBuckets);
}

// Polynomial rolling hash: h = h*31 + c, then % numBuckets
size_t HashMapDict::hashFunction(const std::string& word) const {
    unsigned long long h = 0;
    for (char c : word) {
        h = h * 31 + c;
    }
    return h % numBuckets;
}

bool HashMapDict::isValidInput(const std::string& value) {
    for (char c : value) {
        if (c < 'a' || c > 'z') {
            return false;
        }
    }
    return true;
}

void HashMapDict::insert(const std::string& word) {
    if (word.empty() || !isValidInput(word)) {
        return;
    }

    // 1. Insert into main hash table (Separate Chaining)
    size_t hashIdx = hashFunction(word);

    // Check for duplicates before inserting
    for (const std::string& existingWord : buckets[hashIdx]) {
        if (existingWord == word) return;
    }

    buckets[hashIdx].push_back(word);
    numItems++;

    // 2. Update the capped prefix index (keys of length 1..K
    const std::size_t maxPrefixLen = std::min(K, word.length());
    for (std::size_t i = 1; i <= maxPrefixLen; ++i) {
        std::string prefix = word.substr(0, i);
        prefixIndex[prefix].push_back(word);
    }
}

bool HashMapDict::contains(const std::string& word) const {
    if (word.empty() || !isValidInput(word)) {
        return false;
    }

    size_t hashIdx = hashFunction(word);

    for (const std::string& w : buckets[hashIdx]) {
        if (w == word) {
            return true;
        }
    }
    return false;
}

std::vector<std::string> HashMapDict::autocomplete(const std::string& prefix) const {
    std::vector<std::string> results;

    if (!isValidInput(prefix)) {
        return {};
    }

    if (prefix.empty()) {
        for (const auto& chain : buckets) {
            results.insert(results.end(), chain.begin(), chain.end());
        }
    } else if (prefix.length() <= K) {
        // Direct lookup in the prefix index
        auto it = prefixIndex.find(prefix);
        if (it != prefixIndex.end()) {
            results = it->second;
        }
    }
    else {
        // Prefix longer than K: look up length-K bucket and linear-filter down
        std::string cappedPrefix = prefix.substr(0, K);
        auto it = prefixIndex.find(cappedPrefix);
        if (it != prefixIndex.end()) {
            for (const std::string& word : it->second) {
                // Check if the word actually starts with the full prefix
                if (word.length() >= prefix.length() &&
                    word.compare(0, prefix.length(), prefix) == 0) {
                    results.push_back(word);
                }
            }
        }
    }

    // must match the Trie exactly: ensure the output is sorted alphabetically
    std::sort(results.begin(), results.end());

    // Remove duplicates just in case (though insertion should prevent them)
    results.erase(std::unique(results.begin(), results.end()), results.end());

    return results;
}

size_t HashMapDict::estimatedMemoryBytes() const {
    size_t totalBytes = sizeof(*this);

    // Main buckets memory
    totalBytes += buckets.capacity() * sizeof(std::vector<std::string>);
    for (const auto& chain : buckets) {
        totalBytes += chain.capacity() * sizeof(std::string);
        for (const std::string& w : chain) {
            totalBytes += w.capacity();
        }
    }
    // Prefix index memory
    for (const auto& pair : prefixIndex) {
        totalBytes += 32 + pair.first.capacity();
        totalBytes += pair.second.capacity() * sizeof(std::string);
        for (const std::string& w : pair.second) {
            totalBytes += w.capacity();
        }
    }

    return totalBytes;
}