#include <chrono>
#include <iostream>
using namespace std;

#define MAX 10000

int **createMatrix(int rows, int cols) {
  int **mat = new int *[rows];
  for (int i = 0; i < rows; i++)
    mat[i] = new int[cols];
  return mat;
}

void deleteMatrix(int **mat, int rows) {
  for (int i = 0; i < rows; i++)
    delete[] mat[i];
  delete[] mat;
}

int it1() {
  int **mat = createMatrix(MAX, MAX);
  auto start = chrono::high_resolution_clock::now();

  for (int i = 0; i < MAX; i++)
    for (int j = 0; j < MAX; j++)
      mat[i][j] = i + j;

  auto end = chrono::high_resolution_clock::now();
  auto time = chrono::duration_cast<chrono::microseconds>(end - start)
                  .count();
  cout << "Ejemplo 1: " << time << " microsegundos" << endl;
  deleteMatrix(mat, MAX);
  return time;
}

int it2() {
  int **mat = createMatrix(MAX, MAX);
  auto start = chrono::high_resolution_clock::now();

  for (int j = 0; j < MAX; j++)
    for (int i = 0; i < MAX; i++)
      mat[i][j] = i * j;

  auto end = chrono::high_resolution_clock::now();
  auto time = chrono::duration_cast<chrono::microseconds>(end - start)
                  .count();
  cout << "Ejemplo 2: " << time << " microsegundos" << endl;
  deleteMatrix(mat, MAX);
  return time;
}

int main() {
  it1();
  it2();
  return 0;
}
