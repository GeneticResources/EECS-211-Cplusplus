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

IP_address Node::get_ip() const
{
    // [YOUR CODE HERE]
}

void Node::connect(const shared_ptr<Node>& x)
{
    // [YOUR CODE HERE]
}

void Node::disconnect(const shared_ptr<Node>& x)
{
    // [YOUR CODE HERE]
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
    n.format(os);
}
