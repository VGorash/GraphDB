#include "DB.h"
#include "Node.h"
#include "utils.h"
#include <regex>

bool check_id(const std::string &id){
    std::regex pattern("^[A-Za-z0-9_]+$");
    return std::regex_match(id, pattern);
}

bool checkTwoArgs(const std::vector<std::string> &args){
    if(args.size() != 2){
        std::cout << "Expected 2 arguments " << args.size() << " provided\n";
        return false;
    }
    const std::string& id = args[1];
    if(id.empty() || !check_id(id)){
        std::cout << "ID must contain only letters, digits and '_' symbol\n";
        return false;
    }
    return true;
}

void addNode(const std::vector<std::string> &args){
    if(!checkTwoArgs(args)){
        return;
    }
    Node* node = DB::addNode(args[1]);
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
    Node* node = DB::getNode(args[1]);
    if(!node){
        std::cout << "Node with id " << args[1] << " not found in database\n";
        return;
    }
    std::cout << "Node information:\n";
    std::cout << "ID: " << node->getId() << "\n";
}

void deleteNode(const std::vector<std::string> &args){
    if(!checkTwoArgs(args)){
        return;
    }
    if(!DB::deleteNode(args[1])){
        std::cout << "Node with id " << args[1] << " not found in database\n";
        return;
    }
    std::cout << "Node successfully deleted\n";
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
