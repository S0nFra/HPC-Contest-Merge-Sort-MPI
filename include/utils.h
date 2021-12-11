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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>  // to check correctness of input
#include <limits.h> // for INT_MIN and INT_MAX

int check_int_input(const char* par);

#endif /* FDF65B0C_B221_4A62_8ABF_4B3AA35CA7F1 */
