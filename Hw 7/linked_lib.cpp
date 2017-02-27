#include <iostream>

#include "linked_lib.h"

using namespace std;

ListNode::ListNode()
        : data(nullptr)
{}

ListNode::~ListNode()
{
    if(data != nullptr)
        delete data;
}

// Add a node to the end of linked list
void push_back(List& front, Datagram* data)
{
    List node = make_shared<ListNode>(ListNode());
    node->data = data;

    List lst = front;

    if(lst == nullptr)
        front = node;
    else {
        while(lst->next != nullptr) lst = lst->next;
        lst->next = node;
    }
}

List pop_front(List& front)
{
    if(front == nullptr)
        throw runtime_error("Inputted linked list is a null pointer.");

    List front2 = front;
    front = front->next;
    front2->next = nullptr;

    return front2;
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