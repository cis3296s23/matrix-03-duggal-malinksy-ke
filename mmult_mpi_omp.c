/** 
 * Incomplete program to multiply a matrix times a matrix using both
 * MPI to distribute the computation among nodes and OMP
 * to distribute the computation among threads.
 */

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#include <string.h>
#define min(x, y) ((x)<(y)?(x):(y))

#include "mat.h"

int main(int argc, char* argv[])
{
    int nrows, ncols;
    double *aa;	/* the A matrix */
    double *bb;	/* the B matrix */
    double *cc1;	/* A x B computed using the omp-mpi code you write */
    double *cc2;	/* A x B computed using the conventional algorithm */
    int myid, numprocs;
    double starttime, endtime;
    FILE *a;
    FILE *b;
    char* readLine;
    double *times;
    double total_times;
    int run_index;
    int nruns;
    int i, j, numsent, sender;
    int anstype, row;
    srand(time(0));
    MPI_Status status;

    /* insert other global variables here */

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    if (argc > 1) {
        a = fopen(argv[1], "r");
        b = fopen(argv[2], "r");
        if(a == NULL || b == NULL){
            fprintf(stderr, "Bad File\n");
            exit(1);
        }
	
	int arows, acols, brows, bcols;
	int readvalues = fscanf(a, "%d %d", &arows, &acols);
	readvalues = fscanf(b, "%d %d", &brows, &bcols);
	
	if(arows != brows || acols != bcols){
	   perror("Mismatched dimensions!.\n");
	   exit(1);
	}

	nrows = arows;
	ncols = acols;
	
	fclose(a);
	fclose(b);

        aa = (double*)malloc(sizeof(double) * nrows * ncols);
        bb = (double*)malloc(sizeof(double) * ncols * nrows);
        cc1 = (double*)malloc(sizeof(double) * nrows * ncols);
        double *buffer = (double*)malloc(sizeof(double) * ncols);
        double *ans = (double*)malloc(sizeof(double) * ncols);
	aa = read_matrix_from_file(argv[1]);
	bb = read_matrix_from_file(argv[2]);

        if (myid == 0) {
            // Controller Code goes here
            starttime = MPI_Wtime();
            /* Insert your controller code here to store the product into cc1 */
            numsent = 0;
            MPI_Bcast(bb, ncols*nrows, MPI_DOUBLE, 0,
                      MPI_COMM_WORLD);
            for (i = 0; i < min(numprocs-1, nrows); i++) {
                for (j = 0; j < ncols; j++) {
                    buffer[j] = aa[i * ncols + j];
                }
                MPI_Send(buffer, ncols, MPI_DOUBLE, i+1, i+1,
                         MPI_COMM_WORLD);
                numsent++;
            }
            for (i = 0; i < nrows; i++) {
                MPI_Recv(ans, ncols, MPI_DOUBLE, MPI_ANY_SOURCE,
                         MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                sender = status.MPI_SOURCE;
                anstype = status.MPI_TAG;
                int k = anstype-1;
                for(j = 0; j < ncols; j++){
                    cc1[k*ncols+j] = ans[j];
                }
                //cc1[anstype-1] = ans;
                if (numsent < nrows) {
                    for (j = 0; j < ncols; j++) {
                        buffer[j] = aa[numsent*ncols + j];
                    }
                    MPI_Send(buffer, ncols, MPI_DOUBLE, sender, numsent+1,
                             MPI_COMM_WORLD);
                    numsent++;
                } else {
                    MPI_Send(MPI_BOTTOM, 0, MPI_DOUBLE, sender, 0,
                             MPI_COMM_WORLD);
                }
            }
            endtime = MPI_Wtime();
            printf("%f\n",(endtime - starttime));
            cc2  = malloc(sizeof(double) * nrows * ncols);
            mmult(cc2, aa, nrows, ncols, bb, ncols, nrows);
            compare_matrices(cc2, cc1, nrows, ncols);

            FILE *c = fopen("c.txt", "w");
            fprintf(c, "%d %d\n", nrows, ncols);
            for (int y = 0; y < (nrows*ncols); y++) {
                if(y % ncols == 0){
                    fprintf(c,"\n");
                }
                fprintf(c, "%lf ", cc1[y]);
            }

            fclose(c);

        } else {
            // Worder code goes here
            MPI_Bcast(bb,nrows*ncols, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            if (myid <= nrows) {
                while(1) {
                    MPI_Recv(buffer, ncols, MPI_DOUBLE, 0,
                             MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                    if (status.MPI_TAG == 0){
                        break;
                    }
                    row = status.MPI_TAG;
                    memset(ans, 0, ncols*sizeof(*ans));
                    mmult_omp(ans, buffer, 1, ncols, bb, nrows, ncols);
                    MPI_Send(ans, ncols, MPI_DOUBLE, 0, row,
                             MPI_COMM_WORLD);
                }
            }
        }
    } else {
        fprintf(stderr, "Input at least two text files\n");
    }
    MPI_Finalize();
    return 0;
}
