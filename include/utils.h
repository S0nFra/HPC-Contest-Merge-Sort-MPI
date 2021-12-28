/**
 * @file utils.h
 * @author Mario Pellegrino
 * @author Francesco Sonnessa
 * @brief Function prototypes for utility functions
 * @version 0.1
 * 
 * @copyright Copyright (c) 2021
 */
/** 
 * Course: High Performance Computing 2021/2022
 *
 * Lecturer: Francesco Moscato    fmoscato@unisa.it
 *
 * Group:
 * Mario Pellegrino    0622701671  m.pellegrino42@studenti.unisa.it
 * Francesco Sonnessa   0622701672   f.sonnessa@studenti.unisa.it
 *
 * Copyright (C) 2021 - All Rights Reserved 
 *
 * This file is part of Contest - MPI.
 *
 * Contest - MPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Contest - MPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Contest - MPI.  If not, see <http://www.gnu.org/licenses/>. 
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
