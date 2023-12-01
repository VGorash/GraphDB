//
// Created by goras on 10.11.2023.
//

#ifndef DB_VERTEX_H
#define DB_VERTEX_H

#include <string>
#include <set>


class Vertex {

    friend class VertexStorageImpl;

public:
    ~Vertex() = default;

    Vertex(const Vertex &);

    const std::string &getId() const;

    const std::set<std::pair<std::string, std::string>> &getOutputConnections() const;

    const std::set<std::pair<std::string, std::string>> &getInputConnections() const;

    std::string toString() const;

    static Vertex fromString(const std::string &inputString);

private:
    void connect(const std::string &connName, const std::string &id, bool reverse = false);

    void disconnect(const std::string &connName, const std::string &id, bool reverse = false);

    void fillFromString(const std::string &inputString);

    explicit Vertex(std::string id);

private:
    std::string m_id;

    std::set<std::pair<std::string, std::string>> m_outputConnections;
    std::set<std::pair<std::string, std::string>> m_inputConnections;
};


#endif //DB_VERTEX_H
