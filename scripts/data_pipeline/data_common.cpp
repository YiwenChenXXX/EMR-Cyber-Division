#include "data_common.h"
#include <algorithm>
using namespace std;

vector<string> splitCSV(const string& line){
    vector<string> parts;
    string cur;
    for (char c : line){
        if (c == ','){
          parts.push_back(cur); 
          cur.clear(); 
        }
        else{
          cur.push_back(c);
        }
    }
    parts.push_back(cur);
    return parts;
}

bool isValidCategory(const string& c) {
    static const vector<string> cats = {"medical","fire","flood","rescue","power"};
    return find(cats.begin(), cats.end(), c) != cats.end();
}

string csvHeader() {
    return "report_id,source_id,timestamp,severity,category\n";
}

string toCSVRow(const Report& r) {
    return to_string(r.report_id) + "," +
           to_string(r.source_id) + "," +
           to_string(r.timestamp) + "," +
           to_string(r.severity) + "," +
           r.category + "\n";
}

bool parseReportCSV(const std::string& line, Report& out) {
    auto p = splitCSV(line);
    if (p.size() != 5){
        return false;
    }
    try{
        out.report_id = stoi(p[0]);
        out.source_id = stoi(p[1]);
        out.timestamp = stoll(p[2]);
        out.severity  = stoi(p[3]);
        out.category  = p[4];
        return true;
    } 
    catch (...){
        return false;
    }
}
