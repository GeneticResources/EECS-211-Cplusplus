// DO NOT MODIFY THIS FILE. We will replace this file by our version when
// grading this homework.
#pragma once

#include "linked_lib.h"
#include "datagram.h"

#include <vector>
#include <memory>

class Node
{
    friend class Grader;
private:
    std::string name_;
    IP_address  local_ip_;

public:
    Node(const std::string& n, IP_address ip);
    virtual ~Node();

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    IP_address get_ip() const;
    virtual void format(std::ostream&) const;

    virtual bool can_connect(const std::shared_ptr<Node>& x) const = 0;
    virtual void connect(const std::shared_ptr<Node>& x) = 0;
    virtual void disconnect(const std::shared_ptr<Node>& x) = 0;
    virtual void receive(Datagram* d) = 0;
    virtual size_t send() = 0;
};

class Laptop;
class Server;
class WAN_node;

class Laptop : public Node
{
    friend class Grader;
private:
    Datagram* incoming_;
    Datagram* outgoing_;
    std::shared_ptr<Server> server_;

public:
    Laptop(const std::string& n, IP_address ip);
    ~Laptop() override;

    void format(std::ostream& os) const override;

    bool can_connect(const std::shared_ptr<Node>& x) const override;
    void connect(const std::shared_ptr<Node>& x) override;
    void disconnect(const std::shared_ptr<Node>& x) override;
    void receive(Datagram* d) override;
    size_t send() override;

    void allocate_datagram(const IP_address& dst,
                           const std::string& message);
    std::string release_datagram();
};

class Server : public Node
{
    friend class Grader;
private:
    size_t num_laptops_;
    size_t num_wans_;
    std::vector<std::shared_ptr<Node>> node_list_;
    List data_list_;

public:
    Server(const std::string& n, IP_address a);
    ~Server() override;

    void format(std::ostream& os) const override;

    bool can_connect(const std::shared_ptr<Node>& x) const override;
    void connect(const std::shared_ptr<Node>& x) override;
    void disconnect(const std::shared_ptr<Node>& x) override;
    void receive(Datagram* d) override;
    size_t send() override;
};

class WAN_node : public Node
{
    friend class Grader;
private:
    size_t num_servers_;
    size_t num_wans_;
    std::vector<std::shared_ptr<Node>> node_list_;
    List data_list_;

public:
    WAN_node(const std::string& n, IP_address a);
    ~WAN_node() override;

    void format(std::ostream& os) const override;

    bool can_connect(const std::shared_ptr<Node>& x) const override;
    void connect(const std::shared_ptr<Node>& x) override;
    void disconnect(const std::shared_ptr<Node>& x) override;
    void receive(Datagram* d) override;
    size_t send() override;
};

std::ostream& operator<<(std::ostream&, const Node&);
