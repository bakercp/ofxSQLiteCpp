//
// Copyright (c) 2015 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "SQLiteCpp/Statement.h"
#include "ofJson.h"


namespace SQLite {


class SQLiteUtils
{
public:
    /// \brief Export SQLite results as JSON.
    ///
    /// Encodes BLOBs using BASE64.
    ///
    /// \throws Various exceptions.
    static ofJson execute(Statement& query);

};


} // nampace SQLite
