/**
 * @file utils.h
 * @author Mario Pellegrino
 * @author Francesco Sonnessa
 * @brief Function prototypes for utility functions
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#ifndef FDF65B0C_B221_4A62_8ABF_4B3AA35CA7F1
#define FDF65B0C_B221_4A62_8ABF_4B3AA35CA7F1

#include "datatype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  // to check correctness of input
#include <limits.h> // for INT_MIN and INT_MAX

int check_int_input(const char* par);

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


#endif /* FDF65B0C_B221_4A62_8ABF_4B3AA35CA7F1 */
