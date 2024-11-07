#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define N 10000000
// #define N 100000000

double getrand()
{
    return (double)rand() / RAND_MAX;
}

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double func(double x, double y)
{
    return (exp(x - y));
}

int main()
{
    int in = 0;
    double s = 0;
    double t = wtime();
    for (int i = 0; i < N; i++)
    {
        double x = getrand() * -1;
        double y = getrand();
        if (y <= 1)
        {
            in++;
            s += func(x, y);
        }
    }
    double v = 1 * in / N;
    double res = v * s / in;
    t = wtime() - t;
    printf("Elapsed time (serial): %.6f sec.\n", t);
    printf("Result: %.12f, N %d\n", res, N);
    return 0;
}