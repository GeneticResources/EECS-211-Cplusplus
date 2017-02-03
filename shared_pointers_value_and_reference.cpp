#include <iostream>
#include <memory>
using namespace std;

int main() {
	shared_ptr<int> x = make_shared<int>(10);
    shared_ptr<int> y = x;
    shared_ptr<int>& z = x;
    
    cout << "Before any values are altered: " << endl;
    cout << "Memory and value of x: " << x << '-' << *x << '\n';
    cout << "Memory and value of y: " << y << '-' << *y << '\n';
    cout << "Memory and value of z: " << z << '-' << *z << '\n';
    
    *y += 10;
    cout << endl;
    cout << "Only y is altered:" << endl;
    cout << "Memory and value of x: " << x << '-' << *x << '\n';
    cout << "Memory and value of y: " << y << '-' << *y << '\n';
    cout << "Memory and value of z: " << z << '-' << *z << '\n';
    
    // All point to different memory locations than before
    // z trumps y and y is different from x because z a reference of x
    // so memory locations and values are the same, but y is a "copy" of x
    y = make_shared<int>(2);
    z = make_shared<int>(3);
    cout << endl;
    cout << "y and z set equal to new pointers:" << endl;
    cout << "Memory and value of x: " << x << '-' << *x << '\n';
    cout << "Memory and value of y: " << y << '-' << *y << '\n';
    cout << "Memory and value of z: " << z << '-' << *z << '\n';
	return 0;
}
