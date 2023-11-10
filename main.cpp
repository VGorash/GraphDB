#include "NodeRegistry.h"
#include "Node.h"
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

void printNode(const Node* node){
    std::cout << "\tNode: " << node->getId() << "\n";
    std::cout << "\tOutput connections: " << "\n";
    for(const auto& c : node->getOutputConnections()){
        std::cout << "\t\t --> (" << c.first << ") --> " << c.second << "\n";
    }
    std::cout << "\tInput connections: " << "\n";
    for(const auto& c : node->getInputConnections()){
        std::cout << "\t\t <-- (" << c.first << ") <-- " << c.second << "\n";
    }
}

void printConnection(const std::string &id1, const std::string &connName, const std::string &id2){
    std::cout << id1 << " --> " << connName << " --> " <<id2;
}

void addNode(const std::vector<std::string> &args){
    if(!checkTwoArgs(args)){
        return;
    }
    const Node* node = NodeRegistry::getInstance().addNode(args[1]);
    if(!node){
        std::cout << "Node with id " << args[1] << " already exists in database\n";
        return;
    }
    std::cout << "Node added\n";
}

void readNode(const std::vector<std::string> &args){
    if(!checkTwoArgs(args)){
        return;
    }
    const Node* node = NodeRegistry::getInstance().getNode(args[1]);
    if(!node){
        std::cout << "Node with id " << args[1] << " not found in database\n";
        return;
    }
    printNode(node);
}

void deleteNode(const std::vector<std::string> &args){
    if(!checkTwoArgs(args)){
        return;
    }
    if(!NodeRegistry::getInstance().deleteNode(args[1])){
        std::cout << "Node with id " << args[1] << " not found in database\n";
        return;
    }
    std::cout << "Node successfully deleted\n";
}

void connectNodes(const std::vector<std::string> &args){
    if(!checkFourArgs(args)){
        return;
    }
    const Node* node1 = NodeRegistry::getInstance().getNode(args[1]);
    const Node* node2 = NodeRegistry::getInstance().getNode(args[3]);
    if(!node1){
        std::cout << "Node with id " << args[1] << " not found in database\n";
        return;
    }
    if(!node2){
        std::cout << "Node with id " << args[3] << " not found in database\n";
        return;
    }
    if(!NodeRegistry::getInstance().connectNodes(args[1], args[2], args[3])){
        std::cout << "Connection ";
        printConnection(args[1], args[2], args[3]);
        std::cout << " already exists in database\n";
        return;
    }
    std::cout << "Connection ";
    printConnection(args[1], args[2], args[3]);
    std::cout << " successfully created\n";
}

void disconnectNodes(const std::vector<std::string> &args){
    if(!checkFourArgs(args)){
        return;
    }
    const Node* node1 = NodeRegistry::getInstance().getNode(args[1]);
    const Node* node2 = NodeRegistry::getInstance().getNode(args[3]);
    if(!node1){
        std::cout << "Node with id " << args[1] << " not found in database\n";
        return;
    }
    if(!node2){
        std::cout << "Node with id " << args[3] << " not found in database\n";
        return;
    }
    if(!NodeRegistry::getInstance().disconnectNodes(args[1], args[2], args[3])){
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
        std::vector<std::string> allNodes = NodeRegistry::getInstance().getAllIds();
        for(const auto& id : allNodes){
            for(auto c : NodeRegistry::getInstance().getNode(id)->getOutputConnections()){
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
        Node* node2 = NodeRegistry::getInstance().getNode(args[3]);
        if(!node2){
            std::cout << "Node with id " << args[3] << " not found in database\n";
            return;
        }
        for(auto c : node2->getInputConnections()){
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
        Node* node1 = NodeRegistry::getInstance().getNode(args[1]);
        if(!node1){
            std::cout << "Node with id " << args[1] << " not found in database\n";
            return;
        }
        for(auto c : node1->getOutputConnections()){
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
        Node* node1 = NodeRegistry::getInstance().getNode(args[1]);
        Node* node2 = NodeRegistry::getInstance().getNode(args[3]);
        if(!node1){
            std::cout << "Node with id " << args[1] << " not found in database\n";
            return;
        }
        if(!node2){
            std::cout << "Node with id " << args[3] << " not found in database\n";
            return;
        }
        for(auto c : node1->getOutputConnections()){
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
            addNode(parsed);
        }
        else if(command == "read"){
            readNode(parsed);
        }
        else if(command == "delete"){
            deleteNode(parsed);
        }
        else if(command == "connect"){
            connectNodes(parsed);
        }
        else if(command == "disconnect"){
            disconnectNodes(parsed);
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
