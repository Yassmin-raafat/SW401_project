#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: ./wc_seq <filename>\n";
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Error opening file!\n";
        return 1;
    }

    string word;
    long long count = 0;

    auto start = high_resolution_clock::now();
    while (file >> word) {
        count++;
    }
    auto end = high_resolution_clock::now();

    double duration = duration_cast<milliseconds>(end - start).count();
    cout << "Total words: " << count << endl;
    cout << "Execution time (ms): " << duration << endl;

    return 0;
}
