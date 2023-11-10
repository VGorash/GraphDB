//
// Created by goras on 10.11.2023.
//

#include "Node.h"

#include <utility>

Node::Node(std::string id, NodeCluster* parentCluster) : m_id(std::move(id)), m_parentCluster(parentCluster){

}

const std::string &Node::getId() {
    return m_id;
}
