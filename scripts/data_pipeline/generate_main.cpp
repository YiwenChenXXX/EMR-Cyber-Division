#include "generator.h"
#include <iostream>
#include <string>
using namespace std;

static string getArg(int argc, char** argv, const std::string& key, const std::string& def) {
    for (int i = 1; i + 1 < argc; i++) {
        if (string(argv[i]) == key){
          return string(argv[i + 1]);
        }
    }
    return def;
}

int main(int argc, char** argv) {
    string out = getArg(argc, argv, "--out", "data/generated/raw.csv");
    int n = stoi(getArg(argc, argv, "--n", "100000"));
    int sources = stoi(getArg(argc, argv, "--sources", "50"));
    unsigned int seed = (unsigned int)stoul(getArg(argc, argv, "--seed", "42"));
    long long startTs = stoll(getArg(argc, argv, "--start_ts", "1709500000"));
    bool ok = generateDatasetCSV(out, n, sources, seed, startTs);
    std::cout << (ok ? "Generated " : "Failed generating ") << n << " reports\n";
    return ok ? 0 : 1;
}
