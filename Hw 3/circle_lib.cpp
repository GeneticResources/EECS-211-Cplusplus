#include "circle_lib.h"

#include <iostream>
#include <stdexcept>
#include <cmath>

using namespace std;

Circle read_circle()
{
    Circle circ;
    cin >> circ.x >> circ.y >> circ.radius;

    return circ;
}

bool overlapped(Circle c1, Circle c2)
{

    if(c1.radius < 0 || c2.radius < 0)
        throw runtime_error("Circle radius is less than zero.");

    double bothRadii = c1.radius + c2.radius;
    double distance = sqrt(pow((c2.x-c1.x),2) + pow((c2.y-c1.y),2));

    return distance < bothRadii;
}

