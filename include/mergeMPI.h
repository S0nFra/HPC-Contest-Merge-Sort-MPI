#ifndef FC3F2AA5_0F53_437C_BCCF_B452F14760FC
#define FC3F2AA5_0F53_437C_BCCF_B452F14760FC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  // to check correctness of input
#include <limits.h> // for INT_MIN and INT_MAX
#include <mpi.h>


// shall be changed accordingly
// for example: (MPI_INT, int) or (MPI_DOUBLE, double)
#define MPITYPE MPI_INT
typedef int DATATYPE; // char, ints, unsigned, floats, doubles supported

int check_int_input(const char* par);

/* sort function and helpers */
void quickSort(DATATYPE* a, int lo, int hi);
void swap(DATATYPE* a,DATATYPE* b);
int compare(const void* a_p, const void* b_p);

/* Local functions */
double init(DATATYPE* local_array, int local_size, int n_rank, int rank, char* filename, int version, MPI_Comm com);
double init_local_sort(DATATYPE* local_array, int local_size, int n_rank, int rank, MPI_Comm com); 
void Print_list(DATATYPE* local_array, int n);
void Merge(DATATYPE* local_array, DATATYPE* B, DATATYPE* C, int size);

/* Functions involving communication */
void Merge_sort(DATATYPE* local_array, int local_n, int my_rank, int p, MPI_Comm comm);
void Print_global_list(DATATYPE* local_array, int local_n, int my_rank, int p, MPI_Comm comm);


// Macro for measuring MPI execution time 
#define START_T(X) X = MPI_Wtime();
#define END_T(end,start,comm,sum) do { \
    end = MPI_Wtime() - start; \
    MPI_Barrier(comm); \
    sum = 0;  \
    MPI_Reduce(&end, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, comm); \
}while(0);

#endif /* FC3F2AA5_0F53_437C_BCCF_B452F14760FC */
