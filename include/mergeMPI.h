/**
 * @file mergeMPI.h
 * @author Mario Pellegrino
 * @author Francesco Sonnessa
 * @brief Function prototypes for parallel mergesort
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef FC3F2AA5_0F53_437C_BCCF_B452F14760FC
#define FC3F2AA5_0F53_437C_BCCF_B452F14760FC

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "datatype.h"

// shall be changed accordingly
// for example: (MPI_INT, int) or (MPI_DOUBLE, double)
#define MPITYPE MPI_INT



/* sort function and helpers */
void quickSort(DATATYPE* a, int lo, int hi);
int compare(const void* a_p, const void* b_p);

/* Local functions */
double init(DATATYPE* local_array, int local_size, int n_rank, int rank, char* filename, int version, MPI_Comm com);
double init_local_sort(DATATYPE* local_array, int local_size, int n_rank, int rank, MPI_Comm com); 
void Print_list(DATATYPE* local_array, int n);
void Print_list_node(DATATYPE local_array[], int n, int local_size);
void Merge(DATATYPE* local_array, DATATYPE* B, DATATYPE* C, int size);

/* Functions involving communication */
void Merge_sort(DATATYPE* local_array, int local_n, int my_rank, int p, MPI_Comm comm);
void Print_global_list(DATATYPE* local_array, int local_n, int my_rank, int p, MPI_Comm comm);


// Macro for measuring MPI execution time 
#define START_T(X) X = MPI_Wtime();

// execute the sum of the times taken by each process
#define END_T(end,start,comm,sum) do { \
    end = MPI_Wtime() - start; \
    MPI_Barrier(comm); \
    sum = 0;  \
    MPI_Reduce(&end, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, comm); \
}while(0);

#endif /* FC3F2AA5_0F53_437C_BCCF_B452F14760FC */
