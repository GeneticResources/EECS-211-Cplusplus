#include <iostream>
#include <stdlib.h>

using namespace std;

int main() {
  /*
    Coefficients of the system
      ax + by = c
      dx + ey = f
  */
  int a, b, c, d, e, f;
  cout << "Enter 6 integers: ";
  
  cin >> a >> b >> c >> d >> e >> f;

  double x, y;
  
  // Check if ae - db does not equal 0
  if((a * e) - (d * b) != 0)
  {
      if(abs(a) <= 10000 && abs(b) <= 10000 && abs(c) <= 10000 && abs(d) <= 10000 && abs        (e) <= 10000 && abs(f) <= 10000)
      {
	y = (a * f - d * c) / (a * e - d * b);
	x = (c * e - f * b) / (a * e - d * b);
      }
      else {
	cout << "Enter numbers between -10000 and 10000\n";
	return 0;
      }
  }
  else {
    cout << "ae - db cannot equal 0!\n";
    return 0;
  }

  // Output
  cout << x << '\n';
  cout << y << '\n';

  return 0;
}
