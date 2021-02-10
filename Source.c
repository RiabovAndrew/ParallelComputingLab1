#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define MY_ARRAY_LENGTH 536870912
#define MY_ARRAY_LENGTH_FOR_GIGABYTE 67108864
//#define ADDICTION_CALCULATION


double scalar(const double* a, const double* b) {
	double sum = 0;
	for (size_t i = (size_t)0; i < MY_ARRAY_LENGTH; i++) {
		sum += a[i] * b[i];
	}
	return sum;
}

double scalar_threads(const double* a, const double* b, const int threads) {
	double sum = 0;
	int i;
	#pragma omp parallel for shared(a, b) reduction (+: sum) private(i) num_threads(threads)
		for (i = 0; i < MY_ARRAY_LENGTH; i++) {
			#ifdef ADDICTION_CALCULATION
				sum += sqrt(sqrt(sqrt(a[i] * powf(b[i], 30))));
			#else
				sum += a[i] * b[i];
			#endif
		}
	return sum;
}

double* array_initialiser() {
	double* arr = (double*) malloc(MY_ARRAY_LENGTH * sizeof(double));
	if (!arr) {
		perror("malloc"); 
		exit(EXIT_FAILURE); 
	}
	int i;
	#pragma omp parallel for shared(arr) private(i) num_threads(8)
	for (i = 0; i < MY_ARRAY_LENGTH; i++) arr[i] = rand() % 10;
	return arr;
}

int main(void) {

	srand(time(NULL));

	double start = omp_get_wtime();
	double* a = array_initialiser();
	double* b = array_initialiser();
	printf("array_initialiser; 8 threads; time: %f sec\n", (omp_get_wtime() - start));

	for (size_t i = (size_t)1; i <= 16; i += 2) {
		start = omp_get_wtime();
		printf("scalar = %f\n", scalar_threads(a, b, i));
		printf("%zu thread(s); time: %f sec\n", i, (omp_get_wtime() - start));
		if (i == 1) i--;
	}

	free(a);
	free(b);
	return 0;
}