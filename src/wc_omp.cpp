#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <omp.h>
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

    // Read all words into a vector first
    vector<string> words;
    string word;
    while (file >> word) {
        words.push_back(word);
    }
    file.close();

    auto start = high_resolution_clock::now();

    int total_words = 0;

    // Parallel counting
    #pragma omp parallel for reduction(+:total_words)
    for (int i = 0; i < (int)words.size(); i++) {
        total_words++;  // each thread counts part of the words
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    cout << "Total words: " << total_words << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;

    return 0;
}
