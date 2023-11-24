//
// Created by goras on 24.11.2023.
//

#ifndef DB_CLUSTERLOCKER_H
#define DB_CLUSTERLOCKER_H

template<class T>
class ClusterLocker {
public:
    explicit ClusterLocker(T *cluster) : m_cluster(cluster) {
        m_cluster->lock();
    }

    ~ClusterLocker() {
        m_cluster->unlock();
    }

private:
    T *m_cluster;
};

#endif //DB_CLUSTERLOCKER_H
