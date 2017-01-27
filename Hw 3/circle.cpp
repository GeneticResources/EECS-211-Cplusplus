#include "circle_lib.h"

#include <iostream>
#include <vector>

using namespace std;

int main()
{
  Circle c1 = read_circle();
  string output = "";

  // Infinite loop until the last value is negative
  while(true) {
    Circle cNext = read_circle();

    if(cNext.radius < 0)
      break;

    // Check if circles are overlapping
    if(!overlapped(c1, cNext))
      output = output + "not overlapped\n";
    else
      output = output + "overlapped\n";
  }

  cout << output << '\n';
  return 0;

  return 0;
}
