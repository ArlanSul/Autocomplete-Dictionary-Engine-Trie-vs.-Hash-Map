#pragma once

#include <string>
#include <vector>
#include <cstddef>
#include "Dictionary.h"

struct BenchmarkMetrics {
    std::string engineName;
    std::size_t wordCount{0};
    double loadTimeMs{0.0};
    double searchHitAvgNs{0.0};
    double searchMissAvgNs{0.0};
    double autocompleteShortAvgUs{0.0}; 
    double autocompleteMedAvgUs{0.0};   
    double autocompleteLongAvgUs{0.0};  
    std::size_t memoryBytes{0};
};

BenchmarkMetrics runBenchmark(Dictionary& dict, const std::string& datasetPath = "data/words.txt");


void writeBenchmarkCsv(const std::vector<BenchmarkMetrics>& results, const std::string& outputPath = "results/benchmark_results.csv");