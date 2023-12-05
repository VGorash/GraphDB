//
// Created by goras on 04.12.2023.
//

#include <regex>
#include <sstream>
#include "DatabaseServer.h"
#include "../registry/VertexRegistry.h"
#include "../utils/string_utils.h"
#include "../vertex/Vertex.h"
#include "../vertex/VertexExceptions.h"

bool check_id(const std::string &id) {
    std::regex pattern("^[A-Za-z0-9_]+$");
    return std::regex_match(id, pattern);
}

bool checkTwoArgs(const std::vector<std::string> &args, std::stringstream &out) {
    if (args.size() != 2) {
        out << "Expected 1 argument " << args.size() - 1 << " provided\n";
        return false;
    }
    const std::string &id = args[1];
    if (id.empty() || !check_id(id)) {
        out << "ID must contain only letters, digits and '_' symbol\n";
        return false;
    }
    return true;
}

bool checkFourArgs(const std::vector<std::string> &args, std::stringstream &out, bool query = false) {
    if (args.size() != 4) {
        out << "Expected 3 arguments " << args.size() - 1 << " provided\n";
        return false;
    }
    const std::string &id1 = args[1];
    const std::string &connName = args[2];
    const std::string &id2 = args[3];
    if ((id1.empty() || !check_id(id1)) && (!query || id1 != "?")) {
        out << "ID must contain only letters, digits and '_' symbol\n";
        return false;
    }
    if (id2.empty() || !check_id(id2) && (!query || id2 != "?")) {
        out << "ID must contain only letters, digits and '_' symbol\n";
        return false;
    }
    if (connName.empty() || !check_id(connName) && (!query || connName != "?")) {
        out << "Connection name must contain only letters, digits and '_' symbol\n";
        return false;
    }
    return true;
}

void printVertex(const Vertex &vertex, std::stringstream &out) {
    out << "ID: " << vertex.getId() << "\n";
    out << "Output connections: " << "\n";
    for (const auto &c: vertex.getOutputConnections()) {
        out << "\t --(" << c.first << ")--> " << c.second << "\n";
    }
    out << "Input connections: " << "\n";
    for (const auto &c: vertex.getInputConnections()) {
        out << "\t <--(" << c.first << ")-- " << c.second << "\n";
    }
}

void
printConnection(const std::string &id1, const std::string &connName, const std::string &id2, std::stringstream &out) {
    out << id1 << " --(" << connName << ")--> " << id2;
}

void processVertexException(VertexOperationException &e, std::stringstream &out) {
    switch (e.code) {
        case VertexAlreadyExists:
            out << "Vertex with id " << e.vertexId << " already exists in database\n";
            break;
        case VertexNotFound:
            out << "Vertex with id " << e.vertexId << " not found in database\n";
            break;
        case VertexHasConnections:
            out << "Cannot delete vertex with id " << e.vertexId << " because it has connections\n";
            break;
    }
}

void processConnectionException(ConnectionOperationException &e, std::stringstream &out) {
    switch (e.code) {
        case ConnectionAlreadyExists:
            out << "Connection ";
            printConnection(e.vertexId1, e.vertexId1, e.vertexId2, out);
            out << " already exists in database\n";
            break;
        case ConnectionNotFound:
            out << "Connection ";
            printConnection(e.vertexId1, e.vertexId1, e.vertexId2, out);
            out << " not found in database\n";
            break;
    }
}

std::string addVertex(const std::vector<std::string> &args) {
    std::stringstream out;
    if (!checkTwoArgs(args, out)) {
        return out.str();
    }
    try {
        VertexRegistry::getInstance().addVertex(args[1]);
        out << "Vertex added\n";
    }
    catch (VertexOperationException &e) {
        processVertexException(e, out);
    }
    return out.str();
}

std::string readVertex(const std::vector<std::string> &args) {
    std::stringstream out;
    if (!checkTwoArgs(args, out)) {
        return out.str();
    }
    try {
        const Vertex vertex = VertexRegistry::getInstance().getVertex(args[1]);
        printVertex(vertex, out);
    }
    catch (VertexOperationException &e) {
        processVertexException(e, out);
    }
    return out.str();
}

std::string deleteVertex(const std::vector<std::string> &args) {
    std::stringstream out;
    if (!checkTwoArgs(args, out)) {
        return out.str();
    }
    try {
        VertexRegistry::getInstance().deleteVertex(args[1]);
        out << "Vertex successfully deleted\n";
    }
    catch (VertexOperationException &e) {
        processVertexException(e, out);
    }
    return out.str();
}

std::string connectVertices(const std::vector<std::string> &args) {
    std::stringstream out;
    if (!checkFourArgs(args, out)) {
        return out.str();
    }
    try {
        VertexRegistry::getInstance().connectVertices(args[1], args[2], args[3]);
        out << "Connection ";
        printConnection(args[1], args[2], args[3], out);
        out << " successfully created\n";
    }
    catch (VertexOperationException &e) {
        processVertexException(e, out);
    }
    catch (ConnectionOperationException &e) {
        processConnectionException(e, out);
    }
    return out.str();
}

std::string disconnectVertices(const std::vector<std::string> &args) {
    std::stringstream out;
    if (!checkFourArgs(args, out)) {
        return out.str();
    }
    try {
        VertexRegistry::getInstance().disconnectVertices(args[1], args[2], args[3]);
        out << "Connection ";
        printConnection(args[1], args[2], args[3], out);
        out << " successfully deleted\n";
    }
    catch (VertexOperationException &e) {
        processVertexException(e, out);
    }
    catch (ConnectionOperationException &e) {
        processConnectionException(e, out);
    }
    return out.str();
}

std::string query(const std::vector<std::string> &args) {
    std::stringstream out;
    if (!checkFourArgs(args, out, true)) {
        return out.str();
    }
    out << "Query result:\n";
    // ? ? ? or ? conn ? (1/8, 2/8)
    try {
        if (args[1] == "?" && args[3] == "?") {
            std::vector<std::string> allVertices = VertexRegistry::getInstance().getAllIds();
            for (const auto &id: allVertices) {
                const Vertex vertex = VertexRegistry::getInstance().getVertex(id);
                for (const auto &c: vertex.getOutputConnections()) {
                    if (args[2] != "?" && c.first != args[2]) {
                        continue;
                    }
                    out << "\t";
                    printConnection(id, c.first, c.second, out);
                    out << "\n";
                }
            }
        }
        // ? ? id2 or ? conn id2 (3/8, 4/8)
        if (args[1] == "?" && args[3] != "?") {
            const Vertex vertex2 = VertexRegistry::getInstance().getVertex(args[3]);
            for (const auto &c: vertex2.getInputConnections()) {
                if (args[2] != "?" && c.first != args[2]) {
                    continue;
                }
                out << "\t";
                printConnection(c.second, c.first, args[3], out);
                out << "\n";
            }
        }
        // id1 ? ? or id1 conn ? (5/8, 6/8)
        if (args[1] != "?" && args[3] == "?") {
            const Vertex vertex1 = VertexRegistry::getInstance().getVertex(args[1]);
            for (const auto &c: vertex1.getOutputConnections()) {
                if (args[2] != "?" && c.first != args[2]) {
                    continue;
                }
                out << "\t";
                printConnection(args[1], c.first, c.second, out);
                out << "\n";
            }
        }
        // id1 ? id2 or id1 conn id2 (7/8, 8/8)
        if (args[1] != "?" && args[3] != "?") {
            const Vertex vertex1 = VertexRegistry::getInstance().getVertex(args[1]);
            VertexRegistry::getInstance().getVertex(args[3]);
            for (const auto &c: vertex1.getOutputConnections()) {
                if (c.second != args[3]) {
                    continue;
                }
                if (args[2] != "?" && c.first != args[2]) {
                    continue;
                }
                out << "\t";
                printConnection(args[1], c.first, args[3], out);
                out << "\n";
            }
        }
    }
    catch (VertexOperationException &e) {
        processVertexException(e, out);
    }
    catch (ConnectionOperationException &e) {
        processConnectionException(e, out);
    }
    return out.str();
}

std::string fallback(const std::vector<std::string> &s) {
    return "Unknown command. Use 'help' command to get help";
}

DatabaseServer::DatabaseServer(const std::string &host, int port) : AbstractServer(host, port) {
    m_operations.insert({"add", addVertex});
    m_operations.insert({"read", readVertex});
    m_operations.insert({"delete", deleteVertex});
    m_operations.insert({"connect", connectVertices});
    m_operations.insert({"disconnect", disconnectVertices});
    m_operations.insert({"query", query});
}

std::string DatabaseServer::processString(const std::string &command) {
    auto parsed = split(command);
    if (m_operations.find(parsed[0]) != m_operations.end()) {
        return m_operations[parsed[0]](parsed);
    }
    return fallback(parsed);
}
