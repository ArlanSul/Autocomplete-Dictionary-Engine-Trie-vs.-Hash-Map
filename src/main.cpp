#include "Benchmark.h"
#include "HashMapDict.h"
#include "Loader.h"
#include "Trie.h"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void printResults(const std::vector<std::string>& results, double elapsedUs) {
    std::cout << "Matches: " << results.size()
              << " (search took " << std::fixed << std::setprecision(3)
              << elapsedUs << " us)\n";
    const std::size_t displayCount = std::min<std::size_t>(results.size(), 10);
    for (std::size_t i = 0; i < displayCount; ++i) {
        std::cout << "  " << results[i] << '\n';
    }
    if (results.size() > displayCount) {
        std::cout << "  ... and " << results.size() - displayCount << " more\n";
    }
}

void search(Dictionary& dict) {
    std::string prefix;
    std::cout << "Enter a prefix: ";
    std::cin >> prefix;
    const auto start = std::chrono::steady_clock::now();
    const std::vector<std::string> results = dict.autocomplete(prefix);
    const auto end = std::chrono::steady_clock::now();
    const double elapsedUs =
        std::chrono::duration<double, std::micro>(end - start).count();
    std::cout << dict.name() << " search for \"" << prefix << "\"\n";
    printResults(results, elapsedUs);
}

void compare(Trie& trie, HashMapDict& hashMap) {
    std::string prefix;
    std::cout << "Enter a prefix to compare: ";
    std::cin >> prefix;
    const auto trieResults = trie.autocomplete(prefix);
    const auto hashResults = hashMap.autocomplete(prefix);
    std::cout << "Trie matches: " << trieResults.size() << '\n';
    std::cout << "HashMap matches: " << hashResults.size() << '\n';
    if (trieResults == hashResults) {
        std::cout << "PASS: both structures returned identical sorted results.\n";
    } else {
        std::cout << "FAIL: the structures returned different results.\n";
    }
}

void exactLookup(Trie& trie, HashMapDict& hashMap) {
    std::string word;
    std::cout << "Enter an exact word: ";
    std::cin >> word;
    std::cout << "Trie: " << (trie.contains(word) ? "FOUND" : "NOT FOUND") << '\n';
    std::cout << "HashMap: " << (hashMap.contains(word) ? "FOUND" : "NOT FOUND") << '\n';
}

void writeAndPrintBenchmark() {
    Trie trie;
    HashMapDict hashMap;
    std::vector<BenchmarkRow> rows = runBenchmark(trie);
    const std::vector<BenchmarkRow> hashRows = runBenchmark(hashMap);
    rows.insert(rows.end(), hashRows.begin(), hashRows.end());
    writeBenchmarkCsv(rows);
    std::cout << "Wrote " << rows.size()
              << " benchmark rows to results/benchmarks.csv\n";
}

int runMenu() {
    Trie trie;
    HashMapDict hashMap;
    const std::size_t trieWords = loadDictionary(trie);
    const std::size_t hashWords = loadDictionary(hashMap);
    std::cout << "Loaded " << trieWords << " words into Trie and "
              << hashWords << " words into HashMap.\n";

    while (true) {
        std::cout << "\n1. Trie search\n"
                  << "2. HashMap search\n"
                  << "3. Compare Trie and HashMap\n"
                  << "4. Exact word lookup\n"
                  << "5. Exit\n"
                  << "Select an option: ";
        int choice = 0;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid selection.\n";
            continue;
        }
        if (choice == 1) {
            search(trie);
        } else if (choice == 2) {
            search(hashMap);
        } else if (choice == 3) {
            compare(trie, hashMap);
        } else if (choice == 4) {
            exactLookup(trie, hashMap);
        } else if (choice == 5) {
            return 0;
        } else {
            std::cout << "Invalid selection.\n";
        }
    }
}

} // namespace

int main(int argc, char* argv[]) {
    try {
        if (argc == 2 && std::string(argv[1]) == "--benchmark") {
            writeAndPrintBenchmark();
            return 0;
        }
        if (argc != 1) {
            std::cout << "Usage: " << argv[0] << " [--benchmark]\n";
            return 2;
        }
        return runMenu();
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
