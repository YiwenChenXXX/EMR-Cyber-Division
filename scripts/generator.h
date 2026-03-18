#pragma once
#include <string>
using namespace std;

bool generateDatasetCSV(const string& outPath,
                        int nReports,
                        int nSources,
                        unsigned int seed,
                        long long startTs);
