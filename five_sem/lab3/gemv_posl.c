#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

const int n = 45000;
const int m = 45000;

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

void *xmalloc(size_t size)
{
    void *result = malloc(size);
    if (result)
        return result;
    exit(1);
}

void dgemv(float *a, float *b, float *c, int m, int n)
{
    for (int i = 0; i < m; i++)
    {
        c[i] = 0.0;
        for (int j = 0; j < n; j++)
            c[i] += a[i * n + j] * b[j];
    }
}

int main(int argc, char **argv)
{
    printf("Memory used: %" PRIu64 " MiB\n", (uint64_t)(((double)m * n + m + n) * sizeof(float)) >> 20);
    double t = wtime();
    float *a, *b, *c;
    a = xmalloc(sizeof(*a) * m * n);
    b = xmalloc(sizeof(*b) * n);
    c = xmalloc(sizeof(*c) * m);
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            a[i * n + j] = i + 1;
    }
    for (int j = 0; j < n; j++)
        b[j] = j + 1;
    dgemv(a, b, c, m, n);
    t = wtime() - t;
    // Validation
    for (int i = 0; i < m; i++)
    {
        double r = (i + 1) * (n / 2.0 + pow(n, 2) / 2.0);
        if (fabs(c[i] - r) > 1E-6)
        {
            fprintf(stderr, "Validation failed: elem %d = %f (real value %f), difference = %f\n", i, c[i], r, fabs(c[i] - r));
            break;
        }
    }
    double gflop = 2.0 * m * n * 1E-9;
    printf("Elapsed time (serial): %.6f sec.\n", t);
    printf("Performance: %.2f GFLOPS\n", gflop / t);
    free(a);
    free(b);
    free(c);
    return 0;
}