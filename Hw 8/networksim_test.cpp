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

// System tests
TEST(SYSTEM) {
    System sys;

    // Create laptops
    sys.create_machine("laptop", "myLaptop", IP_address("1.1.1.1"));
    sys.create_machine("laptop", "myLaptop2", IP_address("2.2.2.2"));
    sys.create_machine("laptop", "myLaptop3", IP_address("3.3.3.3"));
    sys.create_machine("laptop", "myLaptop4", IP_address("4.4.4.4"));
    sys.create_machine("laptop", "myLaptop5", IP_address("5.5.5.5"));
    sys.create_machine("laptop", "myLaptop6", IP_address("6.6.6.6"));
    sys.create_machine("laptop", "myLaptop7", IP_address("7.7.7.7"));
    sys.create_machine("laptop", "myLaptop8", IP_address("8.8.8.8"));
    sys.create_machine("laptop", "myLaptop9", IP_address("9.9.9.9"));

    // Create servers
    sys.create_machine("server", "myServer", IP_address("11.11.11.11"));
    sys.create_machine("server", "myServer2", IP_address("12.12.12.12"));
    sys.create_machine("server", "myServer3", IP_address("13.13.13.13"));
    sys.create_machine("server", "myServer4", IP_address("14.14.14.14"));
    sys.create_machine("server", "myServer5", IP_address("15.15.15.15"));

    // Create WANs
    sys.create_machine("wan", "myWan", IP_address("100.100.100.100"));
    sys.create_machine("wan", "myWan2", IP_address("120.120.120.120"));
    sys.create_machine("wan", "myWan3", IP_address("130.130.130.130"));
    sys.create_machine("wan", "myWan4", IP_address("140.140.140.140"));
    sys.create_machine("wan", "myWan5", IP_address("150.150.150.150"));
    sys.create_machine("wan", "myWan5", IP_address("160.160.160.160"));

    // Server connecting
    sys.connect_machine(IP_address("11.11.11.11"), IP_address("1.1.1.1"));
    sys.connect_machine(IP_address("11.11.11.11"), IP_address("2.2.2.2"));
    sys.connect_machine(IP_address("11.11.11.11"), IP_address("3.3.3.3"));
    sys.connect_machine(IP_address("11.11.11.11"), IP_address("4.4.4.4"));
    sys.connect_machine(IP_address("11.11.11.11"), IP_address("5.5.5.5"));
    sys.connect_machine(IP_address("11.11.11.11"), IP_address("6.6.6.6"));
    sys.connect_machine(IP_address("11.11.11.11"), IP_address("7.7.7.7"));
    sys.connect_machine(IP_address("11.11.11.11"), IP_address("8.8.8.8"));

    sys.connect_machine(IP_address("11.11.11.11"), IP_address("100.100.100.100"));
    sys.connect_machine(IP_address("11.11.11.11"), IP_address("120.120.120.120"));
    sys.connect_machine(IP_address("11.11.11.11"), IP_address("130.130.130.130"));
    sys.connect_machine(IP_address("11.11.11.11"), IP_address("140.140.140.140"));

    // Wan connecting
    sys.connect_machine(IP_address("100.100.100.100"), IP_address("12.12.12.12"));
    sys.connect_machine(IP_address("100.100.100.100"), IP_address("13.13.13.13"));
    sys.connect_machine(IP_address("100.100.100.100"), IP_address("14.14.14.14"));

    sys.connect_machine(IP_address("120.120.120.120"), IP_address("130.130.130.130"));
    sys.connect_machine(IP_address("120.120.120.120"), IP_address("140.140.140.140"));
    sys.connect_machine(IP_address("120.120.120.120"), IP_address("150.150.150.150"));
    sys.connect_machine(IP_address("120.120.120.120"), IP_address("160.160.160.160"));

    // No such machine errors
    CHECK_THROW_ENUM(sys.connect_machine(IP_address("10.10.10.10"), IP_address("11.11.11.11")), err_code, no_such_machine);
    CHECK_THROW_ENUM(sys.allocate_datagram(IP_address("0.0.0.0"), IP_address("10.10.10.10"), "hello"), err_code, no_such_machine);
    CHECK_THROW_ENUM(sys.release_datagram(IP_address("255.255.255.255")), err_code, no_such_machine);

    // Server
    CHECK_THROW_ENUM(sys.connect_machine(IP_address("11.11.11.11"), IP_address("150.150.150.150")), err_code, connect_failed);
    CHECK_THROW_ENUM(sys.connect_machine(IP_address("11.11.11.11"), IP_address("9.9.9.9")), err_code, connect_failed);

    // Laptop
    CHECK_THROW_ENUM(sys.connect_machine(IP_address("1.1.1.1"), IP_address("12.12.12.12")), err_code, connect_failed);
    CHECK_THROW_ENUM(sys.connect_machine(IP_address("1.1.1.1"), IP_address("100.100.100.100")), err_code, connect_failed);

    // Wan
    CHECK_THROW_ENUM(sys.connect_machine(IP_address("100.100.100.100"), IP_address("15.15.15.15")), err_code, connect_failed);
    CHECK_THROW_ENUM(sys.connect_machine(IP_address("120.120.120.120"), IP_address("100.100.100.100")), err_code, connect_failed);
}

// Laptop and server tests
TEST(LAPTOP_SERVER) {
    shared_ptr<Laptop> l1 = make_shared<Laptop>("laptop1", IP_address("10.10.10.10"));
    shared_ptr<Laptop> l2 = make_shared<Laptop>("laptop2", IP_address("11.11.11.11"));
    shared_ptr<Server> serv = make_shared<Server>("server", IP_address("100.100.100.100"));
    shared_ptr<Server> serv2 = make_shared<Server>("server2", IP_address("101.101.101.101"));
    shared_ptr<WAN_node> wan = make_shared<WAN_node>("wan", IP_address("1.1.1.1"));

    l1->connect(serv);
    serv->connect(l1);
    CHECK_EQUAL(2, serv.use_count());

    CHECK_EQUAL(false, l1->can_connect(l2));
    CHECK_EQUAL(true, l2->can_connect(serv));
    CHECK_THROW_ENUM(l1->connect(wan), err_code, connect_failed);
    CHECK_THROW_ENUM(l1->connect(l2), err_code, connect_failed);
    CHECK_THROW_ENUM(l1->connect(serv2), err_code, connect_failed);

    l1->disconnect(serv);
    serv->disconnect(l1);
    CHECK_EQUAL(1, serv2.use_count());

    l1->connect(serv2);
    serv2->connect(l1);
    l2->connect(serv2);
    serv2->connect(l2);

    CHECK_EQUAL(3, serv2.use_count());

    l1->allocate_datagram(l2->get_ip(), "hello"); // Create datagram
    CHECK_EQUAL(1, l1->send()); // Send to server

    CHECK_EQUAL(1, serv2->send()); // Send to matched connected machines

    CHECK_EQUAL("hello", l2->release_datagram());

    CHECK_EQUAL(IP_address("10.10.10.10"), l1->get_ip());
    CHECK_EQUAL(IP_address("11.11.11.11"), l2->get_ip());
}

// Laptop server and WAN
TEST(SERVER_WAN) {
    shared_ptr<Laptop> l1 = make_shared<Laptop>("laptop", IP_address("10.10.10.10"));
    shared_ptr<Laptop> l2 = make_shared<Laptop>("laptop2", IP_address("11.11.11.11"));
    shared_ptr<Server> serv = make_shared<Server>("server", IP_address("101.101.101.101"));
    shared_ptr<WAN_node> wan = make_shared<WAN_node>("wan", IP_address("1.1.1.1"));
    shared_ptr<WAN_node> wan2 = make_shared<WAN_node>("wan2", IP_address("5.5.5.5"));

    l1->connect(serv);
    serv->connect(l1);
    wan->connect(serv);
    serv->connect(wan);

    l1->allocate_datagram(IP_address("8.8.8.8"), "hello world");
    l1->send();
    CHECK_EQUAL(1, serv->send());
    CHECK_EQUAL("", l2->release_datagram()); // Only sent to WAN nodes
}

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

TEST(RECEIVE) {
    shared_ptr<Laptop> l1 = make_shared<Laptop>("laptop", IP_address("10.10.10.10"));

    Datagram* d1 = new Datagram(IP_address("1.1.1.1"), IP_address("10.10.10.10"), "hello world");
    Datagram* d2 = new Datagram(IP_address("3.3.3.3"), IP_address("10.10.10.10"), "bye world");

    l1->receive(d1);

    CHECK_THROW_ENUM(l1->receive(d2), err_code, recv_blocked);
}