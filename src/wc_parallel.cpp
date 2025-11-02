#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: ./wc_parallel <filename>\n";
        return 1;
    }

    string filename = argv[1];
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file.\n";
        return 1;
    }

    // Read the entire file into memory
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    int word_count = 0;
    auto start = high_resolution_clock::now();

    // Parallel word counting
    #pragma omp parallel for reduction(+:word_count)
    for (int i = 0; i < content.size(); i++) {
        if ((content[i] == ' ' || content[i] == '\n' || content[i] == '\t') &&
            (i > 0 && content[i-1] != ' ' && content[i-1] != '\n' && content[i-1] != '\t')) {
            word_count++;
        }
    }

    auto end = high_resolution_clock::now();
    duration<double> duration = end - start;

    cout << "Total words (parallel): " << word_count << endl;
    cout << "Execution time: " << duration.count() << " seconds" << endl;
    return 0;
}
