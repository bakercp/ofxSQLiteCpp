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
    else
    {
        // Insert and retern a reference.
        return *_statements.insert(std::make_pair(query, std::make_unique<Statement>(_database, query))).first->second.get();
    }
}


int SQLiteConnection::_toAccessFlag(Mode mode)
{
    switch (mode)
    {
        case Mode::READ_ONLY:
            return SQLITE_OPEN_READONLY;
        case Mode::READ_WRITE:
            return SQLITE_OPEN_READWRITE;
        case Mode::READ_WRITE_CREATE:
            return SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    }
}


} // nampace SQLite
