#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h>
#include <time.h>

//vector max

int main(int argc, char* argv[]){
	int x[10];  //our vector
	int ProcRank, ProcNum, N = 10;
	int result;

	// trivial stuff for distribution init
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int elements_per_proc = N / ProcNum;
	int *subarr = new int[elements_per_proc];

	int localmax;
	srand(time(NULL));
	if (ProcRank == 0){
		for (int i = 0; i < N; i++)
		{
			x[i] = rand() % 100;
			printf(" %d, ", x[i]);
		}
	}
	// Distribute the array
	MPI_Scatter(x, elements_per_proc, MPI_INT,
		subarr, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

	// Find the maximum element of the local subarray
	localmax = 0;
	for (int i = 0; i < elements_per_proc; i++)
		if (subarr[i] > localmax) localmax = subarr[i];

	// Perform global max reduction
	MPI_Reduce(&localmax, &result, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	if (ProcRank == 0)
		printf("\nmax = %d", result);
	MPI_Finalize();
	return 0;
}
