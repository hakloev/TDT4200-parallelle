#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

/*
	A simple MPI example.
	TODO:
	1. Fill in the needed MPI code to make this run on any number of nodes.
	2. The answer must match the original serial version.
	3. Think of corner cases (valid but tricky values).

	Example input:
	./simple 2 10000

*/

#define MASTER 0 /* Declaring a constant for identifying the master process */
#define RETURN_DATA_TAG 4001

double compute_part(int local_start, int local_end) {
	double sum = 0.0;
	for (int i = local_start; i < local_end; i++)
		sum += 1.0 / log(i);
	return sum;
}

int main(int argc, char **argv) {
	int     my_rank; /* My process rank */
	int     size; /* The number of processes */ 
	int 	range_per_process; /* The range to computate per process */
	int 	local_start, local_end; /* Start and end for the local computations */
	double 	part_sum; /* Used as a receive buffer for MPI_Send and MPI_Recv */
	double 	total = 0.0; /* The total sum of all computations */
	double 	local_start_time, local_end_time, local_elapsed_time, elapsed_time;
	MPI_Status status; 

    /* Give the system the initation for MPI */ 
	MPI_Init(&argc, &argv);
    
    /* Get the my process rank */
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Get the number of processes in use by the system */
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	/* From this point, all processes run a instance of the program */

	if (argc < 3) {
		printf("This program requires two parameters:\n \
the start and end specifying a range of positive integers in which \
start is 2 or greater, and end is greater than start.\n");
		exit(1);
	}

	int start = atoi(argv[1]);
	int stop = atoi(argv[2]);

	if(start < 2 || stop <= start){
		printf("Start must be greater than 2 and the end must be larger than start.\n");
		exit(1);
	}

	MPI_Barrier(MPI_COMM_WORLD); /* Synchronize processes */
	local_start_time = MPI_Wtime();

	// TODO: Compute the local range, so that all the elements are accounted for.
	range_per_process = ceil(((double)(stop - start) / size)); /* Calculate the ranger per process */
	local_start = (start + my_rank * range_per_process); /* Set local start */
	local_end = local_start + range_per_process; /* Set local end */

	// If (stop - start) % size != 0 we have to change the local_end for the last process
	local_end = (local_end > stop) ? stop : local_end;
	part_sum = compute_part(local_start, local_end); 
	
	MPI_Barrier(MPI_COMM_WORLD);
	local_end_time = MPI_Wtime();
	local_elapsed_time = local_end_time - local_start_time;
	MPI_Reduce(&local_elapsed_time, &elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	// Debug prints if needed
    
	// TODO: Get the sum of each node's partial answer
	// Use MPI_Send and MPI_Recv here
	// Could also use MPI_Reduce and achieve the same result
   	if (my_rank != MASTER) {
		//printf("Sending part_sum (%f) from process %d\n", part_sum, my_rank);
		MPI_Send(&part_sum, 1, MPI_DOUBLE, MASTER, RETURN_DATA_TAG, MPI_COMM_WORLD);
    } else {  // This is the master process, so here we recieve all the local computations
		total = part_sum;
		for (int i = 1; i < size; i++) {
			MPI_Recv(&part_sum, 1, MPI_DOUBLE, i, RETURN_DATA_TAG, MPI_COMM_WORLD, &status);
			total = total + part_sum;
			//printf("Received part_sum (%f) from process %d\n", part_sum, i);
        }
	}
    
	// TODO: Print the global sum once only
	// We only want to print if it is the master process
	if (my_rank == MASTER) {
		//printf("The sum is: %f\n", total);
		//printf("Elapsed time: %f seconds\n", elapsed_time);
		printf("%f\n", total);
	}

    MPI_Finalize();
	return 0;
}

