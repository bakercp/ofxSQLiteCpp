//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include <string>
#include "SQLiteCpp.h"
#include "ofLog.h"


class MyObject
{
public:
    // Constructor
    MyObject(const std::string& dbFile):
        // Open a database file in readonly mode
        _db(dbFile),
        // Compile a SQL query, containing one parameter (index 1)
        _query(_db, "SELECT * FROM test WHERE weight > :min_weight")
    {
    }

    virtual ~MyObject()
    {
    }

    /// \brief List the rows where the "weight" column is greater than the provided \p aParamValue.
    /// \param aParamValue The value
    void listGreaterThan(int aParamValue)
    {
        ofLogNotice("MyObject::listGreaterThan") << "ListGreaterThan (" << aParamValue << ")";

        // Bind the integer value provided to the first parameter of the SQL query
        _query.bind(":min_weight", aParamValue); // same as mQuery.bind(1, aParamValue);

        // Loop to execute the query step by step, to get one a row of results at a time
        while (_query.executeStep())
        {
            ofLogNotice("MyObject::listGreaterThan") << "row (" << _query.getColumn(0) << ", \"" << _query.getColumn(1) << "\", " << _query.getColumn(2) << ")";
        }

        // Reset the query to be able to use it again later
        _query.reset();
    }

private:
    /// \brief Database connection.
    SQLite::Database _db;

    /// \brief Database prepared SQL query.
    SQLite::Statement _query;

};
