/* Parallelizable Dot Product Benchmark
 * 
 * A.B=A0*B0+A1*B1+A2*B2+A3*B3
 * 
 * 
 */

#define ARRAY_SIZE 6000
#define NUMS_OF_ACCEL 8
#define OPS_PER_ACCEL ARRAY_SIZE/NUMS_OF_ACCEL

#include <stdio.h>
#include "dotproduct.h"

int result_array[NUMS_OF_ACCEL];

// Calculate the values for an entire row
int product (int * inputA, int * inputB, int startidx, int maxidx) 
{
   int i,j;
   int result = 0;
   for (i = 0; i < OPS_PER_ACCEL; i++){
      result+= inputA[i]*inputB[i];
   }
   return result;
}

int main(){
   int result=0;
   int i;

#pragma omp simd
   for (i=0; i<NUMS_OF_ACCEL; i++) {
      result_array[i] = product (A_array, B_array, i*OPS_PER_ACCEL, (i+1)*OPS_PER_ACCEL);
   }

   //combining the results
   for (i=0; i<NUMS_OF_ACCEL; i++) {
      result += result_array[i];
   }

   //check final result
   printf ("Result: %d\n", result);
   if (result == 60604032) {
      printf("RESULT: PASS\n");
      return 0;
   } else {
      printf("RESULT: FAIL\n");
      return 1;
   }
}
