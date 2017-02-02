#include "linked_lib.h"

#include <memory>
#include <stdexcept>
#include <iostream>
#include <vector>

using namespace std;

// Get length of linked list
int lengths(List list) {
    int length = 0;
    for(List curr = list; curr != nullptr; curr = curr->next) {
        length++;
    }
    return length;
}

// Compare whether pointer values (structs) are equal
bool compareLists(List& list1, List& list2) {

    // Only check data if lengths are equal
    if(lengths(list1) == lengths(list2)) {
        for (List curr = list1; curr != nullptr; curr = curr->next) {
            if (curr->data == list2->data)
                list2 = list2->next;
            else
                return false;
        }
        return true;
    }
    return false;
}

// Filters out nodes with data greater than or equal to limit
void filter_lt(List& front, int limit)
{
    List ptrHolder = nullptr;
    vector<List> listVec;

    // Store Lists that have data values less than limit into a new List
    // Use push_back to store
    for(List curr = front; curr != nullptr; curr = curr->next) {
        if(curr->data < limit)
            push_back(ptrHolder, curr->data);
    }
    if(front != nullptr)
        front = ptrHolder;
}

// Adds a node to the end of the linked list
void push_back(List& front, int data)
{
    List newList = make_shared<ListNode>(ListNode{data, nullptr});
    List lst = front;

    if(lst == nullptr)
        front = newList;
    else {
        while(lst->next != nullptr) lst = lst->next;
        lst->next = newList;
    }
}

// Removes the first node of the linked list
List pop_front(List& front)
{
    if(front == nullptr)
        throw runtime_error("Inputted linked list is a null pointer.");

    front->next = nullptr;
    return front;
}

// Returns the data of the n+1th node
int& nth_element(List& front, size_t n)
{
    size_t count = 0;

    if(n >= lengths(front))
        throw runtime_error("N is out of bounds.");

    for(List curr = front; curr != nullptr; curr = curr->next) {
        if (count == n)
            return curr->data;
        count++;
    }
}
