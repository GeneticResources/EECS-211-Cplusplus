#include "parsing.h"
#include "interface.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int parse_int(const string& s) {
    
}

vector<string> tokenize(const string& line)
{
    int index = 0;
    string str = "";
    bool search = false;

    // Create a vector of line's characters
    vector<string> characters, tokens;
    for(int i = 0; i < line.length(); ++i) {
        characters.push_back(line.substr(i, 1));
    }

    while(true) {
        // Break when out of bounds
        if(index == line.length())
            break;

        // If there's a quote, run through until another quote is found
        if(characters[index] == "\"") {
            ++index;
            search = true;

            while(characters[index] != "\"") {
                str += characters[index];
                ++index;
            }
        }
        // Otherwise just look for non-spaces
        else if(characters[index] != " ") {
            search = true;

            while(characters[index] != " ") {
                str += characters[index];
                ++index;
            }
        }

        if(search)
            tokens.push_back(str);
        search = false;
        str = "";
        ++index;
    }
    return tokens;
}
