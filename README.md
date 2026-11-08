README

# Autocomplete Dictionary Engine - Trie vs. Hash Map

## Project Overview

This project implements and compares two autocomplete dictionary engines:

1. A **Trie-based dictionary**
2. A **custom Hash Map with a capped prefix index** (up to 5 characters)

Both data structures are tested on their ability to insert words, perform exact lookups, and autocomplete prefixes using the `dwyl/english-words` dataset (over 100,000 unique English words). The project measures and compares search time, construction time, and memory usage for both implementations.

## Team Members

- **Jordan Serna** (jordansernaa) - Custom Hash Map implementation, performance graphs, test correctness.
- **Chelsea Zhao** (chelsea-zhao-1) - Trie-based autocomplete implementation.
- **Arlan Suleimanov** (ArlanSul) - Dataset loading, normalization, and testing framework.

## Repository Structure

- `src/`: Contains all C++ source files (`Trie.cpp`, `HashMapDict.cpp`, `Loader.cpp`, `Benchmark.cpp`, etc.) and headers.
- `scripts/`: Contains Python scripts (e.g., `plot.py`) used to generate performance graphs from benchmark data.
- `results/`: Directory where benchmark CSV exports and generated graphs are saved.
- `data/`: Directory for the dictionary text file (e.g., `words_alpha.txt`).

## Prerequisites

- **C++ Compiler**: A compiler that supports C++11 or higher (e.g., `g++`, `clang++`).
- **Python 3**: Required to run the visualization scripts.
- **Matplotlib**: Python library for generating graphs. Install via `pip install matplotlib`.

## Instructions for Running the Code

### 1. Build the C++ Application

Open your terminal, navigate to the root directory of the repository, and compile the source code using your C++ compiler. For example, using `g++`:

```bash
g++ -std=c++11 src/*.cpp -o autocomplete_engine
```

### 2. Run the Benchmarks and Tests

Execute the compiled program. Ensure that the dataset file (e.g., `words_alpha.txt`) is located in the expected path (usually the `data/` folder, as defined in the source code).

```bash
./autocomplete_engine
```

The program will load the dataset, normalize the words, insert them into both data structures, and run the automated tests and benchmarks.
The benchmark results (search times, build times, memory usage) will be exported to a CSV file located at `results/benchmarks.csv`.

### 3. Generate Performance Graphs

After the C++ program finishes and generates the `benchmarks.csv` file, run the Python script to visualize the data:

```bash
python3 scripts/plot.py
```

This script will read the CSV data and generate Matplotlib graphs comparing the performance of the Trie and the Hash Map across different prefix lengths and match counts. The graphs will be saved in the `results/` directory.