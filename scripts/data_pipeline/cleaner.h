#pragma once
#include <string>
using namespace std;

bool cleanDatasetCSV(const string& inPath,
                     const string& outPath,
                     int nSources,
                     long long& kept,
                     long long& dropped);
