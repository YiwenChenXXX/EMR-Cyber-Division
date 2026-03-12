# EMR - Cyber Division

## Project Title
**Emergency Data System**

## Team Members
- Yiwen Chen
- Joshua Wilson
- Benjamin Mason

---

## Overview
Emergency management organizations often receive large volumes of reports from many different sources at different times. These reports must be processed and prioritized quickly so that the most urgent cases are handled first.

This project builds a triage system for emergency reports and compares two custom priority-queue data structures implemented from scratch:

- **Binary Max-Heap**
- **Binomial Max-Heap**

Both approaches solve the same problem: organizing incoming emergency reports in the correct priority order as efficiently as possible.

---

## Problem
We are solving a data organization and prioritization problem in the context of disaster relief and emergency management.

Organizations in this field receive multiple streams of emergency information from many sources. Consolidating this data in a time-effective way is critical because delays can directly affect human survival.

---

## Motivation
If emergency data is not processed quickly and efficiently, first responders may miss urgent rescue requests.

A system that can sort and prioritize the most critical updates first can:
- reduce delays,
- reduce errors,
- improve response speed,
- and potentially save more lives.

---

## Project Goals
We will consider the problem solved when the system can successfully:

- process and organize **at least 100,000 emergency reports**,
- prioritize reports based on urgency,
- compare the performance of **two different algorithms/data structures**,
- and identify which implementation is more efficient.

---

## Dataset
We will generate a **synthetic dataset** of **100,000 emergency reports** from **25 to 100 sources**.

Each report includes at least the following fields:

| Column Name | Data Type | Description |
|------------|-----------|-------------|
| `report_id` | `int` | Unique report identifier |
| `source_id` | `int` | Identifier for the source of the report |
| `timestamp` | `int64` | Time the report was created/received |
| `severity` | `int` | Severity score from 1 to 10 |
| `category` | `string` | Type of emergency/report |

---

## Tools and Technologies
- **Language:** C++
- **Collaboration / Version Control:** GitHub

---

## System Interface
The project uses a **terminal-based, menu-driven interface** for demonstration.

The interface will display:
- the number of generated or loaded emergency reports,
- the selected data structure,
- processing results,
- total runtime in milliseconds,
- and basic correctness checks.

### Example Terminal Layout
```text
==================== Emergency Data System ====================

Generated 100000 emergency reports (50 sources)

-------------------------- MENU -------------------------------
1) Generate dataset
2) Run Binary Heap triage
3) Run Binomial Heap triage
4) Run benchmark (both)
0) Exit
---------------------------------------------------------------
Enter choice: 4

Binary Heap processing time: 1070 ms
Binomial Heap processing time: 1021 ms
Correctness check: PASS
