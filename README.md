# EMR - Cyber Division

## Team Members
- Yiwen Chen
- Joshua Wilson
- Benjamin Mason

## How to Run
From the repository, build and run the UI with the following commands line by line:

Mac:
```text
cmake -S . -B build
cmake --build build --target emergency_ui
./build/emergency_ui
```

Windows:
```text
cmake -S . -B build
cmake --build build --target emergency_ui
.\build\emergency_ui.exe
```

## Overview
Emergency management organizations often receive large volumes of reports from many different sources at different times. These reports must be processed and prioritized quickly so that the most urgent cases are handled first.

This project builds a triage system for emergency reports and compares two custom priority-queue data structures implemented from scratch:
- **Binary Max-Heap**
- **Binomial Max-Heap**

Both approaches solve the same problem: organizing incoming emergency reports in the correct priority order as efficiently as possible.

## Problem
We are solving a data organization and prioritization problem in the context of disaster relief and emergency management.

Organizations in this field receive multiple streams of emergency information from many sources. Consolidating this data in a time-effective way is critical because delays can directly affect human survival.

## Motivation
If emergency data is not processed quickly and efficiently, first responders may miss urgent rescue requests.

A system that can sort and prioritize the most critical updates first can reduce delays, reduce errors, improve response speed, and potentially save more lives.

## Project Goals
We will consider the problem solved when the system can successfully:
- process and organize at least 100,000 emergency reports,
- prioritize reports based on urgency,
- compare the performance of two different algorithms/data structures,
- and identify which implementation is more efficient.

## Dataset
We will generate a synthetic dataset of **100,000 emergency reports** from multiple sources.

Each report includes at least the following fields:

| Column Name | Data Type | Description |
|------------|-----------|-------------|
| `report_id` | `int` | Unique report identifier |
| `source_id` | `int` | Identifier for the source of the report |
| `timestamp` | `int64` | Time the report was created/received |
| `severity` | `int` | Severity score from 1 to 10 |
| `category` | `string` | Type of emergency/report |

Priority rule used by both heaps: higher `severity` first, and if tied, earlier `timestamp` first.

## Tools and Technologies
- **Language:** C++ (C++17)
- **Build System:** CMake
- **Collaboration / Version Control:** GitHub

## System Interface
The project uses a terminal-based and menu-driven interface for demonstration. The interface supports dataset generation/cleaning, running each heap-based triage, benchmarking both implementations, and exporting benchmark results as csv file.
### Example Terminal Layout
```text
==================== Emergency Data System ====================

Generated 100000 emergency reports

------------------------------ MENU ---------------------------
Dataset: READY | reports=100000 | sources=50 | seed=42 | topK=5
1) Generate + Clean dataset
2) Quick data summary
3) Run Binary Heap triage (print top K)
4) Run Binomial Heap triage (print top K)
5) Benchmark both + correctness + export CSV
6) Settings (N, sources, seed, topK)
0) Exit
--------------------------------------------------------------
Enter choice: 5

------------------------- BENCHMARK ---------------------------
Binary total:   102 ms (insert 40 ms, extract 62 ms)
Binomial total: 31 ms (insert 12 ms, extract 19 ms)
Correctness: PASS (order check + top100 match)
Exported benchmark to results/benchmark.csv
```
