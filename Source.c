#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define MY_ARRAY_LENGTH 67108864
#define MY_ARRAY_LENGTH_FOR_GIGABYTE 67108864

#define TICK(X) clock_t X = clock()
#define TOCK(X) printf("time %s: %g sec.\n", (#X), (double)(clock() - (X)) / CLOCKS_PER_SEC)

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
			sum += a[i] * b[i];
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

	TICK(TIME_array_initialiser);
	double* a = array_initialiser();
	double* b = array_initialiser();
	TOCK(TIME_array_initialiser);

	TICK(TIME_scalar_threads1);
	printf("scalar = %f\n", scalar_threads(a, b, 1));
	TOCK(TIME_scalar_threads1);

	TICK(TIME_scalar_threads2);
	printf("scalar = %f\n", scalar_threads(a, b, 2));
	TOCK(TIME_scalar_threads2);

	TICK(TIME_scalar_threads4);
	printf("scalar = %f\n", scalar_threads(a, b, 4));
	TOCK(TIME_scalar_threads4);

	TICK(TIME_scalar_threads8);
	printf("scalar = %f\n", scalar_threads(a, b, 8));
	TOCK(TIME_scalar_threads8);

	TICK(TIME_scalar_threads10);
	printf("scalar = %f\n", scalar_threads(a, b, 10));
	TOCK(TIME_scalar_threads10);

	TICK(TIME_scalar_threads12);
	printf("scalar = %f\n", scalar_threads(a, b, 12));
	TOCK(TIME_scalar_threads12);

	TICK(TIME_scalar_threads16);
	printf("scalar = %f\n", scalar_threads(a, b, 16));
	TOCK(TIME_scalar_threads16);


	free(a);
	free(b);
	return 0;
}