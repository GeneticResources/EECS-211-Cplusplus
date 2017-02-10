#include "errors.h"
#include "parsing.h"
#include "datagram.h"

#include <UnitTest++/UnitTest++.h>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace std;

TEST(TOKENIZE_ERROR) {
    CHECK_THROW(tokenize("\" de@f.com \"ghi j-k \" w == \"z\""), runtime_error);
    CHECK_THROW(tokenize("hel\"l\"o wo\"rld \"\" \"eecs 211\""), runtime_error);
    CHECK_THROW(tokenize("\"abc\\\" \"d\"ef\""), runtime_error);
}
TEST(PARSE_INT_ERROR) {
    CHECK_THROW(parse_int("-10912"), runtime_error);
    CHECK_THROW(parse_int("10 912"), runtime_error);
    CHECK_THROW(parse_int("10%912"), runtime_error);
}
TEST(PARSE_IP_ERROR) {
    CHECK_THROW(parse_IP("127&&&01"), runtime_error);
    CHECK_THROW(parse_IP("127.0. .1"), runtime_error);
    CHECK_THROW(parse_IP("-127.0.0.1"), runtime_error);
}