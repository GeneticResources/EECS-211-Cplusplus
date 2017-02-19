#include "datagram.h"
#include "parsing.h"
#include "errors.h"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

IP_address::IP_address(string s)
{
    // [YOUR HW5 parse_IP CODE HERE]
}

bool IP_address::operator==(const IP_address& that) const
{
    // [YOUR CODE HERE]
}

int IP_address::first_octad() const
{
    // [YOUR CODE HERE]
}

std::ostream& operator<<(std::ostream& os, const IP_address& addr)
{
    return os << '"'
            << addr.ip_[0] << '.'
            << addr.ip_[1] << '.'
            << addr.ip_[2] << '.'
            << addr.ip_[3] << '"';
}

Datagram::Datagram(const IP_address& s, const IP_address& d, const string& m)
        : src_(""), dst_("") // [REPLACE THIS LINE BY YOUR CODE HERE]
{};

IP_address Datagram::get_destination() const
{
    // [YOUR CODE HERE]
}

std::ostream& operator<<(std::ostream& os, const Datagram& dg)
{
    os << "datagram {";
    os << "src=" << dg.src_;
    os << ", dst=" << dg.dst_;
    os << ", length=" << dg.length_;
    os << ", msg=\"" << dg.msg_ << "\"";
    os << "}";
    return os;
}
