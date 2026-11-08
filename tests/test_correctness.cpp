#include <iostream>
#include <vector>
#include <string>
#include <cassert>


#include "../include/Trie.h"
#include "../include/HashMapDict.h"

int main() {
    std::cout << "Initializing Correctness Test...\n";

    Trie trie;
    HashMapDict hashMap;

    // A small controlled dataset to test various cases (short/long words, overlaps)
    std::vector<std::string> testDictionary = {
        "apple", "app", "application", "apt", "aptitude",
        "banana", "band", "bandana", "bandit",
        "cat", "caterpillar", "cart",
        "dog", "dodge", "zebra"
    };

    std::cout << "Populating data structures...\n";
    for (const std::string& word : testDictionary) {
        trie.insert(word);
        hashMap.insert(word);
    }

    
    std::vector<std::string> testPrefixes = {
        "a",           // Short prefix, many matches
        "app",         // Exact word that is also a prefix
        "band",        // Exact word and prefix
        "cat",         // Exact word
        "caterpillar", // Long exact word (longer than K=5)
        "cater",       // Prefix longer than K=5 to trigger the linear-filter
        "z",           // Few matches
        "zzzz"         // No match, explicitly requested in the doc
    };

    std::cout << "Testing autocomplete outputs...\n";
    
    for (const std::string& prefix : testPrefixes) {
        std::vector<std::string> trieResults = trie.autocomplete(prefix);
        std::vector<std::string> hashResults = hashMap.autocomplete(prefix);

        // this asserts that both the contents AND the alphabetical sorting are perfectly identical
        if (trieResults != hashResults) {
            std::cerr << "Mismatch found for prefix: '" << prefix << "'\n";
            std::cerr << "Trie returned " << trieResults.size() 
                      << " items, Hash returned " << hashResults.size() << " items.\n";
            
            // Trigger the assertion failure
            assert(trieResults == hashResults && "Autocomplete results do not match!"); 
        }
        
        std::cout << " [PASS] Prefix: '" << prefix << "' (" << trieResults.size() << " matches)\n";
    }

    std::cout << "All correctness tests passed! Trie and Hash Map outputs match exactly.\n";
    return 0;
}