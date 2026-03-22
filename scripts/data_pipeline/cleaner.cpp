#include "cleaner.h"
#include "data_common.h"
#include <fstream>
#include <unordered_set>
using namespace std;

bool cleanDatasetCSV(const string& inPath,
                     const string& outPath,
                     int nSources,
                     long long& kept,
                     long long& dropped) {
    kept = 0; dropped = 0;
    ifstream in(inPath);
    if (!in){
      return false;
    }
    ofstream out(outPath);
    if (!out){
      return false;
    }
    string line;
    if (!getline(in, line)){
      return false;
    }
    out << csvHeader();
    unordered_set<int> seen;
    seen.reserve(200000);
    while (getline(in, line)) {
        if (line.empty()){
          dropped++; 
          continue; 
        }
        Report r;
        if (!parseReportCSV(line, r)){ 
          dropped++; 
          continue; 
        }
        if (r.report_id <= 0){ 
          dropped++; 
          continue; 
        }
        if (seen.count(r.report_id)){ 
          dropped++; 
          continue; 
        }
        if (r.source_id < 0 || r.source_id >= nSources){ 
          dropped++; 
          continue; 
        }
        if (r.timestamp < 0){ 
          dropped++; 
          continue; 
        }
        if (r.severity < 1 || r.severity > 10){ 
          dropped++; 
          continue; 
        }
        if (!isValidCategory(r.category)){ 
          dropped++; 
          continue; 
        }
        seen.insert(r.report_id);
        out << toCSVRow(r);
        kept++;
    }
    return true;
}
