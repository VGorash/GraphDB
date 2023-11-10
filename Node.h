//
// Created by goras on 10.11.2023.
//

#ifndef DB_NODE_H
#define DB_NODE_H

#include <string>
#include <set>


class Node {

    friend class NodeCluster;
    friend class NodeRegistry;

public:
    const std::string &getId() const;
    const std::set<std::pair<std::string, std::string>> &getOutputConnections() const;
    const std::set<std::pair<std::string, std::string>> &getInputConnections() const;

    Node(const Node&) = delete;
    void operator=(const Node&) = delete;

private:
    bool connect(const std::string &connName, const std::string &id, bool reverse = false);
    bool disconnect(const std::string &connName, const std::string &id, bool reverse = false);

    explicit Node(std::string id);
    ~Node() = default;

private:
    std::string m_id;

    std::set<std::pair<std::string, std::string>> m_outputConnections;
    std::set<std::pair<std::string, std::string>> m_inputConnections;
};


#endif //DB_NODE_H
