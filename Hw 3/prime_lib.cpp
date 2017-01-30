#include "prime_lib.h"

#include <stdexcept>
#include <vector>
#include <cmath>

using namespace std;

bool is_prime(int p)
{
    if(p < 1)
        throw runtime_error("A negative integer or zero was entered.");

    int sqroot = sqrt(p);
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

bool check_is_prime(const std::vector<int>& primes, int p)
{
    if(p < 1)
        throw runtime_error("A negative integer or zero was entered.");
    if(primes.empty())
        throw runtime_error("Prime vector is empty.");

    bool isPrime = true;

    // Check for 1, 2, and 3
    if(p == 2 || p == 3)
        return isPrime;
    else if(p == 1)
        return false;

    // Loop to check all prime numbers up to its square root
    for(int i = 0; i < primes.size(); i++) {
        // Check if evenly divisible
        if(p % primes[i] == 0 && primes[i] != 1 && primes[i] != p) {
            isPrime = false;
            break;
        }
    }
    return isPrime;
}

vector<int> generate_primes(int n)
{
    if(n < 1)
        throw runtime_error("A negative integer or zero was entered.");

    vector<bool>sieve;
    vector<int>primes;
    sieve.resize(n+1);

    // Mark true between 2 and n
    for(int i = 2; i <= n; i++) {
        sieve[i] = true;
    }

    // Cancel multiples of primes
    for(int i = 2; i <= n; i++) {
        if(sieve[i]) {
            // Cross out multiples
            for(int j = 2; j < n; j++) {
                // Exit if larger than max index
                if(i*j > sieve.size()-1)
                    break;
                sieve[i*j] = false;
            }
            // Store into vector
            primes.push_back(i);
        }
    }
    return primes;
}

