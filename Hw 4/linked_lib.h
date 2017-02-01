#pragma once

#include <memory>

struct ListNode
{
  int data;
  std::shared_ptr<ListNode> next;
};

using List = std::shared_ptr<ListNode>;

inline List cons(const int data, const List& next)
{
    return std::make_shared<ListNode>(ListNode{data, next});
}

int lengths(List list);
ListNode value(List& list);
bool compareLists(List& list1, List& list2);
void filter_lt(List& front, int limit);
void push_back(List& front, int data);
List pop_front(List& front);
int& nth_element(List& front, size_t n);