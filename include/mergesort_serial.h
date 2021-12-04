#ifndef F15EE0A8_AB33_42A0_AAAF_C6B4A6C041DC
#define F15EE0A8_AB33_42A0_AAAF_C6B4A6C041DC

#include "datatype.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // for INT_MIN and INT_MAX
#include <time.h> // for clock()

// macros for measuring time
#define START_T(start)  start = clock()
#define STOP_T(t)  t = (clock() - t)/CLOCKS_PER_SEC

/**
 * @brief merge function of Merge Sort.
 * <a href="https://github.com/dreamcrash/StackOverflow-/blob/main/OpenMP/MergeSort/main.c">Code reference</a> 
 * 
 * @param X Array to merge
 * @param n Size of the array
 * @param tmp Support array
 */
void merge_rec(DATATYPE* restrict X, int n, DATATYPE* restrict tmp);

/**
 * @brief Helper function of recursive serial Merge Sort.
 * 
 * @param X Array to sort
 * @param n Size of the array
 * @param tmp Support array
 */
void mergesort_rec_h(DATATYPE* restrict X, int n, DATATYPE* restrict tmp);

/**
 * @brief Main function of recursive serial Merge Sort.
 * 
 * @param X Array to sort
 * @param n Size of the array
 */
void mergesort_rec(DATATYPE* restrict X, int n);

int read_file(char* filename, DATATYPE* array, int arraySize);

void printArray(DATATYPE *array, int arraySize);

#endif /* F15EE0A8_AB33_42A0_AAAF_C6B4A6C041DC */
