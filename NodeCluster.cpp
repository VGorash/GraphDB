//
// Created by goras on 10.11.2023.
//

#include "NodeCluster.h"
#include "Node.h"

#include <string>

NodeCluster::NodeCluster(std::pair<size_t, size_t> hashRange) : m_hashRange(hashRange) {

}

NodeCluster::~NodeCluster() {
    for(const auto& it : m_nodes){
        delete it.second;
    }
}

Node *NodeCluster::getNode(const std::string &id) {
    auto it = m_nodes.find(id);
    if(it != m_nodes.end()){
        return m_nodes[id];
    }
    return nullptr;
}

Node *NodeCluster::addNode(const std::string &id) {
    auto it = m_nodes.find(id);
    if(it != m_nodes.end()){
        return nullptr;
    }
    if(m_hasher(id) < m_hashRange.first || m_hasher(id) > m_hashRange.second){
        return nullptr;
    }
    Node* node = new Node(id);
    m_nodes.insert({id, node});
    return node;
}

bool NodeCluster::deleteNode(const std::string &id) {
    auto it = m_nodes.find(id);
    if(it != m_nodes.end()){
        Node* temp = m_nodes[id];
        m_nodes.erase(id);
        delete temp;
        return true;
    }
    return false;
}


std::pair<size_t, size_t> NodeCluster::getHashRange() {
    return m_hashRange;
}
