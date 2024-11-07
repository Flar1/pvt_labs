#include <stdio.h>
#include <math.h>
#include <time.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/time.h>

#define N 10000000

const double eps = 1E-6;

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

double func(double x)
{
    return x / (sin(2 * x) * sin(2 * x) * sin(2 * x));
}

int main(int argc, char **argv)
{
    const double a = 1.0;
    const double b = 1.2;
    int n = N, k;
    double sq[2], delta = 1;
    double start, finish;
    start = wtime();
    for (k = 0; delta > eps; n *= 2, k ^= 1)
    {
        double h = (b - a) / n;
        double s = 0.0;
        for (double i = 0; i < n; i++)
            s += func(a + h * (i + 0.5));
        sq[k] = s * h;
        if (n > N)
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
    }
    finish = wtime();
    printf("Time: %f; Result: %.7f; Runge rule: EPS %e, n %d\n", finish - start, sq[k], eps, n / 2);
    return 0;
}