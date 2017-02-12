#include "errors.h"
#include "parsing.h"
#include "datagram.h"

#include <UnitTest++/UnitTest++.h>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace std;

// Runtime error tests for all three functions
TEST(TOKENIZE_ERROR) {
    CHECK_THROW(tokenize("\" de@f.com \"ghi j-k \" w == \"z\""), runtime_error);
    CHECK_THROW(tokenize("hel\"l\"o wo\"rld \"\" \"eecs 211\""), runtime_error);
    CHECK_THROW(tokenize("\"abc\\\" \"d\"ef\""), runtime_error);
}
TEST(PARSE_INT_ERROR) {
    CHECK_THROW(parse_int("-10912"), runtime_error);
    CHECK_THROW(parse_int("10 912"), runtime_error);
    CHECK_THROW(parse_int("10%912"), runtime_error);
    CHECK_THROW(parse_int(""), runtime_error);
}
TEST(PARSE_IP_ERROR) {
    CHECK_THROW(parse_IP("127&&&01"), runtime_error);
    CHECK_THROW(parse_IP("127.0. .1"), runtime_error);
    CHECK_THROW(parse_IP("-127.0.0.1"), runtime_error);
    CHECK_THROW(parse_IP("260.0.0.2"), runtime_error);
    CHECK_THROW(parse_IP("48320492"), runtime_error);
    CHECK_THROW(parse_IP("129.39.28.23.140"), runtime_error);
}
// Unit tests for all three functions
TEST(PARSE_IP) {
    array<int, 4> test = parse_IP("127.0.0.1");
    array<int, 4> expected = {127, 0, 0, 1};
    array<int, 4> test2 = parse_IP("");

    bool bool1 = true;
    bool bool2 = true;

    // Check equal arrays
    for(int i = 0; i < test.size(); ++i) {
        if (test[i] != expected[i]) {
            bool1 = false;
            break;
        }
    }

    for(int i = 0; i < test2.size(); ++i) {
        if(test2[i] != 0) {
            bool2 = false;
            break;
        }
    }

    CHECK_EQUAL(true, bool1);
    CHECK_EQUAL(true, bool2);
}
TEST(TOKENIZE) {
    vector<string> test1 = tokenize("");
    vector<string> test2 = tokenize(" de@f.com \"ghi j-k \" w == \"z\"");
    vector<string> expected2 {"de@f.com", "ghi j-k ", "w", "==", "z"};
    bool bool2 = true;

    // Check equality
    if(test2.size() == expected2.size()) {
        for(int i = 0; i < test2.size(); ++i) {
            if(test2[i] != expected2[i]) {
                bool2 = false;
                break;
            }
        }
    } else bool2 = false;

    CHECK_EQUAL(true, test1.size() == 0);
    CHECK_EQUAL(true, bool2);
}
TEST(PARSE_INT) {
    CHECK_EQUAL(10, parse_int("10"));
    CHECK_EQUAL(12345, parse_int("12345"));
    CHECK_EQUAL(0, parse_int("0"));
}