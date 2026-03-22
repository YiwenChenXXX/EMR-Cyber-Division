#pragma once
#include <string>
#include <vector>
using namespace std;

struct Report{
  int report_id;
  int source_id;
  long long timestamp;
  int severity; // from 1 to 10
  string category;  // e.g., medical/fire/flood/rescue/power
};

vector<string> splitCSV(const string& line);
bool isValidCategory(const string& c);
string csvHeader();
string toCSVRow(const Report& r);
bool parseReportCSV(const string& line, Report& out);
