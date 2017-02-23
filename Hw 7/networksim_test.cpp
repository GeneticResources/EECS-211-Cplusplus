#include "errors.h"
#include "parsing.h"
#include "datagram.h"
#include "machines.h"
#include "system.h"

#include <UnitTest++/UnitTest++.h>
#include <array>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace std;

// A system of four connected machines. They are used in the following unit
// tests
class SystemF
{
public:
    array<shared_ptr<Node>, 4> ms_;

    SystemF()
    {
        ms_[0] = make_shared<Node>("L", IP_address("192.168.0.1"));
        ms_[1] = make_shared<Node>("S", IP_address("140.112.239.8"));
        ms_[2] = make_shared<Node>("T", IP_address("140.112.253.11"));
        ms_[3] = make_shared<Node>("W", IP_address("8.8.8.8"));
        for (size_t i = 1; i < 4; ++i) {
            ms_[0]->connect(ms_[i]);
            ms_[i]->connect(ms_[0]);
        }
    }

    ~SystemF()
    {
        // Break the connection between the machines to free the memory
        for (size_t i = 1; i < 4; ++i) {
            ms_[0]->disconnect(ms_[i]);
            ms_[i]->disconnect(ms_[0]);
        }
    }
};
/*
// Test the specification "If the destination of the Datagram is
// in the connected machines, send the datagram to that machine."
TEST(SEND_BEST_MACHINE_DESTINATION_MATCH)
{
    SystemF sys;
    sys.ms_[0]->allocate_datagram(sys.ms_[2]->get_ip(), "ping");
    CHECK_EQUAL(1, sys.ms_[0]->send()); // successfully sent one datagram
    CHECK_EQUAL("ping", sys.ms_[2]->release_datagram());
}

// Test the specification "Otherwise, send it to the connected machine
// whose first number of IP address is closest to the first number of
// the destination."
TEST(SEND_BEST_MACHINE_CLOSEST_FIRST_NUMBER)
{
    SystemF sys;
    sys.ms_[0]->allocate_datagram(IP_address("9.8.7.6"), "pong");
    CHECK_EQUAL(1, sys.ms_[0]->send()); // successfully sent one datagram
    CHECK_EQUAL(1, sys.ms_[3]->send()); // ms_[3] is not the destination, but
                                        // we can send it out again
}
*/

#define CHECK_THROW_ENUM(expression, EnumType, EnumValue) \
   UNITTEST_MULTILINE_MACRO_BEGIN \
   bool caught_ = false; \
   try { expression; } \
   catch (EnumType e) { caught_ = e == EnumType::EnumValue; } \
   catch (...) {} \
   if (!caught_) \
      UnitTest::CurrentTest::Results()->OnTestFailure( \
          UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), \
                                __LINE__), \
          "Expected exception: \"" #EnumValue "\" not thrown"); \
   UNITTEST_MULTILINE_MACRO_END


// Runtime error tests for all three functions
TEST(TOKENIZE_ERROR) {
    CHECK_THROW(tokenize("\" de@f.com \"ghi j-k \" w == \"z\""), runtime_error);
    CHECK_THROW(tokenize("hel\"l\"o wo\"rld \"\" \"eecs 211\""), runtime_error);
    CHECK_THROW(tokenize("\"abc\\\" \"d\"ef\""), runtime_error);
    CHECK_THROW(tokenize("\""), runtime_error);
}
TEST(PARSE_INT_ERROR) {
    CHECK_THROW(parse_int("-10912"), runtime_error);
    CHECK_THROW(parse_int("10 912"), runtime_error);
    CHECK_THROW(parse_int("10%912"), runtime_error);
    CHECK_THROW(parse_int(""), runtime_error);
}
TEST(IP_ADDRESS_ERROR) {
    CHECK_THROW_ENUM(IP_address("127&&&01"), err_code, bad_ip_address);
    CHECK_THROW_ENUM(IP_address("127.0. .1"), err_code, bad_ip_address);
    CHECK_THROW_ENUM(IP_address("-127.0.0.1"), err_code, bad_ip_address);
    CHECK_THROW_ENUM(IP_address("260.0.0.2"), err_code, bad_ip_address);
    CHECK_THROW_ENUM(IP_address("48320492"), err_code, bad_ip_address);
    CHECK_THROW_ENUM(IP_address("129.39.28.23.140"), err_code, bad_ip_address);
    CHECK_THROW_ENUM(IP_address(""), err_code, bad_ip_address);
}
TEST(TOKENIZE) {
    vector<string> test1 = tokenize("");
    vector<string> test2 = tokenize(" de@f.com \"ghi j-k \" w == \"z\"");
    vector<string> test3 = tokenize("hello");
    vector<string> test4 = tokenize("hel\"l\"o world \"\" \"eecs 211\"");
    vector<string> test5 = tokenize("\"abc\\\" \"def\"");
    vector<string> test6 = tokenize("hello hi");
    vector<string> test7 = tokenize("  -   ");
    vector<string> test8 = tokenize(" \"     \"     \"  \"");
    vector<string> test9 = tokenize("");

    vector<string> expected2 {"de@f.com", "ghi j-k ", "w", "==", "z"};
    vector<string> expected3 {"hello"};
    vector<string> expected4 = {"hel", "l", "o", "world", "", "eecs 211"};
    vector<string> expected5 = {"abc\\", "def"};
    vector<string> expected6 = {"hello", "hi"};
    vector<string> expected7 = {"-"};
    vector<string> expected8 = {"     ", "  "};

    bool bool2 = true;
    bool bool3 = true;
    bool bool4 = true;
    bool bool5 = true;
    bool bool6 = true;
    bool bool7 = true;
    bool bool8 = true;

    // Check equality
    if(test2.size() == expected2.size()) {
        for(int i = 0; i < test2.size(); ++i) {
            if(test2[i] != expected2[i]) {
                bool2 = false;
                break;
            }
        }
    } else bool2 = false;
    if(test3.size() == expected3.size()) {
        for(int i = 0; i < test3.size(); ++i) {
            if(test3[i] != expected3[i]) {
                bool3 = false;
                break;
            }
        }
    } else bool3 = false;
    if(test4.size() == expected4.size()) {
        for(int i = 0; i < test4.size(); ++i) {
            if(test4[i] != expected4[i]) {
                bool4 = false;
                break;
            }
        }
    } else bool4 = false;
    if(test5.size() == expected5.size()) {
        for(int i = 0; i < test5.size(); ++i) {
            if(test5[i] != expected5[i]) {
                bool5 = false;
                break;
            }
        }
    } else bool5 = false;
    if(test6.size() == expected6.size()) {
        for(int i = 0; i < test6.size(); ++i) {
            if(test6[i] != expected6[i]) {
                bool6 = false;
                break;
            }
        }
    } else bool6 = false;
    if(test7.size() == expected7.size()) {
        for(int i = 0; i < test7.size(); ++i) {
            if(test7[i] != expected7[i]) {
                bool7 = false;
                break;
            }
        }
    } else bool7 = false;
    if(test8.size() == expected8.size()) {
        for(int i = 0; i < test8.size(); ++i) {
            if(test8[i] != expected8[i]) {
                bool8 = false;
                break;
            }
        }
    } else bool8 = false;

    CHECK_EQUAL(true, test1.size() == 0);
    CHECK_EQUAL(true, bool2);
    CHECK_EQUAL(true, bool3);
    CHECK_EQUAL(true, bool4);
    CHECK_EQUAL(true, bool5);
    CHECK_EQUAL(true, bool6);
    CHECK_EQUAL(true, bool7);
    CHECK_EQUAL(true, bool8);
    CHECK_EQUAL(true, test9.size() == 0);
}
TEST(PARSE_INT) {
    CHECK_EQUAL(10, parse_int("10"));
    CHECK_EQUAL(12345, parse_int("12345"));
    CHECK_EQUAL(0, parse_int("0"));
}

// Sample test to demonstrate how to use shared_ptr::use_count() to test whether
// Node::disconnect() works or not. If it works, n1 and n2 should no longer hold
// reference to each other, so the use_count() will decrease to 1. After this
// test case returns, you should also be able to see the message "machine 'NODE_
// DISCONNECT Machine 2' deleted" nad "machine 'NODE_DISCONNECT Machine 1' deleted"
// because n1 and n2 are the only reference to these two machines. After they
// go out of scope, the memory they hold will be deallocated.
TEST(NODE_DISCONNECT)
{

    // shared_ptr, connect_machine, disconnect_machine causing error


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
// Tests for Node class
TEST(GET_IP) {
    shared_ptr<Node> node1 = make_shared<Node>("node1", IP_address("10.105.148.39"));
    shared_ptr<Node> node2 = make_shared<Node>("node2", IP_address("0.0.0.0"));

    CHECK_EQUAL(IP_address("10.105.148.39"), node1->get_ip());
    CHECK_EQUAL(IP_address("0.0.0.0"), node2->get_ip());
}
TEST(CONNECT) {
    shared_ptr<Node> node1 = make_shared<Node>("node1", IP_address("10.105.148.39"));
    shared_ptr<Node> node2 = make_shared<Node>("node2", IP_address("0.0.0.0"));
    shared_ptr<Node> node3 = make_shared<Node>("node3", IP_address("10.103.102.13"));

    CHECK_EQUAL(1, node1.use_count());
    CHECK_EQUAL(1, node2.use_count());
    CHECK_EQUAL(1, node3.use_count());

    node1->connect(node2);
    node2->connect(node1);

    CHECK_EQUAL(2, node1.use_count());
    CHECK_EQUAL(2, node2.use_count());
    CHECK_EQUAL(1, node3.use_count());

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    shared_ptr<Node> node4 = make_shared<Node>("node4", IP_address("10.10.10.10"));
    shared_ptr<Node> node5 = make_shared<Node>("node5", IP_address("11.11.11.11"));
    shared_ptr<Node> node6 = make_shared<Node>("node6", IP_address("12.12.12.12"));


    CHECK_EQUAL(1, node4.use_count());
    CHECK_EQUAL(1, node5.use_count());
    CHECK_EQUAL(1, node6.use_count());

    node4->connect(node5);
    node5->connect(node4);
    node6->connect(node4);
    node4->connect(node6);

    CHECK_EQUAL(3, node4.use_count());
    CHECK_EQUAL(2, node5.use_count());
    CHECK_EQUAL(2, node6.use_count());
}
TEST(DISCONNECT) {
    shared_ptr<Node> node1 = make_shared<Node>("node1", IP_address("10.105.148.39"));
    shared_ptr<Node> node2 = make_shared<Node>("node2", IP_address("0.0.0.0"));
    shared_ptr<Node> node3 = make_shared<Node>("node3", IP_address("10.103.102.13"));

    CHECK_EQUAL(1, node1.use_count());
    CHECK_EQUAL(1, node2.use_count());
    CHECK_EQUAL(1, node3.use_count());

    node1->connect(node2);
    node2->connect(node1);

    CHECK_EQUAL(2, node1.use_count());
    CHECK_EQUAL(2, node2.use_count());
    CHECK_EQUAL(1, node3.use_count());

    node1->disconnect(node2);
    node2->disconnect(node1);

    CHECK_EQUAL(1, node1.use_count());
    CHECK_EQUAL(1, node2.use_count());

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    shared_ptr<Node> node4 = make_shared<Node>("node4", IP_address("10.10.10.10"));
    shared_ptr<Node> node5 = make_shared<Node>("node5", IP_address("11.11.11.11"));
    shared_ptr<Node> node6 = make_shared<Node>("node6", IP_address("12.12.12.12"));

    CHECK_EQUAL(1, node4.use_count());
    CHECK_EQUAL(1, node5.use_count());
    CHECK_EQUAL(1, node6.use_count());

    node4->connect(node5);
    node5->connect(node4);
    node6->connect(node4);
    node4->connect(node6);

    CHECK_EQUAL(3, node4.use_count());
    CHECK_EQUAL(2, node5.use_count());
    CHECK_EQUAL(2, node6.use_count());
}
// Tests for Datagram class
TEST(GET_DESTINATION) {
    Datagram data1 = Datagram(IP_address("255.255.255.255"), IP_address("10.105.105.105"), "hi");
    Datagram data2 = Datagram(IP_address("10.5.6.7"), IP_address("10.5.6.7"), "hi");

    CHECK_EQUAL(IP_address("10.5.6.7"), data2.get_destination());
    CHECK_EQUAL(IP_address("10.105.105.105"), data1.get_destination());
}
// Tests for IP Address class
TEST(FIRST_OCTAD) {
    IP_address ip1 = IP_address("127.0.0.1");
    IP_address ip2 = IP_address("0.0.0.0");
    IP_address ip3 = IP_address("255.255.255.255");

    CHECK_EQUAL(true, ip1.first_octad() == 127);
    CHECK_EQUAL(true, ip2.first_octad() == 0);
    CHECK_EQUAL(false, ip3.first_octad() == 0);
}
// Tests for errors
TEST(CREATE_MACHINE) {
    System sys1 = System();

    CHECK_THROW_ENUM(sys1.create_machine("client", "user", IP_address("10.105.148.38")), err_code, unknown_machine_type);
    CHECK_THROW_ENUM(sys1.create_machine("", "user", IP_address("10.105.148.38")), err_code, unknown_machine_type);
}
TEST(DELETE_MACHINE) {
    System sys1 = System();
    System sys2 = System();

    sys2.create_machine("wan", "user1", IP_address("10.105.148.38"));

    CHECK_THROW_ENUM(sys1.delete_machine(IP_address("100.105.148.38")), err_code, no_such_machine);
    CHECK_THROW_ENUM(sys2.delete_machine(IP_address("10.105.148.100")), err_code, no_such_machine);
    CHECK_THROW_ENUM(sys1.delete_machine(IP_address("100.1.148.38")), err_code, no_such_machine);

    sys2.delete_machine(IP_address("10.105.148.38"));
}
TEST(CONNECT_MACHINE) {
    System sys1 = System();
    System sys2 = System();

    sys1.create_machine("laptop", "user1", IP_address("10.10.10.10"));
    sys1.create_machine("laptop", "user2", IP_address("9.9.9.9"));

    CHECK_THROW_ENUM(sys1.connect_machine(IP_address("10.10.10.10"), IP_address("11.11.11.11")), err_code, no_such_machine);

    sys1.connect_machine(IP_address("10.10.10.10"), IP_address("9.9.9.9"));
}