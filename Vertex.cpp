//
// Created by goras on 10.11.2023.
//

#include "utils.h"
#include "Vertex.h"

#include <utility>

Vertex::Vertex(std::string id) : m_id(std::move(id)) {}

const std::string &Vertex::getId() const {
    return m_id;
}

const std::set<std::pair<std::string, std::string>> &Vertex::getOutputConnections() const {
    return m_outputConnections;
}

const std::set<std::pair<std::string, std::string>> &Vertex::getInputConnections() const {
    return m_inputConnections;
}

bool Vertex::connect(const std::string &connName, const std::string &id, bool reverse) {
    std::set<std::pair<std::string, std::string>> &conns = reverse ? m_inputConnections : m_outputConnections;

    if(conns.find({connName, id}) != conns.end()){
        return false;
    }
    conns.insert({connName, id});
    return true;
}

bool Vertex::disconnect(const std::string &connName, const std::string &id, bool reverse) {
    std::set<std::pair<std::string, std::string>> &conns = reverse ? m_inputConnections : m_outputConnections;

    if(conns.find({connName, id}) == conns.end()){
        return false;
    }
    conns.erase({connName, id});
    return true;
}

const std::string Vertex::toString() const {
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

bool Vertex::fillFromString(const std::string &inputString) {
    m_inputConnections.clear();
    m_outputConnections.clear();
    auto parsedVertex = split(inputString, '|');
    m_id = parsedVertex[0];
    for(const auto& c : split(parsedVertex[1], ';')){
        if(c.empty()){
            continue;
        }
        auto connection = split(c, ',');
        m_outputConnections.insert({connection[0], connection[1]});
    }
    for(const auto& c : split(parsedVertex[2], ';')){
        if(c.empty()){
            continue;
        }
        auto connection = split(c, ',');
        m_inputConnections.insert({connection[0], connection[1]});
    }
    return true;
}

