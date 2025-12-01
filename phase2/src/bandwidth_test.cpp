#include <mpi.h>
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const long messageSize = 50 * 1024 * 1024; // 50 MB
    vector<char> buffer(messageSize, 'x');

    MPI_Barrier(MPI_COMM_WORLD);

    double start = MPI_Wtime();

    if (rank == 0) {
        MPI_Send(buffer.data(), messageSize, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(buffer.data(), messageSize, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } 
    else if (rank == 1) {
        MPI_Recv(buffer.data(), messageSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(buffer.data(), messageSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    double end = MPI_Wtime();

    if (rank == 0) {
        double totalTime = end - start;
        double bandwidth = (2.0 * messageSize) / totalTime / (1024*1024);
        cout << "Bandwidth = " << bandwidth << " MB/s" << endl;
    }

    MPI_Finalize();
    return 0;
}
