#pragma once
#include <string>
#include <vector>
using namespace std;

struct Report{
  int report_id;
  int source_id;
  long long timestamp;
  int severity; // from 1 to 10
  std::string category;  // e.g., medical/fire/flood/rescue/power
};
