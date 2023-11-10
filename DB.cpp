//
// Created by goras on 10.11.2023.
//

#include "DB.h"
#include "Node.h"
#include "NodeRegistry.h"

Node *DB::addNode(const std::string &id) {
    return NodeRegistry::getInstance().addNode(id);
}

Node *DB::getNode(const std::string &id) {
    return NodeRegistry::getInstance().getNode(id);
}

bool DB::deleteNode(const std::string &id) {
    return NodeRegistry::getInstance().deleteNode(id);
}


