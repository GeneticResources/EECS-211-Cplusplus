#include <iostream>
#include <cmath>

using namespace std;

// Represents a circle on the plane.
struct Circle
{
  double x, y;          // (x, y) is the center
  double radius;
};

// Reads a circle from cin and returns it.
Circle read_circle()
{
  cout << "Enter in the (x, y) coordinates and radius: ";

  Circle circ;
  cin >> circ.x >> circ.y >> circ.radius;
  
  return circ;
}

// Determines whether two circles overlap.
bool overlapped(Circle C1, Circle C2)
{
  double bothRadii = C1.radius + C2.radius;
  double distance = sqrt(pow((C2.x-C1.x),2) + pow((C2.y-C1.y),2));
  
  if(distance < bothRadii)
    return true;
  else
    return false;
}

int main() {

  Circle c1 = read_circle();
  string output = "";

  while(true) {
    if(cin.get() == '\n') {
      Circle cNext = read_circle();

      if(cNext.radius < 0)
        break;
      
      // If the circles aren't overlapping
      if(!overlapped(c1, cNext))
	output = output + "not overlapped\n";
      else
	output = output + "overlapped\n";
    }
  }
    
  cout << output << '\n';
  return 0;
}
