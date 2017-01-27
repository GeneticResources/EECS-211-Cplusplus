#include "prime_lib.h"

#include <UnitTest++/UnitTest++.h>
#include <stdexcept>
#include <vector>

using namespace std;

TEST(IS_PRIME)
{
    CHECK_EQUAL(false, is_prime(1));
    CHECK_EQUAL(true, is_prime(2));
    CHECK_EQUAL(true, is_prime(3));
    CHECK_EQUAL(false, is_prime(4));
    CHECK_EQUAL(true, is_prime(5));
}

TEST(IS_PRIME_ERRORS)
{
    CHECK_THROW(is_prime(0), std::runtime_error);
    CHECK_THROW(is_prime(-10), std::runtime_error);
}

TEST(GENERATE_PRIME)
{
    vector<int> p1 = {2,3,5,7};
    vector<int> p2 = generate_primes(10);
    CHECK_EQUAL(true, p1 == p2);

    vector<int> p3 = {2,3,5,7,11};
    vector<int> p4 = generate_primes(11);
    CHECK_EQUAL(true, p3 == p4);
}

TEST(GENERATE_PRIME_ERRORS)
{
    CHECK_THROW(generate_primes(-10), std::runtime_error);
}

TEST(CHECK_IS_PRIME)
{
    vector<int> primes = generate_primes(10);

    CHECK_EQUAL(false, check_is_prime(primes, 1));
    CHECK_EQUAL(true, check_is_prime(primes, 2));
    CHECK_EQUAL(true, check_is_prime(primes, 3));
    CHECK_EQUAL(false, check_is_prime(primes, 4));
    CHECK_EQUAL(true, check_is_prime(primes, 5));

    CHECK_EQUAL(true, check_is_prime(primes, 10) == is_prime(10));
    CHECK_EQUAL(true, check_is_prime(primes, 17) == is_prime(17));
}

TEST(CHECK_IS_PRIME_ERRORS)
{
    vector<int> primes = generate_primes(10);
    CHECK_THROW(check_is_prime(primes, 0), std::runtime_error);
    CHECK_THROW(check_is_prime(primes, -10), std::runtime_error);

    vector<int> prime;
    CHECK_THROW(check_is_prime(prime, 10), std::runtime_error);
}
