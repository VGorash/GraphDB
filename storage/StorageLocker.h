//
// Created by goras on 24.11.2023.
//

#ifndef DB_CLUSTERLOCKER_H
#define DB_CLUSTERLOCKER_H

class VertexStorageImpl;

class StorageLocker {
public:
    explicit StorageLocker(VertexStorageImpl *storage);

    ~StorageLocker();

private:
    VertexStorageImpl *m_storage;
};

#endif //DB_CLUSTERLOCKER_H
