// DO NOT MODIFY THIS FILE. We will provide a new version of machines.h in the
// future to replace this file.
#pragma once

#include "linked_lib.h"
#include "datagram.h"

#include <vector>
#include <memory>
#include <iostream>

class Node
{
    friend class Grader;
private:
    std::string name_;
    IP_address  local_ip_;
    std::vector<std::shared_ptr<Node>> node_list_;
    Datagram* incoming_;
    List data_list_;

public:
    Node(const std::string& n, IP_address ip);
    ~Node();

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    IP_address get_ip() const;
    void format(std::ostream&) const;

    void connect(const std::shared_ptr<Node>& x);
    void disconnect(const std::shared_ptr<Node>& x);
    void receive(Datagram* d);
    size_t send();

    void allocate_datagram(const IP_address& dst, const std::string& message);
    std::string release_datagram();
};

std::ostream& operator<<(std::ostream&, const Node&);
