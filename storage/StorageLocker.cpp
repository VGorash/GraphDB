//
// Created by goras on 18.12.2023.
//
#include "StorageLocker.h"
#include "VertexStorageImpl.h"

StorageLocker::StorageLocker(VertexStorageImpl *storage) : m_storage(storage) {
    m_storage->lock();
}

StorageLocker::~StorageLocker() {
    m_storage->unlock();
}
