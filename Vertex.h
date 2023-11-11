//
// Created by goras on 10.11.2023.
//

#ifndef DB_VERTEX_H
#define DB_VERTEX_H

#include <string>
#include <set>


class Vertex {

    friend class VertexCluster;

public:
    const std::string &getId() const;
    const std::set<std::pair<std::string, std::string>> &getOutputConnections() const;
    const std::set<std::pair<std::string, std::string>> &getInputConnections() const;

    const std::string toString() const;

    Vertex(const Vertex&) = delete;
    void operator=(const Vertex&) = delete;

private:
    bool connect(const std::string &connName, const std::string &id, bool reverse = false);
    bool disconnect(const std::string &connName, const std::string &id, bool reverse = false);

    bool fillFromString(const std::string &inputString);

    explicit Vertex(std::string id);
    ~Vertex() = default;

private:
    std::string m_id;

    std::set<std::pair<std::string, std::string>> m_outputConnections;
    std::set<std::pair<std::string, std::string>> m_inputConnections;
};


#endif //DB_VERTEX_H
