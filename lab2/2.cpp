#include <chrono>
#include <iostream>
using namespace std;

#define MAX 1000

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

int main() {
  int **A = createMatrix(MAX, MAX);
  int **B = createMatrix(MAX, MAX);
  int **C = createMatrix(MAX, MAX);

  for (int i = 0; i < MAX; i++)
    for (int j = 0; j < MAX; j++) {
      A[i][j] = 1;
      B[i][j] = 1;
      C[i][j] = 0;
    }

  auto start = chrono::high_resolution_clock::now();

  for (int i = 0; i < MAX; i++)
    for (int j = 0; j < MAX; j++)
      for (int k = 0; k < MAX; k++)
        C[i][j] += A[i][k] * B[k][j];

  auto end = chrono::high_resolution_clock::now();
  auto time = chrono::duration_cast<chrono::microseconds>(end - start).count();
  cout << "multiplicacion de matrices clasica: " << time << " microsegundos" << endl;

  deleteMatrix(A, MAX);
  deleteMatrix(B, MAX);
  deleteMatrix(C, MAX);
  return 0;
}
