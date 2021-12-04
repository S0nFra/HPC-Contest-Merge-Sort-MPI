#include "../include/mergeMPI.h"
#include "../include/utils.h"

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

  return EXIT_SUCCESS;
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

/**
 * @brief Print the contents of a distributed list 
 * 
 * @param local_array the local list
 * @param local_n the number of elements in the list
 * @param rank rank of the process in the communicator
 * @param n_rank size of communicator
 * @param comm the communicator 
 */
void Print_global_list(DATATYPE* local_array, int local_n, int rank, int n_rank, MPI_Comm comm) {
  DATATYPE* global_A = NULL;

  if (rank == 0) {
    global_A = malloc(n_rank * local_n * sizeof(DATATYPE));
    MPI_Gather(local_array, local_n, MPITYPE, global_A, local_n, MPITYPE, 0, comm);
    Print_list(global_A, n_rank * local_n);
    free(global_A);
  } else {
    MPI_Gather(local_array, local_n, MPITYPE, global_A, local_n, MPITYPE, 0, comm);
  }
}

/**
 * @brief Print a list of DATATYPE to stdout;
 * each element is represented as a double with 
 * two decimal places.
 * 
 * PRE: the DATATYPE shall be a primitive data type 
 * @param local_array the array to be printed
 * @param n the size of the array 
 */
void Print_list(DATATYPE local_array[], int n) {
  int i;
  for (i = 0; i < n; i++)
    printf("%.2lf ", (double)local_array[i]);
  printf("\n");
} 

/**
 * @brief Parallel merge sort: starts with a distributed
 * collection of sorted lists, produces a global sorted list on process
 * with rank 0. Uses tree-structured communication.
 * 
 * @param local_array the sorted array from the process; the global sorted array will be saved here  
 * @param local_n the size of the array
 * @param rank rank of the process
 * @param n_rank size of communicator
 * @param comm the communicator
 */
void Merge_sort(DATATYPE* local_array, int local_n, int rank, int n_rank, MPI_Comm comm) {
  int partner, done = 0, size = local_n;
  unsigned bitmask = 1;
  DATATYPE *B, *C;
  MPI_Status status;

  B = malloc(n_rank * local_n * sizeof(DATATYPE));
  C = malloc(n_rank * local_n * sizeof(DATATYPE));

  while (!done && bitmask < n_rank) {
    partner = rank ^ bitmask;
    if (rank > partner) { // process send to partner
      MPI_Send(local_array, size, MPITYPE, partner, 0, comm);
      done = 1;
    } else { // process receive from partner 
      MPI_Recv(B, size, MPITYPE, partner, 0, comm, & status);
      Merge(local_array, B, C, size);
      size = 2 * size;
      bitmask <<= 1;
    }
  }

  free(B); // No memory leaks!
  free(C);
} 

/**
 * @brief Merge two sorted lists, A and B. Return result in A.
 * C is used for scratch. Both A and B have size elements.
 * 
 * @param A first input array
 * @param B second input array
 * @param C temporary array for merged lists
 * @param size dimen of the three arrays
 */
void Merge(DATATYPE* A, DATATYPE* B, DATATYPE* C, int size) {
  int ai, bi, ci;

  ai = bi = ci = 0;
  while (ai < size && bi < size)
    if (A[ai] <= B[bi]) {
      C[ci] = A[ai];
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
      C[ci] = A[ai];
      
  memcpy(A, C, 2 * size * sizeof(DATATYPE));
} 

/**
 * @brief Implementation of iterative-recursive quicksort. 
 * The recursion is executed only on the shorter array to be 
 * sorted, so the recursive call are reduced and the system stack
 * instead of the heap can be used.
 * 
 * adapted from https://it.wikipedia.org/wiki/Quicksort
 * 
 * @param list the list to be sorted 
 * @param lo lower part of the list to be sorted
 * @param hi higher part of the list to be sorted
 */

void quickSort(DATATYPE* list, int lo, int hi) {
    DATATYPE pivot; DATATYPE tmp;
    
    int  l,r,p;

      while (lo < hi) {   // The while loop replaces the second recursive call
    
        l = lo; p = (lo+hi)/2; r = hi;
        pivot = list[p];

        while (1){
            while ((l<=r) && (compare(&list[l],&pivot) <= 0)) l++;
            while ((l<=r) && (compare(&list[r],&pivot)  > 0)) r--;

            if (l>r) break;
            
            //swap list[l] & list[r]
            DATATYPE tmp = list[l];
            list[l] = list[r];
            list[r] = tmp;

            if (p == r){
              p = l;
            }
            
            l++;
            r--;
        }

        list[p] = list[r];
        list[r] = pivot;
        r--;

        // Select the shorter side of the array & call recursion
        if ((r-lo)<(hi-l)) {
            quickSort(list, lo, r);
            lo=l;
        }
        else {
            quickSort(list, l, hi);
            hi = r;
        }
    }   
}

/**
 * @brief Compare 2 DATATYPES, return -1, 0, or 1,
 *  respectively, when  the first element is 
 * less than, equal, or greater than
 * the second.
 * 
 * @param a_p first element to compare
 * @param b_p second element to compare
 * @return int result of the compare
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
} 