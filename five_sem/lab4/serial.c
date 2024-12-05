#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

// #define N 1000
#define N 10000
#define EPS 0.001
#define PI 3.14159265358979323846
#define IND(i, j) ((i) * nx + (j))

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void *xcalloc(size_t n, size_t size)
{
    void *result = calloc(n, size);
    if (result)
        return result;
    exit(1);
}

int main(int argc, char *argv[])
{
    char *filename = NULL;
    int rows = N;
    int cols = N;
    int ny = rows;
    int nx = cols;
    double finish = wtime();
    double start = wtime();

    double *local_grid = xcalloc(ny * nx, sizeof(*local_grid));
    double *local_newgrid = xcalloc(ny * nx, sizeof(*local_newgrid));
    double dx = 1.0 / (nx - 1.0);
    // Initialize top border: u(x, 0) = sin(pi * x)
    for (int j = 0; j < nx; j++)
    {
        int ind = IND(0, j);
        local_newgrid[ind] = local_grid[ind] = sin(PI * dx * j);
    }
    // Initialize bottom border: u(x, 1) = sin(pi * x) * exp(-pi)
    for (int j = 0; j < nx; j++)
    {
        int ind = IND(ny - 1, j);
        local_newgrid[ind] = local_grid[ind] = sin(PI * dx * j) * exp(-PI);
    }
    int niters = 0;
    for (;;)
    {
        niters++;
        for (int i = 1; i < ny - 1; i++)
        { // Update interior points
            for (int j = 1; j < nx - 1; j++)
            {
                local_newgrid[IND(i, j)] =
                    (local_grid[IND(i - 1, j)] + local_grid[IND(i + 1, j)] +
                     local_grid[IND(i, j - 1)] + local_grid[IND(i, j + 1)]) *
                    0.25;
            }
        }
        // Check termination condition
        double maxdiff = 0;
        for (int i = 1; i < ny - 1; i++)
        {
            for (int j = 1; j < nx - 1; j++)
            {
                int ind = IND(i, j);
                maxdiff = fmax(maxdiff, fabs(local_grid[ind] - local_newgrid[ind]));
            }
        }
        // Swap grids (after termination local_grid will contain result)
        double *p = local_grid;
        local_grid = local_newgrid;
        local_newgrid = p;
        if (maxdiff < EPS)
            break;
    }
    finish = wtime();
    printf("# Heat 2D (serial): grid: rows %d, cols %d\n", rows, cols);
    printf("# niters %d, total time %.6f\n", niters, finish - start);
    // Save grid
    if (argc > 1)
        filename = argv[1];
    if (filename)
    {
        FILE *fout = fopen(filename, "w");
        if (!fout)
        {
            perror("Can't open file");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < ny; i++)
        {
            for (int j = 0; j < nx; j++)
                fprintf(fout, "%.4f ", local_grid[IND(i, j)]);
            fprintf(fout, "\n");
        }
        fclose(fout);
    }
    return 0;
}