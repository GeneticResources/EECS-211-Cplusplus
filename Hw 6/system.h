#pragma once

#include "errors.h"
#include "machines.h"
#include "datagram.h"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

size_t constexpr MAX_MACHINES = 50;

class System
{
    friend class Grader;
private:
    size_t tick_;
    std::array<std::shared_ptr<Node>, MAX_MACHINES> network_;
public:
    System();
    void format(std::ostream&, const std::vector<IP_address>& ips0) const;
    void create_machine(const std::string& type, const std::string& name, const IP_address& ip);
    void delete_machine(const IP_address& ip);
    void connect_machine(const IP_address& ip1, const IP_address& ip2);
    void allocate_datagram(const IP_address& ip1, const IP_address& ip2, const std::string& message);
    void release_datagram(const IP_address& ip);
    void time_click();
};

std::ostream& operator<<(std::ostream&, const System&);
