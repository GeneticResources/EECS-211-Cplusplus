#include "errors.h"

#include <string>
#include <iostream>

using namespace std;

void error_print(const string& cmd, err_code errcode)
{
    cout << cmd << ": ";
    if (errcode == err_code::cmd_undefined) {
        cout << "command not found";
    } else if (errcode == err_code::syntax_error) {
        cout << "syntax error";
    } else if (errcode == err_code::bad_ip_address) {
        cout << "bad IP address";
    } else if (errcode == err_code::unknown_machine_type) {
        cout << "unknown machine type";
    } else if (errcode == err_code::network_full) {
        cout << "network is full";
    } else if (errcode == err_code::no_such_machine) {
        cout << "no such machine";
    } else if (errcode == err_code::connect_failed) {
        cout << "fail to connect";
    } else if (errcode == err_code::send_blocked) {
        cout << "outgoing buffer is full";
    } else if (errcode == err_code::recv_blocked) {
        cout << "incoming buffer is full";
    }
    cout << '\n';
}

