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
            if (network_[i] != nullptr && network_[i]->get_ip() == ip) {
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

void System::create_machine(const string& type,
                            const string& name,
                            const IP_address& ip)
{
    // Error checking
    vector<string> types {"laptop", "server", "wan"};
    int index;
    bool equal = false;

    for(int i = 0; i < types.size(); ++i) {
        if(!type.compare(types[i])) {
            index = i;
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

    // Make different types of machines
    if(index == 0)
        network_[i] = make_shared<Laptop>(name, ip);
    else if(index == 1)
        network_[i] = make_shared<Server>(name, ip);
    else
        network_[i] = make_shared<WAN_node>(name, ip);

    cout << "System::create_machine: " << type << ' ' << name << ' ';
    cout << ip;
    cout << '\n';
}

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

void System::connect_machine(const IP_address& ip1, const IP_address& ip2)
{
    // Error checking
    vector<int> indexes {};
    int index1 = 0, index2 = 0;
    for(int i = 0; i < network_.size(); ++i) {
        if(network_[i] != nullptr)
            indexes.push_back(i);
    }
    if(indexes.size() == 0) throw err_code :: no_such_machine;
    for(int i = 0; i < indexes.size(); ++i) {
        if(network_[indexes[i]]->get_ip() == ip1) {
            index1 = indexes[i];
            break;
        }
        else if(i >= indexes.size() - 1)
            throw err_code :: no_such_machine;
    }
    for(int i = 0; i < indexes.size(); ++i) {
        if(network_[indexes[i]]->get_ip() == ip2) {
            index2 = indexes[i];
            break;
        }
        else if(i >= indexes.size() - 1)
            throw err_code :: no_such_machine;
    }

    // If they are compatible, then connect
    if(network_[index1]->can_connect(network_[index2]) && network_[index2]->can_connect(network_[index1])) {
        network_[index1]->connect(network_[index2]);
        network_[index2]->connect(network_[index1]);
    }
    else
        throw err_code :: connect_failed;

    cout << "System::connect_machine: ";
    cout << ip1;
    cout << ", ";
    cout << ip2;
    cout << '\n';
}

void System::allocate_datagram(const IP_address& ip1,
                               const IP_address& ip2,
                               const string& message)
{
    // Error checking
    int machine1 = 0, machine2 = 0;
    vector<int> indexes {};
    for(int i = 0; i < network_.size(); ++i) {
        if(network_[i] != nullptr)
            indexes.push_back(i);
    }
    if(indexes.size() == 0) throw err_code :: no_such_machine;
    for(int i = 0; i < indexes.size(); ++i) {
        if(network_[indexes[i]]->get_ip() == ip1) {
            machine1 = indexes[i];
            break;
        }
        else if(i >= indexes.size() - 1)
            throw err_code :: no_such_machine;
    }
    for(int i = 0; i < indexes.size(); ++i) {
        if(network_[indexes[i]]->get_ip() == ip2) {
            machine2 = indexes[i];
            break;
        }
        else if(i >= indexes.size() - 1)
            throw err_code :: no_such_machine;
    }
    if(dynamic_pointer_cast<Laptop>(network_[machine1]) == nullptr ||
            dynamic_pointer_cast<Laptop>(network_[machine2]) == nullptr)
        throw err_code :: no_such_machine;

    shared_ptr<Laptop> l = dynamic_pointer_cast<Laptop>(network_[machine1]);
    l->allocate_datagram(ip2, message);

    cout << "System::allocate_datagram: ";
    cout << ip1;
    cout << ", ";
    cout << ip2;
    cout << ", \"" << message << "\"\n";
}

void System::release_datagram(const IP_address& ip)
{
    // Error checking
    int machine = 0;
    vector<int> indexes {};
    for(int i = 0; i < network_.size(); ++i) {
        if(network_[i] != nullptr)
            indexes.push_back(i);
    }
    if(indexes.size() == 0) throw err_code :: no_such_machine;
    for(int i = 0; i < indexes.size(); ++i) {
        if(network_[indexes[i]]->get_ip() == ip) {
            machine = indexes[i];
            break;
        }
        else if(i >= indexes.size() - 1)
            throw err_code :: no_such_machine;
    }

    shared_ptr<Laptop> l = dynamic_pointer_cast<Laptop>(network_[machine]);
    if(l == nullptr) throw err_code :: no_such_machine;
    l->release_datagram();

    cout << "System::release_datagram: ";
    cout << ip;
    cout << '\n';
}

void System::time_click()
{
    for(int i = 0; i < network_.size(); ++i) {
        if(network_[i] != nullptr)
            network_[i]->send();
    }
    ++tick_;

    cout << "System::time_click\n";
}

std::ostream& operator<<(std::ostream& os, const System& sys)
{
    sys.format(os, {});
    return os;
}
