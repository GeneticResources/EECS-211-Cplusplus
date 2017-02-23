#include "machines.h"
#include "errors.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>

using namespace std;

void Node::format(ostream& os) const
{
    os << "node {\n";
    os << "    name = \"" << name_ << "\",\n    local_ip = ";
    os << local_ip_;
    os << ",\n    node_list = [ ";
    for (auto& node : node_list_) {
        os << node->get_ip();
        os << ' ';
    }
    os << "],\n    data_list = []";
    os << "\n  }";
}

// Get IP address of machine
IP_address Node::get_ip() const
{
    return local_ip_;
}

// Connect to other machine
void Node::connect(const shared_ptr<Node>& x)
{
    node_list_.push_back(x);
}

// Disconnect from other machine
void Node::disconnect(const shared_ptr<Node>& x)
{
    for(int i = 0; i < node_list_.size(); ++i) {
        if(node_list_[i] == x) {
            node_list_[i] = node_list_.back();
            node_list_.pop_back();
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
    n.format(os);
}
