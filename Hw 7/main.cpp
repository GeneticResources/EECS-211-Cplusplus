#include "interface.h"
#include "errors.h"
#include "parsing.h"
#include "system.h"

#include <iostream>
#include <string>

using namespace std;

int main()
{
    System sys;
    bool done = false;
    do {
        string cmd_line;
        getline(cin, cmd_line);
        vector<string> tokens = tokenize(cmd_line);
        try {
            done = execute_command(sys, tokens);
        } catch (err_code error_code) {
            error_print(tokens.empty()? "" : tokens[0], error_code);
            done = false;
        }
    } while (!done);
}
