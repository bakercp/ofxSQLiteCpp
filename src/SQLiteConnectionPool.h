//
// Copyright (c) 2015 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofTypes.h"
#include "Poco/RWLock.h"
#include "ofx/ObjectPool.h"
#include "SQLiteConnection.h"


namespace SQLite {


template<typename ConnectionType>
class SQLiteConnectionPoolFactory
{
public:
    SQLiteConnectionPoolFactory(const std::string& filename,
                                SQLiteConnection::Mode mode,
                                uint64_t databaseTimeoutMilliseconds):
        _filename(filename),
        _mode(mode),
        _databaseTimeoutMilliseconds(databaseTimeoutMilliseconds)
    {
    }

    SQLiteConnectionPoolFactory(const SQLiteConnectionPoolFactory& factory):
        _filename(factory._filename),
        _mode(factory._mode),
        _databaseTimeoutMilliseconds(factory._databaseTimeoutMilliseconds)
    {
    }

    std::shared_ptr<ConnectionType> createObject()
    {
        return std::make_shared<ConnectionType>(_filename,
                                                _mode,
                                                _databaseTimeoutMilliseconds,
                                                currentIndex++);
    }

    bool validateObject(std::shared_ptr<ConnectionType> _connection)
    {
        return true;
    }

    void activateObject(std::shared_ptr<ConnectionType> _connection)
    {
        _connection->increment();
    }

    void deactivateObject(std::shared_ptr<ConnectionType> _connection)
    {
    }

    void destroyObject(std::shared_ptr<ConnectionType> _connection)
    {
    }

private:
    std::string _filename;
    SQLiteConnection::Mode _mode;
    uint64_t _databaseTimeoutMilliseconds = 0;

    std::size_t currentIndex = 0;
    
};



template<typename ConnectionType>
class SQLiteConnectionPool_: public ofx::ObjectPool<ConnectionType, std::shared_ptr<ConnectionType>, SQLiteConnectionPoolFactory<ConnectionType>>
{
public:
    SQLiteConnectionPool_(const std::string& filename,
                          SQLiteConnection::Mode mode = SQLiteConnection::Mode::READ_ONLY,
                          uint64_t databaseTimeoutMilliseconds = 0,
                          std::size_t capacity = DEFAULT_CAPACITY,
                          std::size_t peakCapacity = DEFAULT_PEAK_CAPACITY):
        ofx::ObjectPool<ConnectionType, std::shared_ptr<ConnectionType>, SQLiteConnectionPoolFactory<ConnectionType>>(SQLiteConnectionPoolFactory<ConnectionType>(filename, mode, databaseTimeoutMilliseconds), capacity, peakCapacity)
    {
    }

    enum
    {
        DEFAULT_CAPACITY = 100,
        DEFAULT_PEAK_CAPACITY = 200
    };

    std::string toString() const
    {
        std::stringstream ss;
        ss << "Capacity: " << this->capacity() << " Peak Capacity: " << this->peakCapacity() << " Size: " << this->size() << " Available: " << this->available();
        return ss.str();
    }
};


typedef SQLiteConnectionPool_<SQLiteConnection> SQLiteConnectionPool;



} // nampace SQLite
