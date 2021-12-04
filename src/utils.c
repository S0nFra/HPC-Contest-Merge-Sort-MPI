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