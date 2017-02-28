#include "machines.h"
#include "errors.h"

#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>

using namespace std;

Node::Node(const std::string& n, IP_address ip)
        : name_(n), local_ip_(ip), incoming_(nullptr)
{}

Node::~Node()
{
    if(incoming_ != nullptr)
        delete incoming_;
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

    int diff = 0, min = 0, minIndex = 0;
    bool foundMachine, first = true;
    size_t success = 0;

    // Make a copy of data_list_ or not?
    List failedSend;

    if(node_list_.size() != 0) {

        // For every datagram in data_list_, find where connected machine ip equals destination ip
        while(data_list_ != nullptr) {
            foundMachine = false;

            // Check if the destination ip equals machine ip
            for(int i = 0; i < node_list_.size(); ++i) {
                if(data_list_->data->get_destination() == node_list_[i]->get_ip()) {
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

// Ask about changing data_list_ and copy of data_list_
void Node::receive(Datagram* d)
{
    if(d->get_destination() == local_ip_) {
        if(incoming_ == nullptr)
            incoming_ = d;
        else
            throw err_code::recv_blocked;
    } else
        push_back(data_list_, d);
}

void Node::allocate_datagram(const IP_address& dst, const string& message)
{
    Datagram* newPtr = new Datagram(local_ip_, dst, message);
    push_back(data_list_, newPtr);
}

string Node::release_datagram()
{
    if(incoming_ != nullptr) {
        string s = incoming_->get_msg();
        delete incoming_;
        incoming_ = nullptr;
        return s;
    } else return "";
}

std::ostream& operator<<(std::ostream& os, const Node& n)
{
    n.format(os);
    return os;
}
