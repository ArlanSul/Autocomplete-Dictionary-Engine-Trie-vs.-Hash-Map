#ifndef HASHMAPDICT_H
#define HASHMAPDICT_H

#include "Dictionary.h"
#include <string>
#include <vector>
#include <map>
#include <cstddef>

class HashMapDict : public Dictionary {
private:
    size_t numBuckets;
    size_t numItems;
    static constexpr std::size_t K = 5;
    std::vector<std::vector<std::string>> buckets;


    std::map<std::string, std::vector<std::string>> prefixIndex;

    // The polynomial rolling hash function
    size_t hashFunction(const std::string& word) const;
    static bool isValidInput(const std::string& value);

public:
    HashMapDict(size_t bucketsCount = 100003); // Use a prime number for buckets
    ~HashMapDict() override = default;

    void insert(const std::string& word) override;
    bool contains(const std::string& word) const override;
    std::vector<std::string> autocomplete(const std::string& prefix) const override;
    size_t estimatedMemoryBytes() const override;
    std::string name() const override { return "HashMap"; }
};

#endif // HASHMAPDICT_H