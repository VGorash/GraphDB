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
    delete m_cluster;
}

Node *NodeRegistry::getNode(const std::string &id) const {
    return m_cluster->getNode(id);
}

Node *NodeRegistry::addNode(const std::string &id) const {
    return m_cluster->addNode(id);
}

bool NodeRegistry::deleteNode(const std::string &id) const {
    return m_cluster->deleteNode(id);
}

NodeRegistry &NodeRegistry::getInstance() {
    static NodeRegistry instance;
    return instance;
}
