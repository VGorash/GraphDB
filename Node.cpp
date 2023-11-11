//
// Created by goras on 10.11.2023.
//

#include "utils.h"
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

const std::string Node::toString() const {
    std::string result;
    result += m_id;
    result += "|";
    for(auto c : m_outputConnections){
        result += c.first + "," + c.second;
        result += ";";
    }
    result += "|";
    for(auto c : m_inputConnections){
        result += c.first + "," + c.second;
        result += ";";
    }
    return result;
}

bool Node::fillFromString(const std::string &inputString) {
    m_inputConnections.clear();
    m_outputConnections.clear();
    auto parsedNode = split(inputString, '|');
    m_id = parsedNode[0];
    for(const auto& c : split(parsedNode[1], ';')){
        if(c.empty()){
            continue;
        }
        auto connection = split(c, ',');
        m_outputConnections.insert({connection[0], connection[1]});
    }
    for(const auto& c : split(parsedNode[2], ';')){
        if(c.empty()){
            continue;
        }
        auto connection = split(c, ',');
        m_inputConnections.insert({connection[0], connection[1]});
    }
    return true;
}

