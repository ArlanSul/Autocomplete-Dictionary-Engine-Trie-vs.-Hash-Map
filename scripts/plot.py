import csv
from collections import defaultdict
from pathlib import Path

import matplotlib.pyplot as plt


def load_rows(path):
    with path.open(newline="") as file:
        return list(csv.DictReader(file))


def plot_results():
    root = Path(__file__).resolve().parents[1]
    rows = load_rows(root / "results" / "benchmarks.csv")
    by_structure = defaultdict(list)
    for row in rows:
        row["Matches"] = int(row["Matches"])
        row["SearchTimeUs"] = float(row["SearchTimeUs"])
        by_structure[row["Structure"]].append(row)

    for structure, structure_rows in by_structure.items():
        structure_rows.sort(key=lambda row: len(row["Prefix"]))
        plt.plot(
            [len(row["Prefix"]) for row in structure_rows],
            [row["SearchTimeUs"] for row in structure_rows],
            marker="o",
            label=structure,
        )
    plt.xlabel("Prefix length")
    plt.ylabel("Search time (microseconds)")
    plt.title("Autocomplete search time vs. prefix length")
    plt.legend()
    plt.tight_layout()
    plt.savefig(root / "results" / "search_time_vs_prefix_length.png")
    plt.close()

    for structure, structure_rows in by_structure.items():
        structure_rows.sort(key=lambda row: row["Matches"])
        plt.plot(
            [row["Matches"] for row in structure_rows],
            [row["SearchTimeUs"] for row in structure_rows],
            marker="o",
            label=structure,
        )
    plt.xlabel("Number of matches")
    plt.ylabel("Search time (microseconds)")
    plt.title("Autocomplete search time vs. number of matches")
    plt.legend()
    plt.tight_layout()
    plt.savefig(root / "results" / "search_time_vs_matches.png")
    plt.close()


if __name__ == "__main__":
    plot_results()
