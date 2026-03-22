#include "generator.h"
#include "data_common.h"
#include <fstream>
#include <random>
#include <vector>
#include <iostream>
using namespace std;

bool generateDatasetCSV(const string& outPath,
                        int nReports,
                        int nSources,
                        unsigned int seed,
                        long long startTs) {
  if (nReports <= 0 || nSources <= 0){
    return false;
  }
  ofstream out(outPath);
  if (!out){
    return false;
  }
  mt19937 rng(seed);
  uniform_int_distribution<int> srcDist(0, nSources - 1);
  vector<string> cats = {"medical","fire","flood","rescue","power"};
  uniform_int_distribution<int> catDist(0, (int)cats.size() - 1);
  vector<int> sevW(10);
  for (int s = 1; s <= 10; s++){
    sevW[s - 1] = 11 - s;
  }
  discrete_distribution<int> sevDist(sevW.begin(), sevW.end());
  exponential_distribution<double> gap(1.0 / 2.0);
  double t = 0.0;
  out << csvHeader();
  for (int i = 0; i < nReports; i++) {
    Report r;
    r.report_id = i + 1;
    r.source_id = srcDist(rng);
    r.severity  = sevDist(rng) + 1;
    r.category  = cats[catDist(rng)];
    t += gap(rng);
    r.timestamp = startTs + (long long)t;
    out << toCSVRow(r);
  }
  return true;
}


