/**
 * @file utils.c
 * @author Mario Pellegrino
 * @author Francesco Sonnessa
 * @brief Utility functions
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "../include/utils.h"

/**
 * @brief Check if the string parameter
 *  is a valid non-negative integer and convert it. 
 * If is invalid exit with failure
 * @param par string to be checked and converted
 * @return int The integer converted from the string
 */
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