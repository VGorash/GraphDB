#include "VertexRegistry.h"
#include "Vertex.h"
#include "utils.h"
#include <regex>

bool check_id(const std::string &id){
    std::regex pattern("^[A-Za-z0-9_]+$");
    return std::regex_match(id, pattern);
}

bool checkTwoArgs(const std::vector<std::string> &args){
    if(args.size() != 2){
        std::cout << "Expected 1 argument " << args.size() - 1 << " provided\n";
        return false;
    }
    const std::string& id = args[1];
    if(id.empty() || !check_id(id)){
        std::cout << "ID must contain only letters, digits and '_' symbol\n";
        return false;
    }
    return true;
}

bool checkFourArgs(const std::vector<std::string> &args, bool query=false){
    if(args.size() != 4){
        std::cout << "Expected 3 arguments " << args.size() - 1 << " provided\n";
        return false;
    }
    const std::string& id1 = args[1];
    const std::string& connName = args[2];
    const std::string& id2 = args[3];
    if((id1.empty() || !check_id(id1)) && (!query || id1 != "?")){
        std::cout << "ID must contain only letters, digits and '_' symbol\n";
        return false;
    }
    if(id2.empty() || !check_id(id2) && (!query || id2 != "?")){
        std::cout << "ID must contain only letters, digits and '_' symbol\n";
        return false;
    }
    if(connName.empty() || !check_id(connName) && (!query || connName != "?")){
        std::cout << "Connection name must contain only letters, digits and '_' symbol\n";
        return false;
    }
    return true;
}

void printVertex(const Vertex* vertex){
    std::cout << "ID: " << vertex->getId() << "\n";
    std::cout << "Output connections: " << "\n";
    for(const auto& c : vertex->getOutputConnections()){
        std::cout << "\t --(" << c.first << ")--> " << c.second << "\n";
    }
    std::cout << "Input connections: " << "\n";
    for(const auto& c : vertex->getInputConnections()){
        std::cout << "\t <--(" << c.first << ")-- " << c.second << "\n";
    }
}

void printConnection(const std::string &id1, const std::string &connName, const std::string &id2){
    std::cout << id1 << " --(" << connName << ")--> " <<id2;
}

void addVertex(const std::vector<std::string> &args){
    if(!checkTwoArgs(args)){
        return;
    }
    const Vertex* vertex = VertexRegistry::getInstance().addVertex(args[1]);
    if(!vertex){
        std::cout << "Vertex with id " << args[1] << " already exists in database\n";
        return;
    }
    std::cout << "Vertex added\n";
}

void readVertex(const std::vector<std::string> &args){
    if(!checkTwoArgs(args)){
        return;
    }
    const Vertex* vertex = VertexRegistry::getInstance().getVertex(args[1]);
    if(!vertex){
        std::cout << "Vertex with id " << args[1] << " not found in database\n";
        return;
    }
    printVertex(vertex);
}

void deleteVertex(const std::vector<std::string> &args){
    if(!checkTwoArgs(args)){
        return;
    }
    if(!VertexRegistry::getInstance().deleteVertex(args[1])){
        std::cout << "Vertex with id " << args[1] << " not found in database\n";
        return;
    }
    std::cout << "Vertex successfully deleted\n";
}

void connectVertexs(const std::vector<std::string> &args){
    if(!checkFourArgs(args)){
        return;
    }
    const Vertex* vertex1 = VertexRegistry::getInstance().getVertex(args[1]);
    const Vertex* vertex2 = VertexRegistry::getInstance().getVertex(args[3]);
    if(!vertex1){
        std::cout << "Vertex with id " << args[1] << " not found in database\n";
        return;
    }
    if(!vertex2){
        std::cout << "Vertex with id " << args[3] << " not found in database\n";
        return;
    }
    if(!VertexRegistry::getInstance().connectVertices(args[1], args[2], args[3])){
        std::cout << "Connection ";
        printConnection(args[1], args[2], args[3]);
        std::cout << " already exists in database\n";
        return;
    }
    std::cout << "Connection ";
    printConnection(args[1], args[2], args[3]);
    std::cout << " successfully created\n";
}

void disconnectVertexs(const std::vector<std::string> &args){
    if(!checkFourArgs(args)){
        return;
    }
    const Vertex* vertex1 = VertexRegistry::getInstance().getVertex(args[1]);
    const Vertex* vertex2 = VertexRegistry::getInstance().getVertex(args[3]);
    if(!vertex1){
        std::cout << "Vertex with id " << args[1] << " not found in database\n";
        return;
    }
    if(!vertex2){
        std::cout << "Vertex with id " << args[3] << " not found in database\n";
        return;
    }
    if(!VertexRegistry::getInstance().disconnectVertices(args[1], args[2], args[3])){
        std::cout << "Connection ";
        printConnection(args[1], args[2], args[3]);
        std::cout << " not found in database\n";
        return;
    }
    std::cout << "Connection ";
    printConnection(args[1], args[2], args[3]);
    std::cout << " successfully deleted\n";
}

void query(const std::vector<std::string> &args){
    if(!checkFourArgs(args, true)){
        return;
    }
    std::cout << "Query result:\n";
    // ? ? ? or ? conn ? (1/8, 2/8)
    if(args[1] == "?" && args[3] == "?"){
        std::vector<std::string> allVertexs = VertexRegistry::getInstance().getAllIds();
        for(const auto& id : allVertexs){
            for(auto c : VertexRegistry::getInstance().getVertex(id)->getOutputConnections()){
                if (args[2] != "?" && c.first != args[2]){
                    continue;
                }
                std::cout << "\t";
                printConnection(id, c.first, c.second);
                std::cout << "\n";
            }
        }
    }
    // ? ? id2 or ? conn id2 (3/8, 4/8)
    if(args[1] == "?" && args[3] != "?"){
        const Vertex* vertex2 = VertexRegistry::getInstance().getVertex(args[3]);
        if(!vertex2){
            std::cout << "Vertex with id " << args[3] << " not found in database\n";
            return;
        }
        for(auto c : vertex2->getInputConnections()){
            if (args[2] != "?" && c.first != args[2]){
                continue;
            }
            std::cout << "\t";
            printConnection(c.second, c.first, args[3]);
            std::cout << "\n";
        }
    }
    // id1 ? ? or id1 conn ? (5/8, 6/8)
    if(args[1] != "?" && args[3] == "?"){
        const Vertex* vertex1 = VertexRegistry::getInstance().getVertex(args[1]);
        if(!vertex1){
            std::cout << "Vertex with id " << args[1] << " not found in database\n";
            return;
        }
        for(auto c : vertex1->getOutputConnections()){
            if (args[2] != "?" && c.first != args[2]){
                continue;
            }
            std::cout << "\t";
            printConnection(args[1], c.first, c.second);
            std::cout << "\n";
        }
    }
    // id1 ? id2 or id1 conn id2 (7/8, 8/8)
    if(args[1] != "?" && args[3] != "?"){
        const Vertex* vertex1 = VertexRegistry::getInstance().getVertex(args[1]);
        const Vertex* vertex2 = VertexRegistry::getInstance().getVertex(args[3]);
        if(!vertex1){
            std::cout << "Vertex with id " << args[1] << " not found in database\n";
            return;
        }
        if(!vertex2){
            std::cout << "Vertex with id " << args[3] << " not found in database\n";
            return;
        }
        for(auto c : vertex1->getOutputConnections()){
            if (c.second != args[3]){
                continue;
            }
            if (args[2] != "?" && c.first != args[2]){
                continue;
            }
            std::cout << "\t";
            printConnection(args[1], c.first, args[3]);
            std::cout << "\n";
        }
    }
}

int main() {
    bool exitFlag = false;
    std::string input;
    std::string command;

    while (!exitFlag){
        std::cout << ">";
        std::getline(std::cin, input);
        auto parsed = split(input);
        command = lowercase(parsed[0]);
        if(command.empty()){
            continue;
        }
        if(command == "add"){
            addVertex(parsed);
        }
        else if(command == "read"){
            readVertex(parsed);
        }
        else if(command == "delete"){
            deleteVertex(parsed);
        }
        else if(command == "connect"){
            connectVertexs(parsed);
        }
        else if(command == "disconnect"){
            disconnectVertexs(parsed);
        }
        else if(command == "query"){
            query(parsed);
        }
        else if(command == "exit"){
            exitFlag = true;
        }
        else if(command == "help"){
            std::cout << "Some help\n";
        }
        else{
            std::cout << "Unknown command. Use 'help' command to get help\n";
        }
    }
}
