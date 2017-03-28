//
// Copyright (c) 2015 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "SQLiteUtils.h"
#include "SQLiteJSON.h"


namespace SQLite {


ofJson SQLiteUtils::execute(Statement& query)
{
    return SQLiteJSON::execute(query);
}


} // nampace SQLite
