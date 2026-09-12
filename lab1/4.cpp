#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;

const int SIZE = 10000000;

double function() {
  vector<double> a(SIZE);

  for (auto &i : a) {
    i = (double)rand() / RAND_MAX;
  }

  chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
  double sum =0;
  for (int i = 0; i<SIZE; i++) {
    sum += a[i] * a[i] + 3*a[i] +1;
  }

  chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
  chrono::duration<double, std::milli> time_span = end - start;
  cout << "Suma: " << sum << " Tiempo: " << time_span.count() << " ms" << endl;
  return time_span.count();
}

int main() {
  double sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += function();
  }
  cout << "Tiempo promedio: " << sum / 10 << " ms" << endl;
}
