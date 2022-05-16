/*
 * Filename	: ThreadSum.c
 * Author	: Evan Hanzelman
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/******************************************************************/
//globals
int* A; 
int N; 
int P; 

/******************************************************************/
void*
thread (void* id);

long
parallelSumCalc();

long
serialSumCalc();

/******************************************************************/

int
main (int argc, char* argv [])
{
	printf ("p ==> ");
	scanf ("%d", &P);
	printf ("N ==> ");
	scanf ("%d", &N);

	A = malloc (N * sizeof (int));

	for (int i = 0; i < N; i++)
	{
		A [i] = rand () % 5; 
	}

	printf ("Parallel Sum:  %ld\n", parallelSumCalc ());
	printf ("Serial Sum:    %ld\n", serialSumCalc ());

	free (A);
}

/******************************************************************/

void*
thread (void* id)
{
	long t_block = N / P;
	long t_start = t_block * (long) id;
	long end = t_start + t_block;
	
	long t_sum = 0;

	for (long i = t_start; i < end; i++)
	{
		t_sum += A [i];;
	}

	return (void*) t_sum;
}

/******************************************************************/

long
parallelSumCalc ()
{
	pthread_t* threads = malloc (P * sizeof (pthread_t));

	for(long id = 0; id < P; ++id)
	{
		pthread_create (&threads [id], NULL, thread, (void*) id);
	}

	long sum = 0;

	for(long id = 0; id < P; ++id)
	{
		void* returnVal;
		pthread_join (threads [id], &returnVal);
		sum += (long) returnVal;
	}

	free (threads);

	return sum;
}

/******************************************************************/

long
serialSumCalc ()
{
	long serial_sum = 0;

	for (int i = 0; i < N; ++i)
	{
		serial_sum += A [i];
	}

	return serial_sum;
}
