//
// Created by goras on 10.11.2023.
//

#ifndef DB_NODECLUSTER_H
#define DB_NODECLUSTER_H

#include <unordered_map>
#include <string>

class Node;

class NodeCluster {

    friend class NodeRegistry;

public:
    Node* getNode(const std::string& id);
    Node* addNode(const std::string& id);
    bool deleteNode(const std::string& id);

    std::pair<size_t, size_t> getHashRange();

    NodeCluster(const NodeCluster&) = delete;
    void operator=(const NodeCluster&) = delete;

private:
    explicit NodeCluster(std::pair<size_t, size_t> hashRange);
    ~NodeCluster();

private:
    std::hash<std::string> m_hasher;
    std::unordered_map<std::string, Node*> m_nodes;

    std::pair<size_t, size_t> m_hashRange;
};


#endif //DB_NODECLUSTER_H
