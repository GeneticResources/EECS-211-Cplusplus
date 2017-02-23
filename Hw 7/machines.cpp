#include "machines.h"
#include "errors.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>

using namespace std;

Node::Node(const std::string& n, IP_address ip)
        : name_(n), local_ip_(ip)
{}

Node::~Node()
{
    // [YOUR CODE HERE]
}

void Node::format(ostream& os) const
{
    os << "node {\n";
    os << "    name = \"" << name_ << "\",\n    local_ip = ";
    os << local_ip_;
    os << ",\n    incoming_ = ";
    if (incoming_ == nullptr) os << "nullptr";
    else                      os << *incoming_;
    os << ",\n    node_list = [ ";
    for (auto& node : node_list_) {
        os << node->get_ip();
        os << ' ';
    }
    os << "],\n    data_list = ";
    list_format(os, data_list_);
    os << "\n  }";
}

IP_address Node::get_ip() const
{
    return local_ip_;
}

void Node::connect(const shared_ptr<Node>& x)
{
    node_list_.push_back(x);
}

void Node::disconnect(const shared_ptr<Node>& x)
{
    for(int i = 0; i < node_list_.size(); ++i) {
        if(node_list_[i] == x) {
            node_list_[i] = node_list_.back();
            node_list_.pop_back();
        }
    }
}

size_t Node::send()
{
    // [YOUR CODE HERE]
}

void Node::receive(Datagram* d)
{
    // [YOUR CODE HERE]
}

void Node::allocate_datagram(const IP_address& dst, const string& message)
{
    // [YOUR CODE HERE]
}

string Node::release_datagram()
{
    // [YOUR CODE HERE]
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
    n.format(os);
    return os;
}
