// =============================================================================
//
// Copyright (c) 2015-2016 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


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
