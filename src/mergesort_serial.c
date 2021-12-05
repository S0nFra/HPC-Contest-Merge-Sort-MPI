#include "../include/mergesort_serial.h"
#include "../include/utils.h"

int main(int argc, char const *argv[]){

   if (argc < 3){
      fprintf(stderr,"Usage: %s [filename] [input_size] [testMode (default = 0)]\n",argv[0]);
      exit(EXIT_FAILURE);
   }

   char* filename = (char*) argv[1];
   int size = check_int_input(argv[2]);
   int testMode = (argc == 4) ? check_int_input(argv[3]) : 0;

   if (testMode) printf("args: %s %d %d\n",filename, size, testMode);
    
   DATATYPE *input = malloc(size * sizeof(DATATYPE));

   double read_timer = 0;
   double read_merge = 0;
   int return_status;
    
   START_T(read_timer);
      return_status =  read_file(filename, input, size);
   STOP_T(read_timer);

   if (testMode){
      printf(">> PRIMA\n");
      printArray(input, size);
   }

   if(return_status){
      START_T(read_merge);
         mergesort_rec(input, size);
      STOP_T(read_merge);
      
      if (testMode){
         printf(">> DOPO\n");
         printArray(input, size);
      }
      printf("%d;0;%lf;%lf",size,read_timer,read_merge);
   }else{
      fprintf(stderr,"can't read %s", argv[1]);
   }

   free(input);
    
   return EXIT_SUCCESS;
}

void printArray(DATATYPE *array, int arraySize){
   printf("\n");
   for (int i=0; i<arraySize; i++){
      printf("%.3lf ", (double) array[i]);
   }
   printf("\n");
}

int read_file(char* filename, DATATYPE* array, int arraySize){
   FILE* fp;

   if(filename != NULL){
      fp = fopen(filename,"rb");
      if(fp != NULL){ //successfully opened the file
         if(array != NULL){ // destination array is available
              fread(array, sizeof(DATATYPE), arraySize, fp);
         }
         fclose(fp); //close the file!
         return 1; //success 
      }
   }
   return -1; //failed to execute
}

void merge_rec(DATATYPE* restrict X, int n, DATATYPE* restrict tmp) {
   int i = 0;
   int j = n/2;
   int ti = 0;

   while (i<n/2 && j<n) {
      if (X[i] < X[j]) {
         tmp[ti] = X[i];
         ti++; i++;
      } else {
         tmp[ti] = X[j];
         ti++; j++;
      }
   }
   while (i<n/2) { /* finish up lower half */
      tmp[ti] = X[i];
      ti++; i++;
   }
   while (j<n) { /* finish up upper half */
      tmp[ti] = X[j];
      ti++; j++;
   }
   memcpy(X, tmp, n*sizeof(DATATYPE));
}


void mergesort_rec(DATATYPE* restrict X, int n){

   DATATYPE* restrict tmp = malloc(n * sizeof(DATATYPE));

   mergesort_rec_h(X,n,tmp);
   free(tmp);
}

// Seriale
void mergesort_rec_h(DATATYPE* restrict X, int n, DATATYPE* restrict tmp){
   if (n < 2) return;

   mergesort_rec_h(X, n/2, tmp);
   mergesort_rec_h(X+(n/2), n-(n/2), tmp + n/2);

   merge_rec(X, n, tmp);
}