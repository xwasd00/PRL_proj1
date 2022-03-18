#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    

    // Print off a hello world message
    printf("Hello world, rank %d out of %d processors\n", rank, world_size);

    MPI_Finalize();
}

