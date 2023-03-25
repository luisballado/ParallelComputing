// Program to calculate the sum of array elements by passing to a function 

#include <stdio.h>
float calculateSum(int num[][]);

int main() {
  float result;
  int num[3][2] = {{2, 3, 4},{5, 6, 7}};
  
  // num array is passed to calculateSum()
  result = calculateSum(num); 
  printf("Result = %.2f", result);
  return 0;
}

float calculateSum(int num[][]) {
  float sum = 0;

  for (int i = 0; i <= 2; ++i){
    for (int j = 0; j <= 2; ++j) {
      sum += num[i][j];
    }
  }

  return sum;
}
