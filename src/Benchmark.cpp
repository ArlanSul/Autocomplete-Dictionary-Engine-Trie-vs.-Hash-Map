#include "Benchmark.h"
#include "Loader.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <numeric>
#include <vector>
#include <string>

BenchmarkMetrics runBenchmark(Dictionary& dict, const std::string& datasetPath) {
    BenchmarkMetrics m;
    m.engineName = dict.name();

    auto startLoad = std::chrono::high_resolution_clock::now();
    m.wordCount = loadDictionary(dict, datasetPath);


    auto endLoad = std::chrono::high_resolution_clock::now();
    m.loadTimeMs = std::chrono::duration<double, std::milli>(endLoad - startLoad).count();



    std::vector<std::string> hitWords = {"apple", "banana", "programming", "structure", "system", "algorithm", "dictionary"};
    const int searchIterations = 10000;
    


    auto startHit = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < searchIterations; ++i) {
        for (const auto& w : hitWords) {
            volatile bool res = dict.contains(w);
            (void)res;
        }
    }

    auto endHit = std::chrono::high_resolution_clock::now();
    m.searchHitAvgNs = std::chrono::duration<double, std::nano>(endHit - startHit).count() / (searchIterations * hitWords.size());



    std::vector<std::string> missWords = {"xyzqwe", "aaplzz", "nonexistentword123", "randomstr987", "qqwwrtty"};
    auto startMiss = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < searchIterations; ++i) {
        for (const auto& w : missWords) {
            volatile bool res = dict.contains(w);
            (void)res;
        }
    }

    auto endMiss = std::chrono::high_resolution_clock::now();
    m.searchMissAvgNs = std::chrono::duration<double, std::nano>(endMiss - startMiss).count() / (searchIterations * missWords.size());



    const int autoIterations = 1000;
    auto benchPrefixes = [&](const std::vector<std::string>& prefixes) -> double {
        auto t0 = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < autoIterations; ++i) {
            for (const auto& p : prefixes) {
                auto res = dict.autocomplete(p);
                (void)res;
            }
        }

        auto t1 = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double, std::micro>(t1 - t0).count() / (autoIterations * prefixes.size());
    };

    m.autocompleteShortAvgUs = benchPrefixes({"a", "b", "th", "pr"});
    m.autocompleteMedAvgUs   = benchPrefixes({"app", "prog", "comp", "syst"});
    m.autocompleteLongAvgUs  = benchPrefixes({"progr", "comput", "diction", "struc"});


    
    m.memoryBytes = dict.estimatedMemoryBytes();

    return m;
}

void writeBenchmarkCsv(const std::vector<BenchmarkMetrics>& results, const std::string& outputPath) {
    std::ofstream out(outputPath);
    if (!out.is_open()) return;

    out << "Engine,WordsLoaded,LoadTimeMs,SearchHitNs,SearchMissNs,AutoShortUs,AutoMedUs,AutoLongUs,MemoryBytes,MemoryMB\n";
    for (const auto& r : results) {
        double mb = static_cast<double>(r.memoryBytes) / (1024.0 * 1024.0);
        out << r.engineName << ","
            << r.wordCount << ","
            << r.loadTimeMs << ","
            << r.searchHitAvgNs << ","
            << r.searchMissAvgNs << ","
            << r.autocompleteShortAvgUs << ","
            << r.autocompleteMedAvgUs << ","
            << r.autocompleteLongAvgUs << ","
            << r.memoryBytes << ","
            << mb << "\n";
    }
}