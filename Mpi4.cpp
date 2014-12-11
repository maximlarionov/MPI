#include <stdio.h> 
#include <stdlib.h> 
#include <mpi.h>
#include <time.h>

//scalar multiplication

int main(int argc, char* argv[]){
	int x[10]; //1st element
	int y[10]; // 2nd element
	int ProcRank, ProcNum, N = 10;
	int resultMin;
	int resultMax;

	// init and stuff
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	int elements_per_proc = N / ProcNum;
	int *subarr1 = new int[elements_per_proc];
	int *subarr2 = new int[elements_per_proc];

	srand(time(NULL));
	if (ProcRank == 0){
		for (int i = 0; i < N; i++)
		{
			x[i] = rand() % 100;
			printf(" %d ", x[i]);
		}
		printf("\n");
		for (int i = 0; i < N; i++)
		{
			y[i] = rand() % 100;
			printf(" %d ", y[i]);
		}
		printf("\n");
	}
	// Distribute the arrays
	MPI_Scatter(x, elements_per_proc, MPI_INT,
		subarr1, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(y, elements_per_proc, MPI_INT,
		subarr2, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < elements_per_proc; i++)
		subarr1[i] *= subarr2[i];

	// Perform global max reduction
	MPI_Gather(subarr1, elements_per_proc, MPI_INT, x, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);
	if (ProcRank == 0)
		for (int i = 0; i < N; i++)
			printf(" %d ", x[i]);
	MPI_Finalize();
	return 0;
}
