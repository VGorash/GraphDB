//
// Created by goras on 10.11.2023.
//

#ifndef DB_NODE_H
#define DB_NODE_H

#include <string>


class Node {

    friend class NodeCluster;

public:
    const std::string &getId();

    Node(const Node&) = delete;
    void operator=(const Node&) = delete;

private:
    Node(std::string id, NodeCluster *parentCluster);
    ~Node() = default;

private:
    std::string m_id;
    NodeCluster *m_parentCluster;
};


#endif //DB_NODE_H
