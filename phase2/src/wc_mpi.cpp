#include <mpi.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

// Count words in a local chunk
int countWords(const string &text) {
    int count = 0;
    bool inWord = false;

    for (char c : text) {
        if (isalnum(c)) {
            if (!inWord) {
                inWord = true;
                count++;
            }
        } else {
            inWord = false;
        }
    }
    return count;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 2) {
        if (rank == 0)
            cerr << "Usage: mpirun -np <p> ./wc_mpi <input_file>\n";
        MPI_Finalize();
        return 0;
    }

    string filename = argv[1];
    string fileData;
    int totalSize = 0;

    // -------------------------------
    // 1️⃣ Rank 0 reads input file
    // -------------------------------
    if (rank == 0) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Cannot open file.\n";
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fileData.assign((istreambuf_iterator<char>(file)),
                         istreambuf_iterator<char>());
        totalSize = fileData.size();
    }

    // Broadcast the total file size
    MPI_Bcast(&totalSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // If file empty
    if (totalSize == 0) {
        if (rank == 0) cout << "Total words = 0\n";
        MPI_Finalize();
        return 0;
    }

    // -------------------------------
    // 2️⃣ Build Scatterv counts for uneven chunks
    // -------------------------------
    vector<int> sendCounts(size);
    vector<int> displs(size);

    int base = totalSize / size;
    int extra = totalSize % size;

    for (int i = 0; i < size; i++) {
        sendCounts[i] = base + (i < extra ? 1 : 0);
        displs[i] = (i == 0 ? 0 : displs[i - 1] + sendCounts[i - 1]);
    }

    // Local buffer
    string localChunk(sendCounts[rank], '\0');

    // -------------------------------
    // 3️⃣ Scatter file chunks
    // -------------------------------
    MPI_Scatterv(
        fileData.data(), sendCounts.data(), displs.data(), MPI_CHAR,
        localChunk.data(), sendCounts[rank], MPI_CHAR,
        0, MPI_COMM_WORLD
    );

    // -------------------------------
    // 4️⃣ Non-blocking halo exchange
    // -------------------------------
    char leftHalo = ' ';
    char rightHalo = ' ';

    MPI_Request reqs[4];
    int reqCount = 0;

    // Request left neighbor boundary character
    if (rank != 0) {
        MPI_Irecv(&leftHalo, 1, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, &reqs[reqCount++]);
        MPI_Isend(&localChunk[0], 1, MPI_CHAR, rank - 1, 1, MPI_COMM_WORLD, &reqs[reqCount++]);
    }

    // Request right neighbor boundary character
    if (rank != size - 1) {
        MPI_Irecv(&rightHalo, 1, MPI_CHAR, rank + 1, 1, MPI_COMM_WORLD, &reqs[reqCount++]);
        MPI_Isend(&localChunk.back(), 1, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD, &reqs[reqCount++]);
    }

    // -------------------------------
    // 5️⃣ Compute interior word count
    // -------------------------------
    int localWords = countWords(localChunk);

    // Wait for halo exchanges
    if (reqCount > 0) {
        MPI_Waitall(reqCount, reqs, MPI_STATUSES_IGNORE);
    }

    // -------------------------------
    // 6️⃣ Fix split-word problem
    // -------------------------------
    bool leftIsLetter  = rank != 0        && isalnum(localChunk[0])     && isalnum(leftHalo);
    bool rightIsLetter = rank != size - 1 && isalnum(localChunk.back()) && isalnum(rightHalo);

    if (leftIsLetter)  localWords--;
    if (rightIsLetter) localWords--;

    // -------------------------------
    // 7️⃣ Reduce to rank 0
    // -------------------------------
    int totalWords = 0;
    MPI_Reduce(&localWords, &totalWords, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // -------------------------------
    // 8️⃣ Rank 0 prints result
    // -------------------------------
    if (rank == 0) {
        cout << "Total Words = " << totalWords << endl;
    }

    MPI_Finalize();
    return 0;
}
