#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <mpi.h>

int get_chunk(int n, int commsize, int rank)
{
    int q = n / commsize;
    if (n % commsize)
        q++;
    int r = commsize * q - n;
    /* Compute chunk size for the process */
    int chunk = q;
    if (rank >= commsize - r)
        chunk = q - 1;
    return chunk;
}

int main(int argc, char *argv[])
{
    int n = 3000;
    int rank, commsize;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int nrows = get_chunk(n, commsize, rank);
    int *rows = malloc(sizeof(*rows) * nrows);
    double *a = malloc(sizeof(*a) * n * nrows); // Матрица коэффициентов
    double *tmp = malloc(sizeof(*tmp) * n);

    for (int i = 0; i < nrows; i++)
    {
        rows[i] = rank + commsize * i;
        srand(rows[i] * (n + 1));
        for (int j = 0; j < n; j++)
            a[i * n + j] = rand() % 1000 + 1;
    }

    // Прямой ход
    double t = MPI_Wtime();
    int row = 0;
    for (int i = 0; i < n - 1; i++)
    {
        // Исключаем x_i
        if (i == rows[row])
        {
            // Рассылаем строку i, находящуюся в памяти текущего процесса
            MPI_Bcast(&a[row * n], n, MPI_DOUBLE, rank, MPI_COMM_WORLD);
            for (int j = 0; j < n; j++)
                tmp[j] = a[row * n + j];
            row++;
        }
        else
        {
            MPI_Bcast(tmp, n, MPI_DOUBLE, i % commsize, MPI_COMM_WORLD);
        }
        // Вычитаем принятую строку из уравнений, хранящихся в текущем процессе
        for (int j = row; j < nrows; j++)
        {
            double scaling = a[j * n + i] / tmp[i];
            for (int k = i; k < n; k++)
                a[j * n + k] -= scaling * tmp[k];
        }
    }

    double local_det = 1.0;
    for (int i = 0; i < nrows; i++)
    {
        local_det *= a[rows[i]];
    }

    double global_det = 1.0;
    MPI_Reduce(&local_det, &global_det, 1, MPI_DOUBLE, MPI_PROD, 0, MPI_COMM_WORLD);
    t = MPI_Wtime() - t;
    free(tmp);
    free(rows);
    free(a);

    if (rank == 0)
    {
        printf("Gaussian Elimination (MPI): n %d, time (sec) %.6f\n", n, t);
        printf("Speedup :%lf \n", ((328.534671) / t));
    }

    MPI_Finalize();
    return 0;
}