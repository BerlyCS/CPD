#include <cstdlib>
#include <iostream>
#include <vector>
using namespace std;
int main() {
  vector<int> a(100);

  for (auto &i : a) {
    i = rand() % 100;
  }

  vector<int> b(100);
  for (int i = 0; i<100; i++) {
    b[i] = a[i] * a[i] + 3*a[i] +1;
  }

  int sum =0;
  for (auto &i: b){
    sum += i;
  }

  cout<<sum<<endl;
}
