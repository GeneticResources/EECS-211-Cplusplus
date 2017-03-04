#include "machines.h"
#include "errors.h"
#include "linked_lib.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>

using namespace std;

Node::Node(const string& n, IP_address ip)
        : local_ip_(ip), name_(n)
{}

Node::~Node() {}

void Node::format(ostream& os) const
{
    os << "    name = \"" << name_ << "\",\n    local_ip = " << local_ip_;
}

IP_address Node::get_ip() const
{
    return local_ip_;
}

Laptop::Laptop(const string& n, IP_address ip) : Node(n, ip)
{
    incoming_ = nullptr;
    outgoing_ = nullptr;
}

// Only able to connect to server
bool Laptop::can_connect(const shared_ptr<Node>& x) const
{
    return dynamic_pointer_cast<Server>(x) != nullptr;
}

void Laptop::connect(const shared_ptr<Node>& x)
{
    if(server_ != nullptr || dynamic_pointer_cast<Server>(x) == nullptr)
        throw err_code :: connect_failed;
    server_ = dynamic_pointer_cast<Server>(x);
}

void Laptop::disconnect(const shared_ptr<Node>& x)
{
    server_ = nullptr;
}

size_t Laptop::send()
{
    size_t success = 0;

    server_->receive(outgoing_);

    outgoing_ = nullptr;
    ++success;
    return success;
}

void Laptop::receive(Datagram* d)
{
    if(incoming_ != nullptr || !(d->get_destination() == get_ip()))
        throw err_code :: recv_blocked;
    if(d->get_destination() == get_ip())
        incoming_ = d;
}

void Laptop::allocate_datagram(const IP_address& dst,
                               const string& message)
{
    if(outgoing_ != nullptr)
        err_code :: send_blocked;
    else
        outgoing_ = new Datagram(get_ip(), dst, message);
}

string Laptop::release_datagram()
{
    if(incoming_ != nullptr) {
        string s = incoming_->get_msg();
        delete incoming_;
        incoming_ = nullptr;
        return s;
    } else return "";
}

void Laptop::format(ostream& os) const
{
    os << "laptop {\n";
    Node::format(os);
    os << ",\n    incoming = ";
    if (incoming_) os << *incoming_;
    else           os << "nullptr";
    os << ",\n    outgoing = ";
    if (outgoing_) os << *outgoing_;
    else           os << "nullptr";
    os << ",\n    server = ";
    if (server_ != nullptr) os << server_->get_ip();
    else                    os << "nullptr";
    os << "\n  }";
}

Laptop::~Laptop()
{
    if(incoming_ != nullptr)
        delete incoming_;
    if(outgoing_ != nullptr)
        delete outgoing_;
}


Server::Server(const string& n, IP_address a)
        : Node(n, a), num_laptops_(0), num_wans_(0)
{}

// Only able to connect to 8 laptops and 4 WANs
bool Server::can_connect(const shared_ptr<Node>& x) const
{
    return dynamic_pointer_cast<Laptop>(x) != nullptr || dynamic_pointer_cast<WAN_node>(x) != nullptr;
}

void Server::connect(const shared_ptr<Node>& x)
{
    if(dynamic_pointer_cast<Laptop>(x) != nullptr) {
        ++num_laptops_;
        if(num_laptops_ > 8) {
            --num_laptops_;
            throw err_code :: connect_failed;
        }
    }
    else if(dynamic_pointer_cast<WAN_node>(x) != nullptr) {
        ++num_wans_;
        if(num_wans_ > 4) {
            --num_wans_;
            throw err_code :: connect_failed;
        }
    }

    node_list_.push_back(x);
}

void Server::disconnect(const shared_ptr<Node>& x)
{
    for(int i = 0; i < node_list_.size(); ++i) {
        if(node_list_[i] == x) {
            node_list_[i] = node_list_.back();
            node_list_.pop_back();
        }
    }
    if(dynamic_pointer_cast<Laptop>(x) != nullptr)
        --num_laptops_;
    else if(dynamic_pointer_cast<WAN_node>(x) != nullptr)
        --num_wans_;
}

void Server::receive(Datagram* d)
{
    push_back(data_list_, d);
}

void Server::format(ostream& os) const
{
    os << "server {\n";
    Node::format(os);
    os << ",\n    node_list = [ ";
    for (auto& node : node_list_)
        os << node->get_ip() << ' ';
    os << "],\n    data_list = ";
    list_format(os, data_list_);
    os << "\n  }";
}

size_t Server::send()
{
    int diff = 0, min = 0, minIndex = 0;
    bool foundMachine, first;
    size_t success = 0;
    List failedSend;

    if(node_list_.size() != 0) {

        // Loop through every datagram in data_list_
        while(data_list_ != nullptr) {
            foundMachine = false;

            // Check if the destination ip equals machine ip and laptop
            for(int i = 0; i < node_list_.size(); ++i) {
                if(data_list_->data->get_destination() == node_list_[i]->get_ip() &&
                        dynamic_pointer_cast<Laptop>(node_list_[i]) != nullptr) {

                    foundMachine = true;
                    List head = pop_front(data_list_);

                    // If buffer is full, then don't send
                    try {
                        node_list_[i]->receive(head->data);
                    } catch (err_code e){
                        push_back(failedSend, head->data);
                        break;
                    }

                    head->data = nullptr;
                    ++success;
                    break;
                }
            }

            // If no matching machine
            if(!foundMachine) {
                first = true;

                // Find all WAN machines, store locations in vector
                vector<int> wanVec {};
                for(int i = 0; i < node_list_.size(); ++i) {
                    if(dynamic_pointer_cast<WAN_node>(node_list_[i]) != nullptr)
                        wanVec.push_back(i);
                }

                for (int i = 0; i < wanVec.size(); ++i) {

                    if (first) {
                        min = node_list_[wanVec[0]]->get_ip().first_octad() - data_list_->data->get_destination().first_octad();
                        first = false;
                    }
                    diff = node_list_[wanVec[i]]->get_ip().first_octad() - data_list_->data->get_destination().first_octad();

                    // Keep positive
                    if (diff < 0)
                        diff = -diff;
                    if (min < 0)
                        min = -min;

                    if (diff < min || min == diff) {
                        min = diff;
                        minIndex = wanVec[i]; // Index of WAN machine (in node_list_)
                    }
                }

                List head = pop_front(data_list_);

                // Send datagram to minimum octad difference
                try {
                    node_list_[minIndex]->receive(head->data);
                } catch (err_code e){
                    push_back(failedSend, head->data);
                    continue;
                }

                head->data = nullptr;
                ++success;
            }
        }
        data_list_ = failedSend;
    }

    return success;
}

Server::~Server()
{
}


WAN_node::WAN_node(const string& n, IP_address a)
        : Node(n, a), num_servers_(0), num_wans_(0)
{}

// Only able to connect to 4 WANs and 4 servers
bool WAN_node::can_connect(const shared_ptr<Node>& x) const
{
    return dynamic_pointer_cast<Server>(x) != nullptr || dynamic_pointer_cast<WAN_node>(x) != nullptr;
}

void WAN_node::connect(const shared_ptr<Node>& x)
{
    if(dynamic_pointer_cast<Server>(x) != nullptr) {
        ++num_servers_;
        if(num_servers_ > 4) {
            --num_servers_;
            throw err_code :: connect_failed;
        }
    }
    else if(dynamic_pointer_cast<WAN_node>(x) != nullptr) {
        ++num_wans_;
        if(num_wans_ > 4) {
            --num_wans_;
            throw err_code :: connect_failed;
        }
    }

    node_list_.push_back(x);
}

void WAN_node::disconnect(const shared_ptr<Node>& x)
{
    for(int i = 0; i < node_list_.size(); ++i) {
        if(node_list_[i] == x) {
            node_list_[i] = node_list_.back();
            node_list_.pop_back();
        }
    }
    if(dynamic_pointer_cast<Server>(x) != nullptr)
        --num_servers_;
    else if(dynamic_pointer_cast<WAN_node>(x) != nullptr)
        --num_wans_;
}

void WAN_node::receive(Datagram* d)
{
    push_back(data_list_, d);
}


void WAN_node::format(ostream& os) const
{
    os << "WAN_node {\n";
    Node::format(os);
    os << ",\n    node_list = [ ";
    for (auto& node : node_list_)
        os << node->get_ip() << ' ';
    os << "],\n    data_list = ";
    list_format(os, data_list_);
    os << "\n  }";
}

size_t WAN_node::send()
{
    int diff = 0, min = 0, minIndex = 0;
    bool foundMachine, first = true;
    size_t success = 0;
    List failedSend;

    if(node_list_.size() != 0) {

        while(data_list_ != nullptr) {

            // Find machine with minimum difference
            for (int i = 0; i < node_list_.size(); ++i) {
                if (first) {
                    min = node_list_[0]->get_ip().first_octad() - data_list_->data->get_destination().first_octad();
                    first = false;
                }
                diff = node_list_[i]->get_ip().first_octad() - data_list_->data->get_destination().first_octad();

                // Keep positive
                if (diff < 0)
                    diff = -diff;
                if (min < 0)
                    min = -min;

                if (diff < min || min == diff) {
                    min = diff;
                    minIndex = i;
                }
            }

            first = true;
            List head = pop_front(data_list_);

            // Send datagram to minimum octad difference
            node_list_[minIndex]->receive(head->data);

            head->data = nullptr;
            ++success;
        }
        data_list_ = failedSend;
    }
    return success;
}

WAN_node::~WAN_node()
{
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
    n.format(os);
    return os;
}
