//
// Created by goras on 10.11.2023.
//

#include "VertexCluster.h"
#include "Vertex.h"

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

VertexCluster::VertexCluster(std::pair<size_t, size_t> hashRange) : m_hashRange(hashRange) {
    load();
}

VertexCluster::~VertexCluster() {
    for(const auto& it : m_vertices){
        delete it.second;
    }
}

Vertex *VertexCluster::getVertex(const std::string &id) {
    auto it = m_vertices.find(id);
    if(it != m_vertices.end()){
        return m_vertices[id];
    }
    return nullptr;
}

Vertex *VertexCluster::addVertex(const std::string &id) {
    auto it = m_vertices.find(id);
    if(it != m_vertices.end()){
        return nullptr;
    }
    if(m_hasher(id) < m_hashRange.first || m_hasher(id) > m_hashRange.second){
        return nullptr;
    }
    Vertex* vertex = new Vertex(id);
    m_vertices.insert({id, vertex});
    return vertex;
}

bool VertexCluster::deleteVertex(const std::string &id) {
    auto it = m_vertices.find(id);
    if(it != m_vertices.end()){
        Vertex* temp = m_vertices[id];
        m_vertices.erase(id);
        delete temp;
        return true;
    }
    return false;
}

bool VertexCluster::addConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse) {
    Vertex *vertex = getVertex(id1);
    if(!vertex){
        return false;
    }
    return vertex->connect(connName, id2, reverse);
}

bool VertexCluster::removeConnection(const std::string &id1, const std::string &connName, const std::string &id2, bool reverse) {
    Vertex *vertex = getVertex(id1);
    if(!vertex){
        return false;
    }
    return vertex->disconnect(connName, id2, reverse);
}

std::vector<std::string> VertexCluster::getAllIds() const {
    std::vector<std::string> result;
    result.reserve(m_vertices.size());
    for(const auto& n : m_vertices){
        result.push_back(n.first);
    }
    return result;
}

std::pair<size_t, size_t> VertexCluster::getHashRange() const{
    return m_hashRange;
}

bool VertexCluster::dump() const {
    fs::path data_dir("data");
    if(!fs::exists(data_dir)){
        fs::create_directories(data_dir);
    }
    fs::path filepath = data_dir / (std::to_string(m_hashRange.first) + "-" + std::to_string(m_hashRange.second) + ".txt");
    std::ofstream outFile(filepath);
    if(!outFile.is_open()){
        return false;
    }
    outFile << m_vertices.size() << "\n";
    for(auto n : m_vertices){
        outFile << n.second->toString() << "\n";
    }
    if(outFile.fail()){
        return false;
    }
    return true;
}

bool VertexCluster::load() {
    fs::path data_dir("data");
    if(!fs::exists(data_dir)){
        fs::create_directories(data_dir);
    }
    fs::path filepath = data_dir / (std::to_string(m_hashRange.first) + "-" + std::to_string(m_hashRange.second) + ".txt");
    if(!fs::exists(filepath)){
        return false;
    }
    m_vertices.clear();
    std::ifstream inFile(filepath);
    int numVertices;
    std::string VertexString;
    inFile >> numVertices;
    std::getline(inFile, VertexString);
    for(int i=0; i < numVertices; i++){
        std::getline(inFile, VertexString);
        Vertex *vertex = new Vertex("");
        vertex->fillFromString(VertexString);
        m_vertices.insert({vertex->getId(), vertex});
    }
    return true;
}