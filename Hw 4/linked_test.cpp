#include "linked_lib.h"

#include <UnitTest++/UnitTest++.h>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace std;

// Test for push_back function
TEST(PUSH_BACK) {
  // Define pointers to linked lists
  List testList = cons(4,cons(6, nullptr));
  List testList2 = nullptr;

  List expected = cons(4,cons(6,cons(100, nullptr)));
  List expected2 = cons(2, nullptr);

  push_back(testList,100);
  push_back(testList2, 2);

  // Compare lists too see if they are equal
  bool test = compareLists(testList, expected);
  bool test2 = compareLists(testList2, expected2);

  // Tests
  CHECK_EQUAL(true, test);
  CHECK_EQUAL(true, test2);
}

// Test for filter_lt function
TEST(FILTERLT) {
  List testList = cons(4,cons(6,cons(7,cons(-4, nullptr))));
  List testList2 = cons(1,cons(10,cons(3,cons(6, nullptr))));
  List testList3 = cons(10,cons(5,cons(8, nullptr)));

  List expected = cons(4,cons(-4, nullptr));
  List expected2 = cons(1,cons(3, nullptr));
  List expected3 = cons(5, nullptr);

  filter_lt(testList,6);
  filter_lt(testList2,5);
  filter_lt(testList3,7);

  bool test = compareLists(testList, expected);
  bool test2 = compareLists(testList2, expected2);
  bool test3 = compareLists(testList3, expected3);

  CHECK_EQUAL(true, test);
  CHECK_EQUAL(true, test2);
  CHECK_EQUAL(true, test3);
}

// Tests for pop_front function
TEST(POP_FRONT) {
  List empty = nullptr;
  CHECK_THROW(pop_front(empty), runtime_error);

  List param = cons(8,cons(5, nullptr));
  List testList = pop_front(param);
  List expected = make_shared<ListNode>(ListNode{8, nullptr});

  bool test = compareLists(testList, expected);
  CHECK_EQUAL(true, test);
}

// Example test case for push_back
// Testing push_back on a singleton
TEST(PUSH_BACK_OF_ONE_ELEMENT) {
  // Initialize the input linked-list to push_back
  List xs = make_shared<ListNode>();
  xs->data = 514;
  List ys = xs;

  push_back(xs, 2017211);

  // Check that the output is a 2-element linked-list with correct order
  CHECK_EQUAL(ys, xs);
  CHECK_EQUAL(514, xs->data);
  CHECK(xs->next != nullptr);
  CHECK_EQUAL(2017211, xs->next->data);
  // Note: It is impossible to apply CHECK_EQUAL on nullptr, so we use
  // CHECK and manually compares for equality
  CHECK(xs->next->next == nullptr);
}

TEST(NTH_ELEMENT) {
  List testList = cons(4,cons(6,cons(8,cons(10, nullptr))));

  CHECK_THROW(nth_element(testList, 4), runtime_error);
  CHECK_EQUAL(8, nth_element(testList,2));
  CHECK_EQUAL(10,nth_element(testList,3));
}

// This function overloads the '<<' operator to enable printing on List
// types. For example, 'cout << xs << '\n';' will compile and print its
// content. However, we are still not able to write tests such as
// CHECK_EQUAL(nullptr, xs) since nullptr might be of any pointer type
// and the '<<' operator does not know how to print it.
ostream& operator<<(ostream& out, const List& lst)
{
  if (lst) {
    out << "<ListNode@" << static_cast<void*>(lst.get());
    out << " data=" << lst->data;
    out << " next=" << static_cast<void*>(lst->next.get());
    out << ">";
  } else {
    out << "(nullptr)";
  }
  return out;
}
