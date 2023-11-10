//
// Created by goras on 10.11.2023.
//

#include "Node.h"

#include <utility>

Node::Node(std::string id) : m_id(std::move(id)) {}

const std::string &Node::getId() const {
    return m_id;
}

const std::set<std::pair<std::string, std::string>> &Node::getOutputConnections() const {
    return m_outputConnections;
}

const std::set<std::pair<std::string, std::string>> &Node::getInputConnections() const {
    return m_inputConnections;
}

bool Node::connect(const std::string &connName, const std::string &id, bool reverse) {
    std::set<std::pair<std::string, std::string>> &conns = reverse ? m_inputConnections : m_outputConnections;

    if(conns.find({connName, id}) != conns.end()){
        return false;
    }
    conns.insert({connName, id});
    return true;
}

bool Node::disconnect(const std::string &connName, const std::string &id, bool reverse) {
    std::set<std::pair<std::string, std::string>> &conns = reverse ? m_inputConnections : m_outputConnections;

    if(conns.find({connName, id}) == conns.end()){
        return false;
    }
    conns.erase({connName, id});
    return true;
}

