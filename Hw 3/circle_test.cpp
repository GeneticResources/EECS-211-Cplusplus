#include "circle_lib.h"

#include <UnitTest++/UnitTest++.h>
#include <stdexcept>

using namespace std;

TEST(OVERLAPPED)
{
    CHECK_EQUAL(true, overlapped({0,0,5},{0,2,1}));
    CHECK_EQUAL(false, overlapped({0,0,2},{4,0,2}));
}

TEST(OVERLAPPED_ERROR)
{
    CHECK_THROW(overlapped({0,0,-2}, {0,2,1}), std::runtime_error);
}

