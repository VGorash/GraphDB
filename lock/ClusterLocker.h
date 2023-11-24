//
// Created by goras on 24.11.2023.
//

#ifndef DB_CLUSTERLOCKER_H
#define DB_CLUSTERLOCKER_H

#include "../cluster/VertexClusterImpl.h"

class ClusterLocker {
public:
    explicit ClusterLocker(VertexClusterImpl *cluster) : m_cluster(cluster) {
        m_cluster->lock();
    }

    ~ClusterLocker() {
        m_cluster->unlock();
    }

private:
    VertexClusterImpl *m_cluster;
};

#endif //DB_CLUSTERLOCKER_H
