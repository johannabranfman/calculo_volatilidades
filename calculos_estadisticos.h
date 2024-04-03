#include <bits/stdc++.h>
using namespace std;

double devolver_desvio_estandar(float arr[]) {
  int n;
    n=5;


  float sum = 0;
  for (int i = 0; i < n; i++) {
    sum = sum + arr[i];
  }

  double mean = (double)sum / n;

  double sum2 = 0.0;

  for (int i = 0; i < n; i++) {
    sum2 = sum2 + (arr[i] - mean) * (arr[i] - mean);
  }

  double variance = (double)sum2 / n;
  double standardDeviation = sqrt(variance);

  return standardDeviation;
}
