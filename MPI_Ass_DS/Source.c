#include <stdio.h>
#include "mpi.h"
#include "math.h"


//taking input from user
void get_input(int* k_upper_limit, int rank, int comm_sz, double* x) {

	if (rank == 0) {
		printf("Enter upper_limit and x value\n");
		fflush(stdout); fflush(stdin);
		scanf_s("%d %lf", k_upper_limit, x);


		for (int dist = 1; dist < comm_sz; dist++) {
			MPI_Send(k_upper_limit, 1, MPI_INT, dist, 0, MPI_COMM_WORLD);
			MPI_Send(x, 1, MPI_DOUBLE, dist, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(k_upper_limit, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(x, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
}
long double factorial(int k)
{
	long double res = 1, i;
	for (i = 2; i <= k; i++)
		res *= i;
	return res;
}
double calculation(int local_k, double x, int k) {
	double result = 0;
	long double fact;
	for (int i = 0; i < local_k; i++) {
		fact = factorial(2 * k);
		result += (pow(-1, k) * pow(x, 2 * k)) / fact;
		k++;
	}
	return result;
}
int main(int argc, char** argv) {
	int k = 0, rank = 0, size = 0, k_upper_limit;
	double x, total_result, local_result;
	double start_time, end_time, time_spent;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	get_input(&k_upper_limit, rank, size, &x);

	start_time = MPI_Wtime();

	k_upper_limit++;
	int local_k = k_upper_limit / size;

	k = k + (rank * local_k);

	local_result = calculation(local_k, x, k);

	if (rank != 0) {
		MPI_Send(&local_result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	else {
		total_result = local_result;
		for (int source = 1; source < size; source++) {
			MPI_Recv(&local_result, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total_result += local_result;
		}
	}
	if (rank == 0) {
		k_upper_limit--;
		printf("cos(x)=%f as x=%f and k_upper_limit=%d\n", total_result, x, k_upper_limit);
		end_time = MPI_Wtime();
		time_spent = end_time - start_time;
		printf("the time spent = %lf", time_spent);
	}
	MPI_Finalize();
	return 0;
}