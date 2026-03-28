#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <string>
#include <limits>
#include <cstdlib>

#include "data_pipeline/data_common.h"
#include "data_pipeline/cleaner.h"
#include "data_pipeline/generator.h"
#include "BinaryMaxHeap.h"
#include "BinomialMaxHeap.h"

using namespace std;
using namespace std::chrono;
using Clock = high_resolution_clock;

struct RunStats {
    long long insert_ms = 0;
    long long extract_ms = 0;
    long long total_ms = 0;
    double insert_ops_per_sec = 0.0;
    double extract_ops_per_sec = 0.0;
};

static bool higherPriority(const Report& a, const Report& b) {
    if (a.severity != b.severity) {
        return a.severity > b.severity;
    }
    return a.timestamp < b.timestamp;
}

static bool isValidTriageOrder(const Report& prev, const Report& cur) {
    return !higherPriority(cur, prev);
}

static bool loadCleanCSV(const string& path, vector<Report>& list) {
    list.clear();

    ifstream file(path);
    if (!file) {
        return false;
    }

    string line;
    if (!getline(file, line)) {
        return false;
    }

    while (getline(file, line)) {
        Report r;
        if (parseReportCSV(line, r)) {
            list.push_back(r);
        }
    }

    if (list.empty()) {
        return false;
    }
    return true;
}

static void ensureDatasetReadyYN(vector<Report>& list,
                                const string& rawPath,
                                const string& cleanPath,
                                int nReports,
                                int nSources,
                                unsigned int seed,
                                long long startTs) {
    if (loadCleanCSV(cleanPath, list)) {
        return;
    }

    cout << "Dataset not found. Generate " << nReports << " reports now? (y/n): ";
    char ans;
    cin >> ans;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (ans != 'y' && ans != 'Y') {
        cout << "Skipping generation. You can generate later from the menu.\n";
        return;
    }

    system("mkdir -p data/generated");

    long long kept = 0;
    long long dropped = 0;
    cout << "Generating dataset..." << endl;

    bool ok1 = generateDatasetCSV(rawPath, nReports, nSources, seed, startTs);
    bool ok2 = cleanDatasetCSV(rawPath, cleanPath, nSources, kept, dropped);

    if (!ok1 || !ok2) {
        cout << "Failed to generate/clean dataset.\n";
        return;
    }

    loadCleanCSV(cleanPath, list);
    cout << "Generated " << list.size() << " emergency reports" << endl;
}

static void printDataSummary(const vector<Report>& list, int nSources) {
    if (list.empty()) {
        cout << "No dataset loaded.\n";
        return;
    }

    vector<long long> sevCount(11, 0);
    long long medical = 0, fire = 0, flood = 0, rescue = 0, power = 0;

    vector<long long> sourceCount;
    if (nSources > 0) {
        sourceCount.assign(nSources, 0);
    }

    for (const auto& r : list) {
        if (r.severity >= 1 && r.severity <= 10) {
            sevCount[r.severity]++;
        }

        if (r.category == "medical") {
            medical++;
        } 
        else if (r.category == "fire") {
            fire++;
        } 
        else if (r.category == "flood") {
            flood++;
        } 
        else if (r.category == "rescue") {
            rescue++;
        } 
        else if (r.category == "power") {
            power++;
        }

        if (nSources > 0 && r.source_id >= 0 && r.source_id < nSources) {
            sourceCount[r.source_id]++;
        }
    }

    cout << "------------------------- DATA SUMMARY ------------------------\n";
    cout << "Reports: " << list.size() << "\n";
    cout << "Severity counts (1..10):\n";
    for (int s = 1; s <= 10; s++) {
        cout << "  " << s << ": " << sevCount[s] << "\n";
    }
    cout << "Category counts:\n";
    cout << "  medical: " << medical << "\n";
    cout << "  fire:    " << fire << "\n";
    cout << "  flood:   " << flood << "\n";
    cout << "  rescue:  " << rescue << "\n";
    cout << "  power:   " << power << "\n";

    if (!sourceCount.empty()) {
        vector<pair<long long,int>> tmp;
        tmp.reserve(sourceCount.size());
        for (int i = 0; i < (int)sourceCount.size(); i++) {
            tmp.push_back({sourceCount[i], i});
        }
        sort(tmp.begin(), tmp.end(), [](auto& a, auto& b){ return a.first > b.first; });

        cout << "Top 5 sources by count:\n";
        for (int i = 0; i < 5 && i < (int)tmp.size(); i++) {
            cout << "  source " << tmp[i].second << ": " << tmp[i].first << "\n";
        }
    }
    cout << "--------------------------------------------------------------\n";
}

static RunStats runBinary(const vector<Report>& list, int printTopK, bool& orderOk, vector<Report>& topOut) {
    RunStats st;
    orderOk = true;
    topOut.clear();

    BinaryMaxHeap heap;

    auto t1 = Clock::now();
    for (const auto& r : list) {
        heap.insert(r);
    }
    auto t2 = Clock::now();

    bool hasPrev = false;
    Report prev{};
    int extracted = 0;

    auto t3_start = Clock::now();
    while (!heap.isEmpty()) {
        Report cur = heap.extractMax();

        if (hasPrev) {
            if (!isValidTriageOrder(prev, cur)) {
                orderOk = false;
            }
        }

        prev = cur;
        hasPrev = true;

        if (extracted < printTopK) {
            cout << cur.severity << " " << cur.timestamp << " " << cur.report_id << " " << cur.source_id << " " << cur.category << "\n";
        }

        if ((int)topOut.size() < 100) {
            topOut.push_back(cur);
        }

        extracted++;
    }
    auto t3 = Clock::now();

    st.insert_ms = duration_cast<milliseconds>(t2 - t1).count();
    st.extract_ms = duration_cast<milliseconds>(t3 - t3_start).count();
    st.total_ms = duration_cast<milliseconds>(t3 - t1).count();

    double n = (double)list.size();
    if (st.insert_ms > 0) {
        st.insert_ops_per_sec = n / ((double)st.insert_ms / 1000.0);
    }
    if (st.extract_ms > 0) {
        st.extract_ops_per_sec = n / ((double)st.extract_ms / 1000.0);
    }

    return st;
}

static RunStats runBinomial(const vector<Report>& list, int printTopK, bool& orderOk, vector<Report>& topOut) {
    RunStats st;
    orderOk = true;
    topOut.clear();

    BinomialMaxHeap heap;

    auto t1 = Clock::now();
    for (const auto& r : list) {
        heap.insert(r);
    }
    auto t2 = Clock::now();

    bool hasPrev = false;
    Report prev{};
    int extracted = 0;

    auto t3_start = Clock::now();
    while (!heap.isEmpty()) {
        Report cur = heap.extractMax();

        if (hasPrev) {
            if (!isValidTriageOrder(prev, cur)) {
                orderOk = false;
            }
        }

        prev = cur;
        hasPrev = true;

        if (extracted < printTopK) {
            cout << cur.severity << " " << cur.timestamp << " " << cur.report_id << " " << cur.source_id << " " << cur.category << "\n";
        }

        if ((int)topOut.size() < 100) {
            topOut.push_back(cur);
        }

        extracted++;
    }
    auto t3 = Clock::now();

    st.insert_ms = duration_cast<milliseconds>(t2 - t1).count();
    st.extract_ms = duration_cast<milliseconds>(t3 - t3_start).count();
    st.total_ms = duration_cast<milliseconds>(t3 - t1).count();

    double n = (double)list.size();
    if (st.insert_ms > 0) {
        st.insert_ops_per_sec = n / ((double)st.insert_ms / 1000.0);
    }
    if (st.extract_ms > 0) {
        st.extract_ops_per_sec = n / ((double)st.extract_ms / 1000.0);
    }

    return st;
}

static bool topMatch100(const vector<Report>& a, const vector<Report>& b) {
    int m = (int)min(a.size(), b.size());
    if (m > 100) {
        m = 100;
    }

    for (int i = 0; i < m; i++) {
        if (a[i].severity != b[i].severity) {
            return false;
        }
        if (a[i].timestamp != b[i].timestamp) {
            return false;
        }
        if (a[i].report_id != b[i].report_id) {
            return false;
        }
    }
    return true;
}

static void exportBenchmarkCSV(const string& path,
                               int nReports, int nSources, unsigned int seed,
                               const RunStats& bin, const RunStats& bio,
                               bool okOrderBinary, bool okOrderBinomial, bool okTop100) {
    system("mkdir -p results");

    ofstream out(path);
    if (!out) {
        cout << "Failed to write: " << path << "\n";
        return;
    }

    out << "nReports,nSources,seed,"
        << "binary_insert_ms,binary_extract_ms,binary_total_ms,binary_insert_ops,binary_extract_ops,"
        << "binomial_insert_ms,binomial_extract_ms,binomial_total_ms,binomial_insert_ops,binomial_extract_ops,"
        << "binary_order_ok,binomial_order_ok,top100_match\n";

    out << nReports << "," << nSources << "," << seed << ","
        << bin.insert_ms << "," << bin.extract_ms << "," << bin.total_ms << ","
        << bin.insert_ops_per_sec << "," << bin.extract_ops_per_sec << ","
        << bio.insert_ms << "," << bio.extract_ms << "," << bio.total_ms << ","
        << bio.insert_ops_per_sec << "," << bio.extract_ops_per_sec << ","
        << (okOrderBinary ? 1 : 0) << "," << (okOrderBinomial ? 1 : 0) << "," << (okTop100 ? 1 : 0) << "\n";

    cout << "Exported benchmark to " << path << endl;
}

int main() {
    const string rawPath = "data/generated/raw.csv";
    const string cleanPath = "data/generated/clean.csv";

    int nReports = 100000;
    int nSources = 50;
    unsigned int seed = 42;
    long long startTs = 0;
    int printTopK = 5;

    vector<Report> list;
    ensureDatasetReadyYN(list, rawPath, cleanPath, nReports, nSources, seed, startTs);

    cout << "==================== Emergency Data System ====================" << endl << endl;
    cout << "Generated " << list.size() << " emergency reports" << endl;

    while (true) {
        cout << "\n------------------------------ MENU ---------------------------" << endl;
        cout << "Dataset: " << (list.empty() ? "NOT LOADED" : "READY") << " | reports=" << list.size() << " | sources=" << nSources << " | seed=" << seed << " | topK=" << printTopK << endl;
        cout << "1) Generate + Clean dataset" << endl;
        cout << "2) Data summary" << endl;
        cout << "3) Run Binary Heap triage (print top K)" << endl;
        cout << "4) Run Binomial Heap triage (print top K)" << endl;
        cout << "5) Benchmark both, correctness, and export CSV" << endl;
        cout << "6) Settings (N, sources, seed, topK)" << endl;
        cout << "0) Exit" << endl;
        cout << "--------------------------------------------------------------" << endl;
        cout << "Enter choice: ";
        int choice;
        cin >> choice;

        if (choice == 0) {
            break;
        }

        if (choice == 1) {
            long long kept = 0;
            long long dropped = 0;

            system("mkdir -p data/generated");
            cout << "Generating dataset..." << endl;

            bool ok1 = generateDatasetCSV(rawPath, nReports, nSources, seed, startTs);
            bool ok2 = cleanDatasetCSV(rawPath, cleanPath, nSources, kept, dropped);

            if (!ok1 || !ok2) {
                cout << "Failed to generate/clean dataset.\n";
            } 
            else {
                loadCleanCSV(cleanPath, list);
                cout << "Generated " << list.size() << " emergency reports" << endl;
            }
        }

        else if (choice == 2) {
            printDataSummary(list, nSources);
        }

        else if (choice == 3) {
            if (list.empty()) {
                cout << "No dataset loaded. Generate first.\n";
                continue;
            }

            cout << "Top " << printTopK << " processed (sev ts id source category):\n";
            bool okOrder = true;
            vector<Report> top100;

            RunStats st = runBinary(list, printTopK, okOrder, top100);

            cout << "Binary insert:  " << st.insert_ms << " ms (" << st.insert_ops_per_sec << " ops/sec)\n";
            cout << "Binary extract: " << st.extract_ms << " ms (" << st.extract_ops_per_sec << " ops/sec)\n";
            cout << "Binary total:   " << st.total_ms << " ms\n";
            cout << "Order check: " << (okOrder ? "PASS" : "FAIL") << endl;
        }

        else if (choice == 4) {
            if (list.empty()) {
                cout << "No dataset loaded. Generate first.\n";
                continue;
            }

            cout << "Top " << printTopK << " processed (sev ts id source category):\n";
            bool okOrder = true;
            vector<Report> top100;

            RunStats st = runBinomial(list, printTopK, okOrder, top100);

            cout << "Binomial insert:  " << st.insert_ms << " ms (" << st.insert_ops_per_sec << " ops/sec)\n";
            cout << "Binomial extract: " << st.extract_ms << " ms (" << st.extract_ops_per_sec << " ops/sec)\n";
            cout << "Binomial total:   " << st.total_ms << " ms\n";
            cout << "Order check: " << (okOrder ? "PASS" : "FAIL") << endl;
        }

        else if (choice == 5) {
            if (list.empty()) {
                cout << "No dataset loaded. Generate first.\n";
                continue;
            }

            cout << "Running benchmark on the same input...\n";

            bool binaryOk = true;
            bool binomialOk = true;
            vector<Report> topB;
            vector<Report> topBi;

            RunStats stB = runBinary(list, 0, binaryOk, topB);
            RunStats stBi = runBinomial(list, 0, binomialOk, topBi);

            bool topOk = topMatch100(topB, topBi);

            cout << "\n------------------------- BENCHMARK ---------------------------\n";
            cout << "Binary total:   " << stB.total_ms << " ms (insert " << stB.insert_ms << " ms, extract " << stB.extract_ms << " ms)\n";
            cout << "Binomial total: " << stBi.total_ms << " ms (insert " << stBi.insert_ms << " ms, extract " << stBi.extract_ms << " ms)\n";
            cout << "Correctness: " << ((binaryOk && binomialOk && topOk) ? "PASS" : "FAIL") << " (order check + top100 match)\n";
            cout << "--------------------------------------------------------------\n";

            exportBenchmarkCSV("results/benchmark.csv",
                               nReports, nSources, seed,
                               stB, stBi,
                               binaryOk, binomialOk, topOk);
        }

        else if (choice == 6) {
            cout << "Enter N reports (current " << nReports << "): ";
            cin >> nReports;
            cout << "Enter K sources (current " << nSources << "): ";
            cin >> nSources;
            cout << "Enter seed (current " << seed << "): ";
            cin >> seed;
            cout << "Enter topK to print (current " << printTopK << "): ";
            cin >> printTopK;

            cout << "Settings updated. Run 'Generate + Clean dataset' to apply changes." << endl;
        }
    }

    return 0;
}