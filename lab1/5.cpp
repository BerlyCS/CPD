#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

#ifndef SIZE
#define SIZE 1000000
#endif
#ifndef THREADS
#define THREADS 1
#endif

double sum_parallel(const vector<double> &a, int start, int end) {
  double sum = 0.0;
  for (int i = start; i < end; i++) {
    sum += a[i] * a[i] + 3 * a[i] + 1;
  }
  return sum;
}

int function() {
  vector<double> a(SIZE);

  for (auto &i : a) {
    i = (double)rand() / RAND_MAX;
  }
  chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();

  thread threads[THREADS];
  vector<double> partials(THREADS, 0.0);

  for (int i = 0; i < THREADS; i++) {
    int start_index = i * (SIZE / THREADS);
    int end_index = (i == THREADS - 1) ? SIZE : (i + 1) * (SIZE / THREADS);
    threads[i] = thread([&a, &partials, i, start_index, end_index]() {
      partials[i] = sum_parallel(a, start_index, end_index);
    });
  }
  double sum = 0.0;
  for (int i = 0; i < THREADS; i++) {
    threads[i].join();
    sum += partials[i];
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
  cout << "Tiempo promedio: " << sum / 10.0 << " ms" << endl;
}
