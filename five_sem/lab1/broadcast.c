#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void make_broadcast_blocking(int size, int argc, char **argv)
{
    int rank, commsize;
    char *sbuf = NULL;
    char *rbuf = malloc(size);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    if (rank == 0)
    {
        sbuf = malloc(size);
        memset(sbuf, 'a', size); 
    }

    double start_time = MPI_Wtime();

    if (rank == 0)
    {
        for (int dest = 0; dest < commsize; dest++)
        {
            if (dest == 0)
            {
                memcpy(rbuf, sbuf, size); 
            }
            else
            {
                MPI_Send(sbuf, size, MPI_CHAR, dest, 0, MPI_COMM_WORLD);
            }
        }
    }
    else
    {
        MPI_Recv(rbuf, size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
    }

    double end_time = MPI_Wtime();

    if (rank == 0)
    {
        printf("size = %d Time = %f\n", size, end_time - start_time);
        free(sbuf);
    }

    free(rbuf);

}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int MSG_SIZE = 1024;
    make_broadcast_blocking(MSG_SIZE, argc, argv);
    make_broadcast_blocking(MSG_SIZE*1024, argc, argv);
    MPI_Finalize();
    return 0;
}
