#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 10000;  // number of ping-pong tests
    char msg = 'x';
    double start, end;

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        start = MPI_Wtime();
        for (int i = 0; i < N; i++) {
            MPI_Send(&msg, 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&msg, 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        end = MPI_Wtime();

        double latency = (end - start) / (2.0 * N);
        cout << "Estimated Latency = " << latency * 1e6 << " microseconds" << endl;
    } 
    else if (rank == 1) {
        for (int i = 0; i < N; i++) {
            MPI_Recv(&msg, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&msg, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
