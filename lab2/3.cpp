#include <algorithm>
#include <chrono>
#include <iostream>
using namespace std;

#define MAX 1000
#define BLOCK 64

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

  for (int ii = 0; ii < MAX; ii += BLOCK)
    for (int jj = 0; jj < MAX; jj += BLOCK)
      for (int kk = 0; kk < MAX; kk += BLOCK) {
        int i_end = min(ii + BLOCK, MAX);
        int j_end = min(jj + BLOCK, MAX);
        int k_end = min(kk + BLOCK, MAX);
        for (int i = ii; i < i_end; i++)
          for (int j = jj; j < j_end; j++)
            for (int k = kk; k < k_end; k++)
              C[i][j] += A[i][k] * B[k][j];
      }

  auto end = chrono::high_resolution_clock::now();
  auto time = chrono::duration_cast<chrono::microseconds>(end - start).count();
  cout << "multiplicacion por bloques: " << time << " microsegundos" << endl;

  deleteMatrix(A, MAX);
  deleteMatrix(B, MAX);
  deleteMatrix(C, MAX);
  return 0;
}
