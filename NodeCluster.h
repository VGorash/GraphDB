//
// Created by goras on 10.11.2023.
//

#ifndef DB_NODECLUSTER_H
#define DB_NODECLUSTER_H

#include <unordered_map>
#include <string>
#include <vector>

class Node;

class NodeCluster {

    friend class NodeRegistry;

public:
    NodeCluster(const NodeCluster&) = delete;
    void operator=(const NodeCluster&) = delete;

private:
    Node* getNode(const std::string& id);
    Node* addNode(const std::string& id);
    bool deleteNode(const std::string& id);

    bool addConnection(const std::string& id1, const std::string& connName, const std::string& id2, bool reverse=false);
    bool removeConnection(const std::string& id1, const std::string& connName, const std::string& id2, bool reverse=false);

    std::vector<std::string> getAllIds() const;

    bool dump() const;
    bool load();
    std::pair<size_t, size_t> getHashRange() const;

    explicit NodeCluster(std::pair<size_t, size_t> hashRange);
    ~NodeCluster();

private:
    std::hash<std::string> m_hasher;
    std::unordered_map<std::string, Node*> m_nodes;

    std::pair<size_t, size_t> m_hashRange;
};


#endif //DB_NODECLUSTER_H
