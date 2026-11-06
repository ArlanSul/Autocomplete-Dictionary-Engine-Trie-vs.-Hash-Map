#include "Loader.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>

std::size_t loadDictionary(Dictionary& dict, const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath);
    }

    std::string word;
    std::size_t count = 0;

    while (file >> word) {
        std::string cleaned;
        for (char ch : word) {
            if (std::isalpha(static_cast<unsigned char>(ch))) {
                cleaned += static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
            }
        }
        if (!cleaned.empty()) {
            dict.insert(cleaned);
            ++count;
        }
    }

    return count;
}