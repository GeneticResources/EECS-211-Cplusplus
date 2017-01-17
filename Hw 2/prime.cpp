#include <iostream>
#include <cmath>
#include <math.h>

using namespace std;

// Determines whether the given number is prime.
bool is_prime(int p)
{
  int sqroot = floor(sqrt(p));
  bool isPrime = false;
  bool numIsPrime = false;	     

  // Loop to check all prime numbers up to its square root
  for(int i = 2; i <= sqroot; i++) {

    // Check if numbers less than square root are prime
    for(int j = 2; j < i; j++) {
      if(i % j == 0) {
	isPrime = false;
	break;
      }
      else
	isPrime = true;
    }
    
    // Check if evenly divisible
    if(isPrime || i == 2) {
      if(p % i != 0)
	numIsPrime = true;
      else
	numIsPrime = false;
    }

    // Break if the number is not a prime
    if(!numIsPrime)
      break;
  }

  // Takes into account outlyers 2 and 3 (sqrt is smaller than 1)
  if(p == 2 || p == 3)
    return true;
  return numIsPrime;
}

int main()
{
  int n;
  cout << "Enter an integer: ";
  cin >> n;

  // Check contraints
  if(n < 2 || n > 300000) {
    cout << "Enter an integer between 2 and 30000.";
    return 0;
  }

  // Run through every number from 2 up to that number
  for(int i = 2; i < n; i++) {
    if(is_prime(i))
      cout << i << '\n';
  }

  return 0;
}
