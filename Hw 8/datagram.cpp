#include "datagram.h"
#include "parsing.h"
#include "errors.h"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

IP_address::IP_address(string s)
{
    string stringNum = "";
    int lastStartIndex = 0;
    int index = 0;
    array<int, 4> nums = {};
    int countDots = 0;

    // Check errors
    if(s.length() == 0)
        throw err_code::bad_ip_address;
    for(int i = 0; i < s.length(); ++i) {
        int ascii = (int) s.at(i);

        // If not between 0-9 and is not a period
        if(!(ascii >= 48 && ascii <= 57) && ascii != 46)
            throw err_code::bad_ip_address;
    }

    for(int i = 0; i < s.length(); ++i) {
        if(s.at(i) == '.') {
            stringNum = s.substr(lastStartIndex, i - lastStartIndex);
            lastStartIndex = i + 1;
            ++countDots;

            try {
                parse_int(stringNum);
            } catch (runtime_error) {
                throw err_code::bad_ip_address;
            }

            // Error checking for between 0 and 255
            if(parse_int(stringNum) < 0 || parse_int(stringNum) > 255)
                throw err_code::bad_ip_address;

            nums[index] = parse_int(stringNum);
            ++index;
        }
        if(index == 3) {
            stringNum = s.substr(lastStartIndex, s.length());

            if(stringNum.length() > 3)
                throw err_code::bad_ip_address;

            nums[index] = parse_int(stringNum);
            break;
        }
    }

    // Error checking
    if(countDots != 3 && s.length() != 0)
        throw err_code::bad_ip_address;

    ip_ = nums;
}

bool IP_address::operator==(const IP_address& that) const
{
    if(that.ip_.size() == ip_.size()) {
        for(int i = 0; i < ip_.size(); ++i) {
            if(that.ip_[i] != ip_[i])
                return false;
        }
        return true;
    }
    else
        return false;
}

int IP_address::first_octad() const
{
    return ip_[0];
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
        : src_(""), dst_("") // [REPLACE THIS LINE BY YOUR HW6 CODE HERE]
{};

IP_address Datagram::get_destination() const
{
    return dst_;
}

string Datagram::get_msg() const
{
    return msg_;
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
