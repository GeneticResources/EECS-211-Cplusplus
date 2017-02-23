#include <iostream>

#include "linked_lib.h"

using namespace std;

ListNode::ListNode()
        // [YOUR CODE HERE]
{}

ListNode::~ListNode()
{
    // [YOUR CODE HERE]
}

void push_back(List& front, Datagram* data)
{
    // [YOUR HW4 CODE HERE]
}

List pop_front(List& front)
{
    // [YOUR HW4 CODE HERE]
}

void list_format(ostream& os, const List& front)
{
    if (front == nullptr) {
        os << "[]";
        return;
    }
    os << "[\n";
    List curr = front;
    while (curr != nullptr) {
        os << "      " << *curr->data;
        curr = curr->next;
        if (curr != nullptr)
            os << ",\n";
    }
    os << "\n    ]";
}
