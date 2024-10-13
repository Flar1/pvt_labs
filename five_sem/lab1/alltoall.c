#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MESSAGE_SIZE 1024

void all_to_all_test() {
    int rank, size;
    char *sbuf, *rbuf;
    double start_time, end_time;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    sbuf = (char*)malloc(MESSAGE_SIZE * sizeof(char));
    rbuf = (char*)malloc(MESSAGE_SIZE * size * sizeof(char));

    for (int i = 0; i < MESSAGE_SIZE; i++) {
        sbuf[i] = 'A' + rank;
    }

    MPI_Request *send_requests = (MPI_Request*)malloc(size * sizeof(MPI_Request));
    MPI_Request *recv_requests = (MPI_Request*)malloc(size * sizeof(MPI_Request));

    start_time = MPI_Wtime();

    for (int i = 0; i < size; i++) {
        MPI_Isend(sbuf, MESSAGE_SIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD, &send_requests[i]);
        MPI_Irecv(rbuf + i * MESSAGE_SIZE, MESSAGE_SIZE, MPI_CHAR, i, 0, MPI_COMM_WORLD, &recv_requests[i]);
    }

    MPI_Waitall(size, send_requests, MPI_STATUSES_IGNORE);
    MPI_Waitall(size, recv_requests, MPI_STATUSES_IGNORE);

    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("Time for All-to-All: %f seconds\n", end_time - start_time);
    }

    free(sbuf);
    free(rbuf);
    free(send_requests);
    free(recv_requests);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    all_to_all_test();
    MPI_Finalize();
    return 0;
}