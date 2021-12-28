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
#ifndef FC3F2AA5_0F53_437C_BCCF_B452F14760FC
#define FC3F2AA5_0F53_437C_BCCF_B452F14760FC

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#include "datatype.h"
#include "utils.h"

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
