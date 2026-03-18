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

