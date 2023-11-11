//
// Created by goras on 10.11.2023.
//

#include "NodeRegistry.h"
#include "NodeCluster.h"
#include "Node.h"

#include <string>

NodeRegistry::NodeRegistry() {
    m_cluster = new NodeCluster({0, (size_t)-1});
}

NodeRegistry::~NodeRegistry() {
    m_cluster->dump();
    delete m_cluster;
}

const Node *NodeRegistry::getNode(const std::string &id) const {
    return m_cluster->getNode(id);
}

const Node *NodeRegistry::addNode(const std::string &id) const {
    return m_cluster->addNode(id);
}

bool NodeRegistry::deleteNode(const std::string &id) const {
    const Node *node = getNode(id);
    if(!node){
        return false;
    }
    for(auto c : std::set<std::pair<std::string, std::string>>(node->getOutputConnections())){
        disconnectNodes(id, c.first, c.second);
    }
    for(auto c : std::set<std::pair<std::string, std::string>>(node->getInputConnections())){
        disconnectNodes(c.second, c.first, id);
    }
    return m_cluster->deleteNode(id);
}

NodeRegistry &NodeRegistry::getInstance() {
    static NodeRegistry instance;
    return instance;
}

bool NodeRegistry::connectNodes(const std::string &id1, const std::string &connName, const std::string &id2) const {
    const Node *node1 = getNode(id1);
    const Node *node2 = getNode(id2);
    if(!node1 || !node2){
        return false;
    }
    bool res = true;
    res &= m_cluster->addConnection(node1->getId(), connName, node2->getId());
    res &= m_cluster->addConnection(node2->getId(), connName, node1->getId(), true);
    return res;
}

bool NodeRegistry::disconnectNodes(const std::string &id1, const std::string &connName, const std::string &id2) const {
    const Node *node1 = getNode(id1);
    const Node *node2 = getNode(id2);
    if(!node1 || !node2){
        return false;
    }
    bool res = true;
    res &= m_cluster->removeConnection(node1->getId(), connName, node2->getId());
    res &= m_cluster->removeConnection(node2->getId(), connName, node1->getId(), true);
    return res;
}

std::vector<std::string> NodeRegistry::getAllIds() const {
    return m_cluster->getAllIds();
}
