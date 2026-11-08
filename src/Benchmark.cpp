#include "Benchmark.h"
#include "Loader.h"
#include <chrono>
#include <fstream>
#include <stdexcept>
#include <vector>

std::vector<BenchmarkRow> runBenchmark(
    Dictionary& dict,
    const std::string& datasetPath) {
    const auto buildStart = std::chrono::steady_clock::now();
    const std::size_t wordCount = loadDictionary(dict, datasetPath);
    const auto buildEnd = std::chrono::steady_clock::now();
    const double buildTimeMs =
        std::chrono::duration<double, std::milli>(buildEnd - buildStart).count();
    const std::size_t memoryBytes = dict.estimatedMemoryBytes();

    const std::vector<std::string> prefixes = {
        "a", "app", "band", "cat", "progr", "comput",
        "diction", "struc", "zzzz"
    };
    constexpr int iterations = 100;
    std::vector<BenchmarkRow> results;
    results.reserve(prefixes.size());

    for (const std::string& prefix : prefixes) {
        const std::vector<std::string> matches = dict.autocomplete(prefix);
        const auto searchStart = std::chrono::steady_clock::now();
        for (int i = 0; i < iterations; ++i) {
            volatile std::size_t resultCount = dict.autocomplete(prefix).size();
            (void)resultCount;
        }
        const auto searchEnd = std::chrono::steady_clock::now();
        const double totalMicroseconds =
            std::chrono::duration<double, std::micro>(searchEnd - searchStart).count();

        results.push_back({
            dict.name(),
            prefix,
            matches.size(),
            totalMicroseconds / iterations,
            buildTimeMs,
            memoryBytes
        });
    }

    if (wordCount == 0) {
        throw std::runtime_error("The dictionary dataset contains no words");
    }
    return results;
}

void writeBenchmarkCsv(
    const std::vector<BenchmarkRow>& results,
    const std::string& outputPath) {
    std::ofstream out(outputPath);
    if (!out.is_open()) {
        throw std::runtime_error("Could not open benchmark output: " + outputPath);
    }

    out << "Structure,Prefix,Matches,SearchTimeUs,BuildTimeMs,EstimatedMemoryBytes\n";
    for (const BenchmarkRow& row : results) {
        out << row.structure << ','
            << row.prefix << ','
            << row.matches << ','
            << row.searchTimeUs << ','
            << row.buildTimeMs << ','
            << row.estimatedMemoryBytes << '\n';
    }
}
