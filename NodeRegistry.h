//
// Created by goras on 10.11.2023.
//

#ifndef DB_NODEREGISTRY_H
#define DB_NODEREGISTRY_H

#include <string>
#include <vector>

class NodeCluster;
class Node;

class NodeRegistry {

public:
    static NodeRegistry &getInstance();

    const Node* getNode(const std::string& id) const;
    const Node* addNode(const std::string& id) const;
    bool deleteNode(const std::string& id) const;

    bool connectNodes(const std::string &id1, const std::string &connName, const std::string &id2) const;
    bool disconnectNodes(const std::string &id1, const std::string &connName, const std::string &id2) const;

    std::vector<std::string> getAllIds() const;

    NodeRegistry(const NodeRegistry&) = delete;
    void operator=(const NodeRegistry&) = delete;

private:
    NodeRegistry();
    ~NodeRegistry();

private:
    NodeCluster* m_cluster;
};


#endif //DB_NODEREGISTRY_H
