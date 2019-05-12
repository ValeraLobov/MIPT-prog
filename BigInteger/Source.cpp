#include "biginteger.h"
#include <iostream>

using namespace std;

int main() {
  BigInteger B;
  cin >> B;
  BigInteger A = to_int("123456789");
  BigInteger C = to_int(123456789ll);
  int x;
  x = 123456789;

  if (B <= A)
    cout << A - B;
  else {
    cout << "-";
    cout << B - A;
  }

  cout << A + B;
  cout << A * B;
  cout << A / B;
  cout << A % B;
  printf("%d\n", A % x);

  C = ++A;
  ++B;
  C += B + x;

  A = to_int(16);
  x = 12;

  system("pause");
  return 0;
}