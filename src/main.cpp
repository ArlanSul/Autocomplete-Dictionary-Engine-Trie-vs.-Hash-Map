#include <iostream>
#include <memory>
#include <chrono>
#include <string>
#include <vector>
#include "Dictionary.h"
#include "Loader.h"

void runMenu(Dictionary& dict) {
    int choice = 0;
    while (true) {
        std::cout << "\n========================================\n";
        std::cout << " Autocomplete Engine: " << dict.name() << "\n";
        std::cout << "========================================\n";
        std::cout << "1. Search for a word (contains)\n";
        std::cout << "2. Autocomplete prefix\n";
        std::cout << "3. Show estimated memory usage\n";
        std::cout << "4. Exit\n";
        std::cout << "Select an option (1-4): ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 4) {
            std::cout << "Exiting...\n";
            break;
        }

        if (choice == 1) {
            std::string word;
            std::cout << "Enter word to search: ";
            std::cin >> word;

            auto start = std::chrono::high_resolution_clock::now();
            bool found = dict.contains(word);
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - start
            ).count();

            std::cout << "'" << word << "' " << (found ? "FOUND" : "NOT FOUND")
                      << " (took " << elapsed << " ns)\n";
        } else if (choice == 2) {
            std::string prefix;
            std::cout << "Enter prefix: ";
            std::cin >> prefix;

            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::string> results = dict.autocomplete(prefix);
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start
            ).count();

            std::cout << "Found " << results.size() << " match(es) (took " << elapsed << " µs):\n";
            std::size_t limit = std::min(results.size(), std::size_t(10));
            for (std::size_t i = 0; i < limit; ++i) {
                std::cout << "  - " << results[i] << "\n";
            }
            if (results.size() > 10) {
                std::cout << "  ... and " << (results.size() - 10) << " more\n";
            }
        } else if (choice == 3) {
            std::size_t bytes = dict.estimatedMemoryBytes();
            double mb = static_cast<double>(bytes) / (1024.0 * 1024.0);
            std::cout << "Estimated memory: " << bytes << " bytes (~" << mb << " MB)\n";
        }
    }
}

int main() {
    std::cout << "Loader & Menu initialized.\n";
    return 0;
}