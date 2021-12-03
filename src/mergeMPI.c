#include "mergeMPI.h"

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


  if (testMode && rank == 0)
    printf("init local sort time taken: %.3lf\n", init_local_time_mean);

  if(testMode)
    Print_global_list(local_array, local_size, rank, n_rank, comm);

  Merge_sort(local_array, local_size, rank, n_rank, comm);

  if(testMode && rank == 0)
    Print_list(local_array, size);

  free(local_array);
  MPI_Finalize();
  return 0;
}

/**
 * @brief Read the input data to be sorted from file and stores the result in memory.
 * The time taken to read the file and fill the memory is returned;
 * 
 * @param local_array the array to be filled with data
 * @param local_size  the size of the array
 * @param n_rank the size of the communicator
 * @param rank the rank of node in the communicator
 * @param filename name of the file to be read 
 * @param version changes the read mode
 * @param com the MPI communicator involved 
 * @return double the time spent to read the file and store the data in memory
 */

double init(DATATYPE *local_array, int local_size, int n_rank, int rank, char* filename, int version, MPI_Comm com) {

  MPI_Status status;
  MPI_File fh;

  double start,end,sum;

  //start counting time
  START_T(start)

  if (version == 1){ // many collective, contigous requests
    MPI_Offset offset = rank * local_size * sizeof(DATATYPE);
    MPI_File_open(com, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    for (int i=0; i<n_rank; i++){
      MPI_File_seek(fh, offset, MPI_SEEK_SET);
      MPI_File_read_all(fh, local_array, local_size, MPITYPE, &status);
    }
    MPI_File_close(&fh);
  }
  else if (version == 2){ // single collective, noncontigous request
    MPI_Datatype array_integer_type;
    MPI_Type_contiguous(local_size, MPITYPE , &array_integer_type);
    MPI_Type_commit(&array_integer_type);

    int displacement = rank * local_size * sizeof(DATATYPE);

    MPI_File_open(com, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);  
    MPI_File_set_view(fh, displacement, MPITYPE, array_integer_type, "native", MPI_INFO_NULL);
    MPI_File_read_all(fh, local_array, local_size, MPITYPE, &status);
    MPI_File_close(&fh);
  }
  //stop the timer
  END_T(end,start,com,sum)

  return sum/n_rank; //return the mean of the time spent in this function by each node in the communicator
}

double init_local_sort(DATATYPE* local_array, int local_size, int n_rank, int rank, MPI_Comm com){
  
  double start_time,end_time,sum;

  START_T(start_time)
    //qsort(local_array, local_size, sizeof(local_array[0]), Compare);
    quickSort(local_array,0,local_size-1); //sort the local array 
  END_T(end_time,start_time,com,sum)

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
void Print_global_list(DATATYPE* local_array, int local_n, int my_rank, int p, MPI_Comm comm) {
  DATATYPE* global_A = NULL;

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
 * Purpose: Compare 2 DATATYPES, return -1, 0, or 1, respectively, when
 * the first int is less than, equal, or greater than
 * the second. Used by qsort
 */
int compare(const void* a_p, const void* b_p) {
  int a = * ((DATATYPE* ) a_p);
  int b = * ((DATATYPE* ) b_p);

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
void Merge_sort(DATATYPE* local_array, int local_n, int my_rank, int p, MPI_Comm comm) {
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
void Merge(DATATYPE* local_array, DATATYPE* B, DATATYPE* C, int size) {
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
     * from https://stackoverflow.com/questions/9748393/how-can-i-get-argv-as-int
     * answer of https://stackoverflow.com/users/1201863/luc
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
void quickSort(DATATYPE a[], int lo, int hi) {
    int i = lo, j = (lo + hi)/2, k = hi;
    int pivot;
    if (a[k] < a[i]) swap(&a[k], &a[i]);
    if (a[j] < a[i]) swap(&a[j], &a[i]);
    if (a[k] < a[j]) swap(&a[k], &a[j]);
    pivot = a[j];
    while (i <= k) {            // partition
        while (a[i] < pivot) i++;
        while (a[k] > pivot) k--;
        if (i <= k) {
            swap(&a[i], &a[k]);
            i++;
            k--;
        }
    }
    if (lo < k)   // recurse in the lower half
        quickSort(a, lo, k);
    if (i < hi)
        quickSort(a, i, hi); // recurse in the higher half
}

void swap(DATATYPE* a,DATATYPE* b){
  DATATYPE tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}
