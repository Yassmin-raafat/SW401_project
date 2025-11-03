#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    string filename = argv[1];
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file.\n";
        return 1;
    }

    auto start = high_resolution_clock::now();

    int word_count = 0;
    string word;
    while (file >> word) {
        word_count++;
    }

    auto end = high_resolution_clock::now();
    duration<double, std::micro> duration_us = end - start;  // microseconds
    duration<double> duration_s = end - start;               // seconds

    cout << "Total words: " << word_count << endl;
    cout << "Execution time: " << duration_us.count() << " microseconds (" 
         << duration_s.count() << " seconds)" << endl;

    return 0;
}
