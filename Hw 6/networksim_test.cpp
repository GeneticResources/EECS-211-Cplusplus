#include "errors.h"
#include "parsing.h"
#include "datagram.h"
#include "machines.h"
#include "system.h"

#include <UnitTest++/UnitTest++.h>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace std;

// Sample test to demonstrate how to use shared_ptr::use_count() to test whether
// Node::disconnect() works or not. If it works, n1 and n2 should no longer hold
// reference to each other, so the use_count() will decrease to 1. After this
// test case returns, you should also be able to see the message "machine 'NODE_
// DISCONNECT Machine 2' deleted" nad "machine 'NODE_DISCONNECT Machine 1' deleted"
// because n1 and n2 are the only reference to these two machines. After they
// go out of scope, the memory they hold will be deallocated.
TEST(NODE_DISCONNECT)
{
    // Allocate the machine and connect them, as did in System::create_machine
    // and System::connect_machine.
    shared_ptr<Node> n1 = make_shared<Node>("NODE_DISCONNECT Machine 1", IP_address("1.2.3.4"));
    shared_ptr<Node> n2 = make_shared<Node>("NODE_DISCONNECT Machine 2", IP_address("5.6.7.8"));

    n1->connect(n2);
    n2->connect(n1);

    // Both n1 and n2 also hold a reference to each other.
    CHECK_EQUAL(2, n1.use_count());
    CHECK_EQUAL(2, n2.use_count());

    n1->disconnect(n2);
    n2->disconnect(n1);

    // No references other than 'n1' and 'n2' remained.
    CHECK_EQUAL(1, n1.use_count());
    CHECK_EQUAL(1, n2.use_count());
}
