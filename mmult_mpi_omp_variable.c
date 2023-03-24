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
	        
  	nrows = atoi(argv[1]);
        ncols = nrows;

        aa = (double*)malloc(sizeof(double) * nrows * ncols);
        bb = (double*)malloc(sizeof(double) * ncols * nrows);
        cc1 = (double*)malloc(sizeof(double) * nrows * ncols);
        double *buffer = (double*)malloc(sizeof(double) * ncols);
        double *ans = (double*)malloc(sizeof(double) * ncols);
	bb = gen_matrix(ncols, nrows); 

        if (myid == 0) {
            // Controller Code goes here
            starttime = MPI_Wtime();
            

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
	
	    FILE *output = fopen("graphMPIOMP.txt",  "a");
	    if(output == NULL){
		perror("File not found");		
		exit(1);	
	    }
	    fprintf(output,"%.7f\n", (endtime - starttime));
	    fclose(output);		
	 

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
        fprintf(stderr, "Input a numerical size as the only input.\n");
    }
    MPI_Finalize();
    return 0;
}
