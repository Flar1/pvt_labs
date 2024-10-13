#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    int rank, commsize;

    int sizes[] = {1024, 1048576}; 
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    for (int j = 0; j < num_sizes; j++)
    {
        int size = sizes[j];
        char *sbuf = malloc(size * sizeof(char));
        char *rbuf = NULL;

        if (rank == 0)
        {
            rbuf = malloc(size * commsize * sizeof(char)); 
        }

        for (int i = 0; i < size; i++) {
            sbuf[i] = (char)(rank + j);
        }

        double start_time = MPI_Wtime();

        if (rank == 0)
        {
            for (int i = 1; i < commsize; i++)
            {
                MPI_Recv(rbuf + i * size, size, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        
            memcpy(rbuf, sbuf, size); 
        }
        else
        {
            MPI_Send(sbuf, size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        }

        double end_time = MPI_Wtime();

        if (rank == 0)
        {
            printf("size = %d, time = %f\n", size, end_time - start_time);
            free(rbuf);
        }

        free(sbuf);
    }

    MPI_Finalize();
    return 0;
}
