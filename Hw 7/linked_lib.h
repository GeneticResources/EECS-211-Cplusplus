#pragma once

#include "datagram.h"

#include <memory>
#include <iostream>

struct ListNode
{
    Datagram* data;
    std::shared_ptr<ListNode> next;
    ListNode();
    ~ListNode();
};

using List = std::shared_ptr<ListNode>;

void push_back(List& front, Datagram* data);
List pop_front(List& front);
void list_format(std::ostream& os, const List& front);
