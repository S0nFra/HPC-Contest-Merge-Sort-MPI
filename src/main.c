/*
 * File: mpi_merge_sort.c
 * Purpose: Implement parallel merge sort on a distributed list
 * of randomly generated ints
 * Input:
 * n: number of elements per process
 * Output:
 * local_array: elements of distributed list after sorting
 *
 * Compile: mpicc -g -Wall -o mpi_merge_sort mpi_merge_sort.c
 * Run:
 * mpiexec -n <p> ./mpi_merge_sort
 * - p: the number of processes
 *
 * Notes:
 * 1. Uses tree-structured communication to gather the distributed
 * lists onto process 0.
 * 2. No attempt is made to be efficient with storage.
 * 3. Assumes p is a power of 2
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  // to check correctness of input
#include <limits.h> // for INT_MIN and INT_MAX
#include <mpi.h>

#define MPITYPE MPI_INT
typedef int DATATYPE; // char, ints, unsigned, floats, doubles supported

int check_int_input(const char* par);

void QuickSort(DATATYPE a[], int lo, int hi);
void swap(DATATYPE* a,DATATYPE* b);

/* Local functions */
double init(DATATYPE *local_array, int local_size, int n_rank, int rank, char* filename, int version, MPI_Comm com);
double init_local_sort(DATATYPE* local_array, int local_size, int n_rank, int rank, MPI_Comm com); 


int Compare(const void * a_p, const void * b_p);
void Print_list(DATATYPE local_array[], int n);
void Merge(DATATYPE local_array[], DATATYPE B[], DATATYPE C[], int size);

/* Functions involving communication */
void Merge_sort(DATATYPE local_array[], int local_n, int my_rank, int p, MPI_Comm comm);
void Print_global_list(DATATYPE local_array[], int local_n, int my_rank, int p, MPI_Comm comm);

/*-------------------------------------------------------------------*/
int main(int argc, char * argv[]) {

  int rank, n_rank;
  DATATYPE *local_array;
  int local_size;
  MPI_Comm comm;

  double init_time_mean, init_local_time_mean;

  MPI_Init(&argc, &argv);
  comm = MPI_COMM_WORLD;
  MPI_Comm_size(comm, &n_rank);
  MPI_Comm_rank(comm, &rank);

  if (argc < 4){
    if(rank == 0)
		  fprintf(stderr,"Usage:\n\t%s [input_fileName] [inputSize] [VERSION] [testMode (default = 0)]\n",argv[0]);
		exit(EXIT_FAILURE);
  }

  char* filename = argv[1];
  int size = check_int_input(argv[2]);
  int VERSION = check_int_input(argv[3]);
  int testMode = (argc == 5) ? check_int_input(argv[4]) : 0;

  local_size = size / n_rank;
  local_array = malloc(n_rank * local_size * sizeof(DATATYPE));

  init_time_mean = init(local_array, local_size, n_rank, rank, filename, VERSION, comm);

  if(testMode && rank == 0)
  if (rank == 0)
    printf("init time taken: %.3lf\n", init_time_mean);

  init_local_time_mean = init_local_sort(local_array, local_size, n_rank, rank, comm);

  if(testMode && rank == 0)
  if (rank == 0)
    printf("init local sort time taken: %.3lf\n", init_local_time_mean);

  if(testMode)
    Print_global_list(local_array, local_size, rank, n_rank, comm);

  Merge_sort(local_array, local_size, rank, n_rank, comm);

  if(testMode && rank == 0)
    Print_list(local_array, n_rank * local_size);

  // printf(); // output

  free(local_array);
  MPI_Finalize();
  return 0;
} /* main */

double init(DATATYPE *local_array, int local_size, int n_rank, int rank, char* filename, int version, MPI_Comm com) {

  MPI_Status status;
  MPI_File fh;

  double start = MPI_Wtime();
  if (version == 1){
    MPI_Offset offset = rank * local_size * sizeof(DATATYPE);
    MPI_File_open(com, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    for (int i=0; i<n_rank; i++){
      MPI_File_seek(fh, offset, MPI_SEEK_SET);
      MPI_File_read_all(fh, local_array, local_size, MPITYPE, &status);
    }
    MPI_File_close(&fh);
  }
  else if (version == 2){
    MPI_Datatype array_integer_type;
    MPI_Type_contiguous(local_size, MPITYPE , &array_integer_type);
    MPI_Type_commit(&array_integer_type);

    int displacement = rank * local_size * sizeof(DATATYPE);

    MPI_File_open(com, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);  
    MPI_File_set_view(fh, displacement, MPITYPE, array_integer_type, "native", MPI_INFO_NULL);
    MPI_File_read_all(fh, local_array, local_size, MPITYPE, &status);
    MPI_File_close(&fh);
  }
  double end = MPI_Wtime() - start;

  // printf("%d# local_time: %.3lf\n",rank, end);
  MPI_Barrier(com);

  double sum = 0;
  MPI_Reduce(&end, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, com);

  // printf("In %d#\n",rank);
  // for (int i=0; i<local_size; i++)
  //   printf("%d ", local_array[i]);
  // printf("\n");

  return sum/n_rank;
}

double init_local_sort(DATATYPE* local_array, int local_size, int n_rank, int rank, MPI_Comm com){
  
  double start = MPI_Wtime();
    //qsort(local_array, local_size, sizeof(local_array[0]), Compare);
    QuickSort(local_array,0,local_size-1);
  double end = MPI_Wtime() - start;

  // printf("%d# %.3lf\n",rank, end);
  MPI_Barrier(com);

  double sum = 0;
  MPI_Reduce(&end, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, com);

  return sum / n_rank;
}

/*-------------------------------------------------------------------
 * Function: Print_global_list
 * Purpose: Print the contents of a distributed list
 * Input args:
 * local_array: the local list
 * local_n: the number of elements in each local list
 * my_rank, p, comm: the usual
 */
void Print_global_list(DATATYPE local_array[], int local_n, int my_rank, int p, MPI_Comm comm) {
  DATATYPE * global_A = NULL;

  if (my_rank == 0) {
    global_A = malloc(p * local_n * sizeof(DATATYPE));
    MPI_Gather(local_array, local_n, MPITYPE, global_A, local_n, MPITYPE, 0, comm);
    Print_list(global_A, p * local_n);
    free(global_A);
  } else {
    MPI_Gather(local_array, local_n, MPITYPE, global_A, local_n, MPITYPE, 0, comm);
  }
} /* Print_global_list */

/*-------------------------------------------------------------------
 * Function: Compare
 * Purpose: Compare 2 ints, return -1, 0, or 1, respectively, when
 * the first int is less than, equal, or greater than
 * the second. Used by qsort.
 */
int Compare(const void * a_p, const void * b_p) {
  int a = * ((DATATYPE * ) a_p);
  int b = * ((DATATYPE * ) b_p);

  if (a < b)
    return -1;
  else if (a == b)
    return 0;
  else /* a > b */
    return 1;
} /* Compare */

/*-------------------------------------------------------------------
 * Function: Print_list
 * Purpose: Print a list of ints to stdout
 * In args:
 * local_array: the list
 * n: the number of elements in the list
 */
void Print_list(DATATYPE local_array[], int n) {
  int i;
  for (i = 0; i < n; i++)
    printf("%.2lf ", (double)local_array[i]);
  printf("\n");
} /* Print_list */

/*-------------------------------------------------------------------
 * Function: Merge_sort
 * Purpose: Parallel merge sort: starts with a distributed
 * collection of sorted lists, ends with a global
 * sorted list on process 0. Uses tree-structured
 * communication.
 * In args: local_n: number of elements contributed by each process
 * my_rank, p, comm: the usual
 *
 * In/out arg: local_array: each process's local sorted list on input
 * global sorted list on process 0 on output
 */
void Merge_sort(DATATYPE local_array[], int local_n, int my_rank, int p, MPI_Comm comm) {
  int partner, done = 0, size = local_n;
  unsigned bitmask = 1;
  DATATYPE * B, * C;
  MPI_Status status;

  B = malloc(p * local_n * sizeof(DATATYPE));
  C = malloc(p * local_n * sizeof(DATATYPE));

  while (!done && bitmask < p) {
    partner = my_rank ^ bitmask;
    if (my_rank > partner) {
      MPI_Send(local_array, size, MPITYPE, partner, 0, comm);
      done = 1;
    } else {
      MPI_Recv(B, size, MPITYPE, partner, 0, comm, & status);
      Merge(local_array, B, C, size);
      size = 2 * size;
      bitmask <<= 1;
    }
  }

  free(B);
  free(C);
} /* Merge_sort */

/*-------------------------------------------------------------------
 * Function: Merge
 * Purpose: Merge two sorted lists, local_array, B. Return result in local_array.
 * C is used for scratch. Both local_array and B have size elements.
 * In args: B: second input array
 * size: number of elements in local_array and B
 * In/out arg: local_array: first input array, output array
 * Scratch: C: temporary storage for merged lists
 */
void Merge(DATATYPE local_array[], DATATYPE B[], DATATYPE C[], int size) {
  int ai, bi, ci;

  ai = bi = ci = 0;
  while (ai < size && bi < size)
    if (local_array[ai] <= B[bi]) {
      C[ci] = local_array[ai];
      ci++;
      ai++;
    } else {
      C[ci] = B[bi];
      ci++;
      bi++;
    }

  if (ai >= size)
    for (; ci < 2 * size; ci++, bi++)
      C[ci] = B[bi];
  else
    for (; ci < 2 * size; ci++, ai++)
      C[ci] = local_array[ai];
      
  memcpy(local_array, C, 2 * size * sizeof(DATATYPE));
} /* Merge */


int check_int_input(const char* par){
    /**
     * @brief 
     * from https://stackoverflow.com/questions/9748393/how-can-i-get-argv-as-int
     *  answer of https://stackoverflow.com/users/1201863/luc
     */
    char* p;
    errno = 0; /*not 'int errno', because the '#include' already defined it*/
    long arg = strtol(par, &p, 10);
    if (*p != '\0' || errno != 0) {
        fprintf(stderr,"not valid argument");
        exit(EXIT_FAILURE); 
    }
    if (arg < INT_MIN || arg > INT_MAX) { /*if exceed the representation of int*/
        fprintf(stderr,"argument exceed representation");
        exit(EXIT_FAILURE);
    }
    int val = arg; /*#val now contains the size to be read*/
    if(val < 0){
        fprintf(stderr,"negative values not allowed");
        exit(EXIT_FAILURE);
    }
    return val;
}


/*QuickSort using median of low, middle, high, values as pivot*/
void QuickSort(DATATYPE a[], int lo, int hi) {
    int i = lo, j = (lo + hi)/2, k = hi;
    int pivot;
    if (a[k] < a[i])            // median of 3
        swap(&a[k], &a[i]);
    if (a[j] < a[i])
        swap(&a[j], &a[i]);
    if (a[k] < a[j])
        swap(&a[k], &a[j]);
    pivot = a[j];
    while (i <= k) {            // partition
        while (a[i] < pivot)
            i++;
        while (a[k] > pivot)
            k--;
        if (i <= k) {
            swap(&a[i], &a[k]);
            i++;
            k--;
        }
    }
    if (lo < k)                 // recurse
        QuickSort(a, lo, k);
    if (i < hi)
        QuickSort(a, i, hi);
}

void swap(DATATYPE* a,DATATYPE* b){
  DATATYPE tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}
