//
// Created by goras on 10.11.2023.
//

#ifndef DB_DB_H
#define DB_DB_H

#include <string>

class Node;

class DB {
public:
    static Node* addNode(const std::string &id);
    static Node* getNode(const std::string &id);
    static bool deleteNode(const std::string &id);
};


#endif //DB_DB_H
