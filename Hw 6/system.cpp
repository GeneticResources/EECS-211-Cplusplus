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

// Create a new machine
void System::create_machine(const string& type, const string& name, const IP_address& ip)
{
    // Error checking
    vector<string> types {"laptop", "server", "wan"};
    bool equal = false;
    for(int i = 0; i < types.size(); ++i) {
        if(!type.compare(types[i])) {
            equal = true;
            break;
        }
    }
    if(!equal)
        throw err_code :: unknown_machine_type;

    int i = 0;
    while(network_[i] != nullptr) {
        if(i >= network_.size() - 1)
            throw err_code :: network_full;
        ++i;
    }
    network_[i] = make_shared<Node>(name, ip);
    //cout << "create machine: " << network_[i]->get_ip() << ' ' << i << endl;

    cout << "System::create_machine: " << type << ' ' << name << ' ';
    cout << ip;
    cout << '\n';
}

// Disconnect machine from network
void System::delete_machine(const IP_address& ip)
{
    // Store indexes of existing machines and run through them
    vector<int> indexes {};
    for(int i = 0; i < network_.size(); ++i) {
        if(network_[i] != nullptr)
            indexes.push_back(i);
    }
    if(indexes.size() == 0) throw err_code :: no_such_machine; // Check if network doesn't have any machines
    for(int i = 0; i < indexes.size(); ++i) {
        if(network_[indexes[i]]->get_ip() == ip)
            break;
        else if(i >= indexes.size() - 1)
            throw err_code :: no_such_machine;
    }

    // Find machine with matching ip
    int i = 0;
    while(!(network_[i]->get_ip() == ip)) ++i;

    // Disconnect machines
    for(int a = 0; a < network_.size(); ++a) {
        if(network_[a] != nullptr) {
            network_[a]->disconnect(network_[i]);
            network_[i]->disconnect(network_[a]);
        }
    }
    network_[i] = nullptr;

    cout << "System::delete_machine: ";
    cout << ip;
    cout << '\n';
}

// Connect machines in network to each other
void System::connect_machine(const IP_address& ip1, const IP_address& ip2)
{
    // Store indexes and run through every existing machine
    vector<int> indexes {};
    for(int i = 0; i < network_.size(); ++i) {
        if(network_[i] != nullptr)
            indexes.push_back(i);
    }
    if(indexes.size() == 0) throw err_code :: no_such_machine;
    for(int i = 0; i < indexes.size(); ++i) {
        if(network_[indexes[i]]->get_ip() == ip1)
            break;
        else if(i >= indexes.size() - 1)
            throw err_code :: no_such_machine;
    }
    for(int i = 0; i < indexes.size(); ++i) {
        if(network_[indexes[i]]->get_ip() == ip2)
            break;
        else if(i >= indexes.size() - 1)
            throw err_code :: no_such_machine;
    }

    // Find and connect machines
    int mach1 = 0, mach2 = 0;
    for(int i = 0; i < network_.size(); ++i) {
        if(network_[i] == nullptr) break;
        if(network_[i]->get_ip() == ip1)
            mach1 = i;
        else if(network_[i]->get_ip() == ip2)
            mach2 = i;
    }

    network_[mach1]->connect(network_[mach2]);
    network_[mach2]->connect(network_[mach1]);

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
