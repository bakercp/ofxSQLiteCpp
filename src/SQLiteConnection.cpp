//
// Copyright (c) 2015 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "SQLiteConnection.h"
#include "ofUtils.h"


namespace SQLite {


SQLiteConnection::SQLiteConnection(const std::string& filename,
                                   Mode mode,
                                   uint64_t databaseTimeoutMilliseconds,
                                   std::size_t index):
    _mode(mode),
    _database(ofToDataPath(filename, true),
              _toAccessFlag(_mode),
              databaseTimeoutMilliseconds),
    _index(index)
{
}


SQLiteConnection::~SQLiteConnection()
{
}


Database& SQLiteConnection::database()
{
    return _database;
}


const Database& SQLiteConnection::database() const
{
    return _database;
}


bool SQLiteConnection::hasStatement(const std::string& query) const
{
    return _statements.find(query) != _statements.end();
}


Statement& SQLiteConnection::getStatement(const std::string& query) const
{
    auto iter = _statements.find(query);

    if (iter != _statements.end())
    {
        iter->second->reset();
        iter->second->clearBindings();
        return *iter->second.get();
    }

    // Insert and retern a reference.
    return *_statements.insert(std::make_pair(query, std::make_unique<Statement>(_database, query))).first->second.get();
}


int SQLiteConnection::_toAccessFlag(Mode mode)
{
    switch (mode)
    {
        case Mode::READ_ONLY:
            return OPEN_READONLY;
        case Mode::READ_WRITE:
            return OPEN_READWRITE;
        case Mode::READ_WRITE_CREATE:
            return OPEN_READWRITE | OPEN_CREATE;
    }
    
    return OPEN_READONLY;
}


} // nampace SQLite
