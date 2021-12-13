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
