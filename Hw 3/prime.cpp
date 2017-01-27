#include "prime_lib.h"

#include <iostream>
#include <vector>

using namespace std;

int main()
{
  int n;
  cin >> n;

  vector<int> primes = generate_primes(n);

  int p;
  cin >> p;
  while (p > 0) {
    if (check_is_prime(primes, p))
      cout << "yes\n";
    else
      cout << "no\n";
    cin >> p;
  }
  return 0;
}
