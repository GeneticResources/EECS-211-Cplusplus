#include "system.h"
#include "machines.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;

System::System()
        : tick_(0) {}

void System::format(std::ostream& os, const vector<IP_address>& ips0) const
{
    vector<IP_address> ips = ips0;
    if (ips.empty()) {
        for (size_t i = 0; i < network_.size(); ++i)
            if (network_[i] != nullptr)
                ips.push_back(network_[i]->get_ip());
    }
    os << "system {\n  tick = " << tick_ << ",\n";
    for (IP_address& ip : ips) {
        os << "  ";
        os << ip;
        os << " = ";
        int idx = -1;
        for (int i = 0; i < MAX_MACHINES; ++i) {
            if (network_[i]->get_ip() == ip) {
                idx = i;
                break;
            }
        }
        if (idx < 0)
            throw err_code::no_such_machine;
        os << *network_[idx];
        os << '\n';
    }
    os << "}\n";
}

void System::create_machine(const string& type, const string& name, const IP_address& ip)
{
    // [YOUR CODE HERE]
    cout << "System::create_machine: " << type << ' ' << name << ' ';
    cout << ip;
    cout << '\n';
}

void System::delete_machine(const IP_address& ip)
{
    // [YOUR CODE HERE]
    cout << "System::delete_machine: ";
    cout << ip;
    cout << '\n';
}

void System::connect_machine(const IP_address& ip1, const IP_address& ip2)
{
    // [YOUR CODE HERE]
    cout << "System::connect_machine: ";
    cout << ip1;
    cout << ", ";
    cout << ip2;
    cout << '\n';
}

void System::allocate_datagram(const IP_address& ip1, const IP_address& ip2, const string& message)
{
    cout << "System::allocate_datagram: ";
    cout << ip1;
    cout << ", ";
    cout << ip2;
    cout << ", \"" << message << "\"\n";
}

void System::release_datagram(const IP_address& ip)
{
    cout << "System::release_datagram: ";
    cout << ip;
    cout << '\n';
}

void System::time_click()
{
    ++tick_;
    cout << "System::time_click\n";
}

std::ostream& operator<<(std::ostream& os, const System& sys)
{
    sys.format(os, {});
    return os;
}
