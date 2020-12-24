/******************************************************************************
	This program demonstrates matrix multiplication with threads.
	Compile this program with cc -o Matrix_Multiplication  Matrix_Multiplication.c  -pthread

******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define MAT_SIZE 1024
int MAX_THREADS;


int R1,C1,C2;         //Parameters For Rows And Columns
int FIRST_MATRIX[MAT_SIZE][MAT_SIZE]; //First Matrix
int SECOND_MATRIX[MAT_SIZE][MAT_SIZE]; //Second Matrix
int RESULT_MATRIX [MAT_SIZE][MAT_SIZE]; //Multiplied Matrix
int step_i = 0;

//Function For Calculate Each Element in Result Matrix Used By Threads - - -//
void* multiply(void* arg){


    int core = step_i++;
    //Calculating Each Element in Result Matrix Using Passed Arguments
    for (int i = core * R1 / MAX_THREADS; i < (core + 1) * R1 / MAX_THREADS; i++)
        for (int k = 0; k< C2; k++)
            for (int j = 0; j< C1; j++)
                RESULT_MATRIX[i][j] += FIRST_MATRIX[i][k] * SECOND_MATRIX[k][j];
    //sleep(3);

    //End Of Thread
    pthread_exit(0);
}

int calculateTimeDifference(struct timespec *start, struct timespec *finish,
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec;
  long long int dn =  finish->tv_nsec - start->tv_nsec;

  if(dn < 0 ) {
    ds--;
    dn += 1000000000;
  }
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

int main() {
	struct timespec start, finish;
	long long int time_elapsed;
	// Getting Row And Column(Same As Row In second matrix) Number For first matrix
	printf("Enter number of rows for first Matrix: ");
	scanf("%d", &R1);
	printf("Enter number of columns for first Matrix: ");
	scanf("%d", &C1);
	printf("Enter number of columns for second Matrix: ");
	scanf("%d", &C2);
	printf("Enter the number for threads for matrix multiplication: \n");
	scanf("%d", &MAX_THREADS);


	for (int x = 0; x < R1; x++) {
		for (int y = 0; y < C1; y++) {
			FIRST_MATRIX[x][y] = rand() % 50;
		}
	}

	for (int x = 0; x < C1; x++) {
		for (int y = 0; y < C2; y++) {
			SECOND_MATRIX[x][y] = rand() % 50;
		}
	}

	pthread_t *t = malloc(sizeof(pthread_t) * MAX_THREADS);

	//Defining Threads
	pthread_t thread[MAX_THREADS];

	//Counter For Thread Index
	int thread_number = 0;

	//Defining p For Passing Parameters To Function As Struct


	//Start Timer
	clock_gettime(CLOCK_MONOTONIC, &start);


	for (int x = 0; x < MAX_THREADS; x++)
	{
		int *p;
		//Status For Checking Errors
		int status;

		//Create Specific Thread For Each Element In Result Matrix
		status = pthread_create(&thread[thread_number], NULL, multiply, (void *)&p[thread_number]);

		//Check For Error
		if (status != 0) {
			printf("Error In Threads");
			exit(0);
		}

		thread_number++;
	}

	//Wait For All Threads Done - - - - - - - - - - - - - - - - - - - - - - //

	for (int z = 0; z < MAX_THREADS; z++)
		pthread_join(thread[z], NULL);


	//Print Multiplied Matrix (Result) - - - - - - - - - - - - - - - - - - -//

	printf(" ##### Multiplied first and second Matrixes #####\n\n");

	for (int x = 0; x < R1; x++) {
		for (int y = 0; y < C2; y++) {
			printf("%5d", RESULT_MATRIX[x][y]);
		}
		printf("\n\n");
	}


	//Calculate Total Time Including 3 Soconds Sleep In Each Thread - - - -//

	clock_gettime(CLOCK_MONOTONIC, &finish);
	calculateTimeDifference(&start, &finish, &time_elapsed);
	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, (time_elapsed / 1.0e9));


	//Total Threads Used In Process - - - - - - - - - - - - - - - - - - - -//
	printf(" ---> Used Threads : %d \n\n", thread_number);

	printf("Provided number of rows for first Matrix: %d \n", R1);
	printf("Provided number of columns for first Matrix: %d \n\n", C1);
	printf("Provided number of rows for second Matrix: %d \n", C1);
	printf("Provided number of columns for second Matrix: %d \n\n", C2);

	for (int z = 0; z < thread_number; z++)
		printf(" - Thread %d ID : %d\n", z + 1, (int)thread[z]);
	free(t);
	return 0;
}

