#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <stdlib.h>
#include "mat.h"

int main(int argc, char* argv[]) {
    clock_t start, end;
    double *a, *b, *calc1, *calc2, *calc3;
    int n;
    double times[3];
    if (argc > 1) {
        FILE *fp1;
        FILE *fp2;
        FILE *fp3;
        fp1 = fopen("graphUnoptimizedMmult.txt", "a");
        fp2 = fopen("graphSIMD.txt", "a");
        fp3 = fopen("graphOMP.txt", "a");
        n = atoi(argv[1]);
        a = gen_matrix(n, n);//generate matrix a
        b = gen_matrix(n, n);//generate matrix b
        calc1 = malloc(sizeof(double) * n*n);//calculate UnoptimizedMmult
        calc2 = malloc(sizeof(double) * n*n);//calculate SIMD
        calc3 = malloc(sizeof(double) * n*n);//calculate OMP

        start = clock();
        mmult(calc1, a, n, n, b, n, n);
        end = clock();
        times[0] = ((double) (end - start)) / CLOCKS_PER_SEC;
        fprintf(fp1, "%d %.7lf", n, times[0]);
        fprintf(fp1, "\n");
        fclose(fp1);

        start = clock();
        mmult_simd(calc1, a, n, n, b, n, n);
        end = clock();
        times[1] = ((double) (end - start)) / (double)CLOCKS_PER_SEC;
        fprintf(fp2, "%d %.7lf", n, times[1]);
        fprintf(fp2, "\n");
        fclose(fp2);

        start = clock();
        mmult_omp(calc3, a, n, n, b, n, n);
        end = clock();
        times[2] = ((double) (end - start)) / (double)CLOCKS_PER_SEC;
        fprintf(fp3, "%d %.7lf", n, times[2]);
        fprintf(fp3, "\n");
        fclose(fp3);
        }
}

