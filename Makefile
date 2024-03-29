PGMS=mmult_omp_timing matrix_times_vector hello test_mmult mxv_omp_mpi mmult_mpi_omp mmult_mpi_omp_variable

EXES = test_mmult test_mmult_vector test_mmult_simd test_mmult_simd_vector main_out main_out_vector

all:	${PGMS} ${EXES}

mmult_mpi_omp:		mmult.o mmult_omp.o mmult_mpi_omp.o mat.c
	mpicc -o mmult_mpi_omp -fopenmp -O3 mmult.o mmult_omp.o mmult_mpi_omp.o mat.c

mmult_mpi_omp.o:	mmult_mpi_omp.c
	mpicc -c -fopenmp -O3 mmult_mpi_omp.c

mmult_mpi_omp_variable:		mmult.o mmult_omp.o mmult_mpi_omp_variable.o mat.c
	mpicc -o mmult_mpi_omp_variable -fopenmp -O3 mmult.o mmult_omp.o mmult_mpi_omp_variable.o mat.c

mmult_mpi_omp_variable.o:	mmult_mpi_omp_variable.c
	mpicc -c -fopenmp -O3 mmult_mpi_omp_variable.c


mmult_omp_timing:	mmult.o mmult_omp.o mmult_omp_timing.o mat.c
	gcc -o mmult -fopenmp -O3 mmult.o mmult_omp.o mmult_omp_timing.o mat.c -o mmult_omp_timing

mat.o:	mat.c
	gcc -c mat.c 

mmult.o:	mmult.c
	gcc -c mmult.c

mmult_simd.o:	mmult_simd.c
	gcc -c mmult_simd.c

mmult_omp.o:	mmult_omp.c
	gcc -c -O3 -fopenmp mmult_omp.c

mmult_omp_timing.o:	mmult_omp_timing.c
	gcc -c -O3 mmult_omp_timing.c

matrix_times_vector:	matrix_times_vector.c mat.c
	mpicc -O3 -o matrix_times_vector matrix_times_vector.c mat.c

hello:	hello.c
	mpicc -O3 -o hello hello.c

mxv_omp_mpi:	mxv_omp_mpi.c mat.c
	mpicc -fopenmp -O3 -o mxv_omp_mpi mxv_omp_mpi.c mat.c

test_mmult:	test_mmult.c mmult.c mat.c
	gcc test_mmult.c mmult.c mat.c -lm -o test_mmult

test_mmult_vector:	test_mmult.c mmult.c mat.c
	gcc test_mmult.c mmult.c mat.c -lm -O3 -o test_mmult_vector

run_test_mmult:	test_mmult
	./test_mmult

test_mmult_simd:	test_mmult_simd.c mmult_simd.c mat.c
	gcc test_mmult_simd.c mmult_simd.c mat.c -lm -o test_mmult_simd

test_mmult_simd_vector:	test_mmult_simd.c mmult_simd.c mat.c
	gcc test_mmult_simd.c mmult_simd.c mat.c -lm -O3 -o test_mmult_simd_vector

main_out:	main.c mmult_omp.c mmult_simd.c mat.c mmult.c
	gcc -fopenmp main.c mmult_omp.c mmult_simd.c mat.c mmult.c -lm -o main_out

main_out_vector:	main.c mmult_omp.c mmult_simd.c mat.c mmult.c
	gcc -fopenmp main.c mmult_omp.c mmult_simd.c mat.c mmult.c -lm -O3 -o main_out_vector

clean:
	rm -f *.o
	rm -f ${PGMS}
	rm -f ${EXES}
