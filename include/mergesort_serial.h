/**
 * @file mergesort_serial.h
 * @author Mario Pellegrino
 * @author Francesco Sonnessa
 * @brief Function prototypes for serial mergesort
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
#ifndef F15EE0A8_AB33_42A0_AAAF_C6B4A6C041DC
#define F15EE0A8_AB33_42A0_AAAF_C6B4A6C041DC

#include "datatype.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // for INT_MIN and INT_MAX
#include <time.h> // for clock()

// macros for measuring time
#define START_T(start)  start = clock()
#define STOP_T(t)  t = (clock() - t)/CLOCKS_PER_SEC

int read_file(char* filename, DATATYPE* array, int arraySize);

void printArray(DATATYPE *array, int arraySize);

#endif /* F15EE0A8_AB33_42A0_AAAF_C6B4A6C041DC */
