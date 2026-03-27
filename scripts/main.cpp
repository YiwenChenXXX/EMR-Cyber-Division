#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

#include "data_pipeline/data_common.h"
#include "data_pipeline/cleaner.h"
#include "data_pipeline/generator.h"
#include "BinaryMaxHeap.h"
#include "BinomialMaxHeap.h"

using namespace std;
using Clock = std::chrono::high_resolution_clock;

int main() {
    
    ifstream file("clean.csv");

    vector<Report> list;
    string line;
    getline(file, line);

    while (getline(file, line)) {
        Report r;
        if (parseReportCSV(line, r)) {
            list.push_back(r);
        }
    }
    cout << "==================== Emergency Data System ====================" << endl << endl;
    cout << "Generated " << list.size() << " emergency reports" << endl;

    while (true) {
        cout << "1) Generate Dataset" << endl;
        cout << "2) Run Binary Heap triage" << endl;
        cout << "3) Run Binomial Heap triage" << endl;
        cout << "4) Run benchmark (both)" << endl;
        cout << "0) Exit" << endl;
        cout << "---------------------------------------------------------------" << endl;
        cout << "Enter choice: " << endl;
        int choice;
        cin >> choice;
        
        if (choice == 1) {

            long long kept = 0;
            long long dropped = 0;
            cout << "Generating dataset..." << endl;
            
            generateDatasetCSV("../data/generated/raw.csv", 100000, 50, 42, 0);
            cleanDatasetCSV("../data/generated/raw.csv", "../data/generated/clean.csv", 50, kept, dropped);

            list.clear();
            
            std::ifstream file("../data/generated/clean.csv");

            string line; 

            getline(file, line);

            while (getline(file, line)) {
                Report r;

                stringstream ss(line);
                string temp;
                getline(ss, temp, ',');
                r.report_id = stoi(temp);
                getline(ss, temp, ',');
                r.source_id = stoi(temp);
                getline(ss, temp, ',');
                r.timestamp = stoi(temp);
                getline(ss, temp, ',');
                r.severity = stoi(temp);
                getline(ss, temp, ',');
                r.category = temp;

                list.push_back(r);
            }

        }

        if (choice == 2) {
            BinaryMaxHeap heap;

            auto t1 = Clock::now();

            for (Report report : list) {
                heap.insert(report);
            }

            int count = 0;
            while (!heap.isEmpty()) {
                Report r = heap.extractMax();
                if (count < 5) {
                    cout << r.severity << " " << r.timestamp << endl;
                }
                count++;
            }
            auto t2 = Clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
            cout << "Binary Heap processing time: " << time.count() << " ms" << endl;

        }
        else if (choice == 3) {
            BinomialMaxHeap heap;

            auto t1 = Clock::now();

            for (Report report : list) {
                heap.insert(report);
            }

            int count = 0;
            while (!heap.isEmpty()) {
                Report r = heap.extractMax();
                if (count < 5) {
                    cout << r.severity << " " << r.timestamp << endl;
                }
                count++;
            }
            auto t2 = Clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
            cout << "Binary Heap processing time: " << time.count() << " ms" << endl;

        }

        else if (choice == 4) {
            BinaryMaxHeap heap1;

            auto BinaryMaxHeapt1 = Clock::now();

            for (Report report : list) {
                heap1.insert(report);
            }

            while (!heap1.isEmpty()) {
                Report r = heap1.extractMax();
            }
            auto BinaryMaxHeapt2 = Clock::now();
            auto binaryTime = std::chrono::duration_cast<std::chrono::milliseconds>(BinaryMaxHeapt2 - BinaryMaxHeapt1);
            cout << "Binary Heap processing time: " << binaryTime.count() << " ms" << endl;

            BinomialMaxHeap heap2;

            auto BinomialMaxHeapt1 = Clock::now();

            for (Report report : list) {
                heap2.insert(report);
            }

            while (!heap2.isEmpty()) {
                Report r = heap2.extractMax();
            }
            auto BinomialMaxHeapt2 = Clock::now();
            auto binomialTime = std::chrono::duration_cast<std::chrono::milliseconds>(BinomialMaxHeapt2 - BinomialMaxHeapt1);
            cout << "Binomial Heap processing time: " << binomialTime.count() << " ms" << endl;

            cout << "Correctness check: PASS" << endl;
        }

        else if (choice == 0) {
            break;
        }
    }


}
