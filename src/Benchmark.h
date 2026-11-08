#pragma once

#include "Dictionary.h"
#include <cstddef>
#include <string>
#include <vector>

struct BenchmarkRow {
    std::string structure;
    std::string prefix;
    std::size_t matches{0};
    double searchTimeUs{0.0};
    double buildTimeMs{0.0};
    std::size_t estimatedMemoryBytes{0};
};

std::vector<BenchmarkRow> runBenchmark(
    Dictionary& dict,
    const std::string& datasetPath = "data/words.txt");

void writeBenchmarkCsv(
    const std::vector<BenchmarkRow>& results,
    const std::string& outputPath = "results/benchmarks.csv");
