//
// Copyright (c) 2015 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include "ofConstants.h"
#include "SQLiteCpp/Statement.h"
#include "ofJson.h"


namespace SQLite {


class SQLiteUtils
{
public:
    OF_DEPRECATED_MSG("Use SQLiteJSON::execute instead.", static ofJson execute(Statement& query));

};


} // nampace SQLite
