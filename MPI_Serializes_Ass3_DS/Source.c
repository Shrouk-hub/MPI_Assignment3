#include <stdio.h>
#include "math.h"
#include <time.h>

double factorial(int k)
{
	double res = 1, i;
	for (i = 2; i <= k; i++)
		res *= i;
	return res;
}

int main(int argc, char** argv) {
	int k_upper_limit;
	double x,result=0, time_spent;
	clock_t t, t2; 

	printf("Enter upper_limit and x value\n");
	fflush(stdout); fflush(stdin);
	scanf_s("%d %lf", &k_upper_limit, &x);

	t = clock();

	double fact;
	for (int i = 0; i <= k_upper_limit; i++) {
		fact = factorial(2 * i);
		result += (pow(-1, i) * pow(x, 2 * i)) / fact;
	}
	printf("cos(x)=%lf as x=%lf and k_upper_limit=%d\n", result, x, k_upper_limit);
	t2 = clock();
	time_spent = (double)(t2 - t) / CLOCKS_PER_SEC;
	printf("the time spent = %lf", time_spent);
	
}