#include "cleaner.h"
#include <iostream>
#include <string>
using namespace std;

static string getArg(int argc, char** argv, const string& key, const string& def) {
    for (int i = 1; i + 1 < argc; i++) {
        if (string(argv[i]) == key){
          return string(argv[i + 1]);
        }
    }
    return def;
}

int main(int argc, char** argv) {
    string in  = getArg(argc, argv, "--in",  "data/generated/raw.csv");
    string out = getArg(argc, argv, "--out", "data/generated/clean.csv");
    int sources = stoi(getArg(argc, argv, "--sources", "50"));
    long long kept = 0, dropped = 0;
    bool ok = cleanDatasetCSV(in, out, sources, kept, dropped);
    if (ok){
      cout << "Cleaned. kept=" << kept << " dropped=" << dropped << "\n";
    }
    else{
      cout << "Failed cleaning\n";
    }
    return ok ? 0 : 1;
}
