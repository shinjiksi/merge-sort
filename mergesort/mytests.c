#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h> /* for times system call */
#include <sys/time.h>  /* for gettimeofday system call */
#include <unistd.h>
#include <error.h>
#include "lab.h"

// function prototypes
int check_if_sorted(int A[], int n);
void generate_random_array(int A[], int n, int seed);
void serial_mergesort(int A[], int p, int r);
void merge(int A[], int p, int q, int r);
void insertion_sort(int A[], int p, int r);
double getMilliSeconds(void);
//added function
void parallel_mergesort(int A[], int p, int r, int threadCount);

/*
---------------------------------------------------------------------------
clock_t times(struct tms *buffer);
times() fills the structure pointed to by buffer with
time-accounting information.  The structure defined in
<sys/times.h> is as follows:
struct tms {
    clock_t tms_utime;       user time
    clock_t tms_stime;       system time
    clock_t tms_cutime;      user time, children
    clock_t tms_cstime;      system time, children
The time is given in units of 1/CLK_TCK seconds where the
value of CLK_TCK can be determined using the sysconf() function
with the agrgument _SC_CLK_TCK.
---------------------------------------------------------------------------
*/


float report_cpu_time(void)
{
	struct tms buffer;
	float cputime;

	times(&buffer);
	cputime = (buffer.tms_utime)/ (float) sysconf(_SC_CLK_TCK);
	return (cputime);
}


float report_sys_time(void)
{
	struct tms buffer;
	float systime;

	times(&buffer);
	systime = (buffer.tms_stime)/ (float) sysconf(_SC_CLK_TCK);
	return (systime);
}

double getMilliSeconds(void)
{
    struct timeval now;
    gettimeofday(&now, (struct timezone *)0);
    return (double) now.tv_sec*1000.0 + now.tv_usec/1000.0;
}


/*
 * generate_random_array(int A[], int n, int seed):
 *
 * description: Generate random integers in the range [0,RANGE]
 *              and store in A[1..n]
 */

#define RANGE 1000000

void generate_random_array(int A[], int n, int seed)
{
    int i;

	srandom(seed);
    for (i=1; i<=n; i++)
        A[i] = random()%RANGE;
}


/*
 * check_if_sorted(int A[], int n):
 *
 * description: returns TRUE if A[1..n] are sorted in nondecreasing order
 *              otherwise returns FALSE
 */

int check_if_sorted(int A[], int n)
{
	int i=0;

	for (i=1; i<n; i++) {
		if (A[i] > A[i+1]) {
			return FALSE;
		}
	}
	return TRUE;
}




int main(int argc, char **argv) {

//be 3, def 2
	if (argc < 3) { // there must be at least one command-line argument
			fprintf(stderr, "Usage: %s <input size> <num of threads> [<seed>] \n", argv[0]);
			exit(1);
	}

	int n = atoi(argv[1]);
//added
int numOfThread = atoi(argv[2]);

	int seed = 1;
	if (argc == 4)
		seed = atoi(argv[3]);

	int *A = (int *) malloc(sizeof(int) * (n+1)); // n+1 since we are using A[1]..A[n]

	// generate random input

	generate_random_array(A,n, seed);

	double start_time;
	double sorting_time;

	// sort the input (and time it)
	start_time = getMilliSeconds();

	//serial_mergesort(A,1,n);

	//added
	parallel_mergesort(A, 1, n, numOfThread);

	sorting_time = getMilliSeconds() - start_time;


	// print results if correctly sorted otherwise cry foul and exit
	if (check_if_sorted(A,n)) {
		//for test results only
		//printf("%4.2lf \n",   sorting_time/1000.0);
		
		printf("Sorting %d elements took %4.2lf seconds.\n", n,  sorting_time/1000.0);
	} else {
		printf("%s: sorting failed!!!!\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	free(A);

	exit(EXIT_SUCCESS);
}
