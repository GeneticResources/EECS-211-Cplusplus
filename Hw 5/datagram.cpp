#include "datagram.h"
#include "parsing.h"
#include "errors.h"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

IP_address parse_IP(string s)
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

    return nums;
}
