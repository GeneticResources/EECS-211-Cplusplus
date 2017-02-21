#include "errors.h"
#include "parsing.h"
#include "datagram.h"

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
TEST(PARSE_IP_ERROR) {
    CHECK_THROW_ENUM(parse_IP("127&&&01"), err_code, bad_ip_address);
    CHECK_THROW_ENUM(parse_IP("127.0. .1"), err_code, bad_ip_address);
    CHECK_THROW_ENUM(parse_IP("-127.0.0.1"), err_code, bad_ip_address);
    CHECK_THROW_ENUM(parse_IP("260.0.0.2"), err_code, bad_ip_address);
    CHECK_THROW_ENUM(parse_IP("48320492"), err_code, bad_ip_address);
    CHECK_THROW_ENUM(parse_IP("129.39.28.23.140"), err_code, bad_ip_address);
    CHECK_THROW_ENUM(parse_IP(""), err_code, bad_ip_address);
}
// Unit tests for all three functions
TEST(PARSE_IP) {
    array<int, 4> test = parse_IP("127.0.0.1");
    array<int, 4> test3 = parse_IP("255.255.255.255");

    array<int, 4> expected = {127, 0, 0, 1};
    array<int, 4> expected3 = {255, 255, 255, 255};

    bool bool1 = true;
    bool bool3 = true;

    // Check equal arrays
    for(int i = 0; i < test.size(); ++i) {
        if (test[i] != expected[i]) {
            bool1 = false;
            break;
        }
    }
    for(int i = 0; i < test3.size(); ++i) {
        if (test3[i] != expected3[i]) {
            bool1 = false;
            break;
        }
    }

    CHECK_EQUAL(true, bool1);
    CHECK_EQUAL(true, bool3);
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