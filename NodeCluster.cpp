//
// Created by goras on 10.11.2023.
//

#include "NodeCluster.h"
#include "Node.h"

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

NodeCluster::NodeCluster(std::pair<size_t, size_t> hashRange) : m_hashRange(hashRange) {
    load();
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

bool NodeCluster::addConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse) {
    Node *node = getNode(id1);
    if(!node){
        return false;
    }
    return node->connect(connName, id2, reverse);
}

bool NodeCluster::removeConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse) {
    Node *node = getNode(id1);
    if(!node){
        return false;
    }
    return node->disconnect(connName, id2, reverse);
}

std::vector<std::string> NodeCluster::getAllIds() const {
    std::vector<std::string> result;
    result.reserve(m_nodes.size());
    for(const auto& n : m_nodes){
        result.push_back(n.first);
    }
    return result;
}

std::pair<size_t, size_t> NodeCluster::getHashRange() const{
    return m_hashRange;
}

bool NodeCluster::dump() const {
    fs::path data_dir("data");
    if(!fs::exists(data_dir)){
        fs::create_directories(data_dir);
    }
    fs::path filepath = data_dir / (std::to_string(m_hashRange.first) + "-" + std::to_string(m_hashRange.second) + ".txt");
    std::ofstream outFile(filepath);
    if(!outFile.is_open()){
        return false;
    }
    outFile << m_nodes.size() << "\n";
    for(auto n : m_nodes){
        outFile << n.second->toString() << "\n";
    }
    if(outFile.fail()){
        return false;
    }
    return true;
}

bool NodeCluster::load() {
    fs::path data_dir("data");
    if(!fs::exists(data_dir)){
        fs::create_directories(data_dir);
    }
    fs::path filepath = data_dir / (std::to_string(m_hashRange.first) + "-" + std::to_string(m_hashRange.second) + ".txt");
    if(!fs::exists(filepath)){
        return false;
    }
    m_nodes.clear();
    std::ifstream inFile(filepath);
    int numNodes;
    std::string nodeString;
    inFile >> numNodes;
    std::getline(inFile, nodeString);
    for(int i=0; i<numNodes; i++){
        std::getline(inFile, nodeString);
        Node *node = new Node("");
        node->fillFromString(nodeString);
        m_nodes.insert({node->getId(), node});
    }
    return true;
}
