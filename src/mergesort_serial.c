/**
 * @file mergesort_serial.c
 * @author Mario Pellegrino
 * @author Francesco Sonnessa
 * @brief Serial implementation of Mergesort algorithm
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
            size_t ret = fread(array, sizeof(DATATYPE), arraySize, fp);
            if(ret != arraySize){ // elements read not equal to size of the buffer
               char* quantity = (ret > arraySize) ? "more" : "less";
               fprintf(stderr,"read %s elements than expected", quantity);
               fclose(fp); // close the file!
               return -1; // FAIL 
            }
         }
         fclose(fp); //close the file!
         return 1; //success 
      }
   }
   return -1; //failed to execute
}