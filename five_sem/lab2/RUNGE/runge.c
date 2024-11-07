#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>

#define N 10000000

const double eps = 1E-6;

double func(double x)
{
    return x / (sin(2 * x) * sin(2 * x) * sin(2 * x));
}

const double a = 0.1;
const double b = 0.5;

int main(int argc, char **argv)
{
    int commsize, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = N, k;
    double sq[2], delta = 1;
    double start_time = 0;
    double finish_time = 0;
    if (rank == 0)
        start_time = MPI_Wtime();
    for (k = 0; delta > eps; n *= 2, k ^= 1)
    {
        int points_per_proc = n / commsize;
        int lb = rank * points_per_proc;
        int ub = (rank == commsize - 1) ? (n - 1) : (lb + points_per_proc - 1);
        double h = (b - a) / n;
        double s = 0.0;
        for (int i = lb; i <= ub; i++)
            s += func(a + h * (i + 0.5));
        MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        sq[k] *= h;
        if (n > N)
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
    }
    if (rank == 0)
    {
        finish_time = MPI_Wtime();
        printf("Time = %f; Result = %f; Runge rule: EPS %e, n %d\n", finish_time - start_time, sq[k], eps, n / 2);
        double speedup = 2.353567/(finish_time - start_time);
        printf("Speedup = %lf", speedup);
    }
    MPI_Finalize();
    return 0;
}