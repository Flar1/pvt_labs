#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void ring(size_t msg_siz)
{
    int rank;
    int size;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char *msg = (char *)malloc(msg_siz);

    int next = (rank + 1) % size;
    int prev = (rank + size - 1) % size;

    double start, end;
    if (rank == 0)
    {
        start = MPI_Wtime();
    }

    for (int i = 0; i < size - 1; i++)
    {
        if (rank % 2 == 0)
        {
            MPI_Send(msg, msg_siz, MPI_CHAR, next, 0, MPI_COMM_WORLD);
            MPI_Recv(msg, msg_siz, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        else
        {
            MPI_Recv(msg, msg_siz, MPI_CHAR, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(msg, msg_siz, MPI_CHAR, next, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == 0)
    {
        end = MPI_Wtime();
        printf("Message size = %zu Time = %f\n", msg_siz, end - start);
    }

    free(msg);
}

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    ring(1);
    ring(1024);
    ring(1024 * 1024);

    MPI_Finalize();
    return 0;
}
