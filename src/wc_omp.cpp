#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

int main() {
    string filename = "sample.txt";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file.\n";
        return 1;
    }

    auto start = high_resolution_clock::now();

    int word_count = 0;
    string line;

    #pragma omp parallel for reduction(+:word_count)
    for (int i = 0; i < 1000; i++) { // simulate work on chunks
        ifstream localFile(filename);
        string word;
        while (localFile >> word) {
            word_count++;
        }
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    cout << "Total words: " << word_count << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;

    return 0;
}
