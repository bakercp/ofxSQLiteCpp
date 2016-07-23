// =============================================================================
//
// Copyright (c) 2014-2016 Christopher Baker <http://christopherbaker.net>
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
