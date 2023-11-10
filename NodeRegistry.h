//
// Created by goras on 10.11.2023.
//

#ifndef DB_NODEREGISTRY_H
#define DB_NODEREGISTRY_H

#include <string>

class NodeCluster;
class Node;

class NodeRegistry {
public:
    static NodeRegistry &getInstance();

    Node* getNode(const std::string& id) const;
    Node* addNode(const std::string& id) const;
    bool deleteNode(const std::string& id) const;

    NodeRegistry(const NodeRegistry&) = delete;
    void operator=(const NodeRegistry&) = delete;

private:
    NodeRegistry();
    ~NodeRegistry();

private:
    NodeCluster* m_cluster;
};


#endif //DB_NODEREGISTRY_H
