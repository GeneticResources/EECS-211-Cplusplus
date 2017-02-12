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

    // Check errors
    for(int i = 0; i < s.length(); ++i) {
        int ascii = (int) s.at(i);

        // If not between 0-9 and is not a period
        if(!(ascii >= 48 && ascii <= 57) && ascii != 46)
            throw runtime_error("String does not follow specifications.");
    }

    for(int i = 0; i < s.length(); ++i) {
        if(s.at(i) == '.') {
            stringNum = s.substr(lastStartIndex, i - lastStartIndex);
            lastStartIndex = i + 1;

            // Error checking
            if(parse_int(stringNum) < 0 || parse_int(stringNum) > 255)
                throw runtime_error("Numbers cannot be less than 0 or greator than 255");

            nums[index] = parse_int(stringNum);
            ++index;
        }
        if(index == 3) {
            stringNum = s.substr(lastStartIndex, s.length());
            nums[index] = parse_int(stringNum);
            break;
        }
    }

    return nums;
}
