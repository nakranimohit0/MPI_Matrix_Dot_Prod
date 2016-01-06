/*
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	Mo Nakrani
	Matrix Dot Product (MPI)
	Jan. 6, 2016

for i: for j: for k: res[i][j] += A[i][k]*B[k][j]

>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
*/

#include<stdlib.h>
#include<stdio.h>
#include<mpi.h>

void prnt(int N, int l[N][N], char* msg, int pid);

int main(int argc, char* argv[]) {
  int N = atoi(argv[1]), A[N][N], B[N][N], res[N][N], i, j, k, part, elms, start, end;
  
  int np, worker;
  MPI_Comm world;
  MPI_Status status;

  MPI_Init(&argc, &argv);

  world = MPI_COMM_WORLD;
  MPI_Comm_size(world, &np);
  MPI_Comm_rank(world, &worker);

  if (!worker) {
	for (i = 0; i < N; ++i) for (j = 0; j < N; ++j) B[i][j] = (A[i][j] = (i + 1)*(j + 1)) + 1;
	//printf("#%d: %d, %d\n", worker, A[N - 1][N - 1], B[N - 1][N - 1]);
	
	for (i = 0; i < N; ++i) {
	  for (j = 0; j < N; ++j) {
		res[i][j] = 0;
		//for (k = 0; k < N; ++k) res[i][j] += A[i][k]*B[k][j];
	  }
	}
	//prnt(N, res, "Actual ", worker);
  }

  part = N/np;
  elms = part*N;
  start = worker*part;
  end = start + part;

  MPI_Scatter(A, elms, MPI_INT, A[start], elms, MPI_INT, 0, world);
  MPI_Bcast(B, N*N, MPI_INT, 0, world);

  for (i = start; i < end; ++i) {
	for (j = 0; j < N; ++j) {
	  res[i][j] = 0;
	  for (k = 0; k < N; ++k) res[i][j] += A[i][k]*B[k][j];
	}
  }

  MPI_Gather(res[start], elms, MPI_INT, res, elms, MPI_INT, 0, world);

  if (!worker) {
	//prnt(N, res, "Result ", worker);
  }

  MPI_Finalize();
}

void prnt(int N, int l[N][N], char* msg, int pid) {
  int i, j;
  printf("%d: %s:\n", pid, msg);
  
  for (i = 0; i < N; ++i) {
	for (j = 0; j < N; ++j) printf("%11d", l[i][j]);
	printf("\n");
  }
}
