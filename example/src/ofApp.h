// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
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


#include "ofMain.h"
#include "SQLiteC++.h"
//#include "Route.h"
//#include "Station.h"
//#include "Trip.h"


/// Object Oriented Basic example
class Example
{
public:
    // Constructor
    Example(const std::string& dbFile) :
    mDb(dbFile),                                  // Open a database file in readonly mode
    mQuery(mDb, "SELECT * FROM test WHERE weight > :min_weight")// Compile a SQL query, containing one parameter (index 1)
    {
    }
    virtual ~Example(void)
    {
    }

    /// List the rows where the "weight" column is greater than the provided aParamValue
    void ListGreaterThan (const int aParamValue)
    {
        std::cout << "ListGreaterThan (" << aParamValue << ")\n";

        // Bind the integer value provided to the first parameter of the SQL query
        mQuery.bind(":min_weight", aParamValue); // same as mQuery.bind(1, aParamValue);

        // Loop to execute the query step by step, to get one a row of results at a time
        while (mQuery.executeStep())
        {
            std::cout << "row (" << mQuery.getColumn(0) << ", \"" << mQuery.getColumn(1) << "\", " << mQuery.getColumn(2) << ")\n";
        }

        // Reset the query to be able to use it again later
        mQuery.reset();
    }

private:
    SQLite::Database    mDb;    ///< Database connection
    SQLite::Statement   mQuery; ///< Database prepared SQL query
};


class ofApp: public ofBaseApp
{
public:
    void setup();
    void draw();

//    ofxSQLite db;

};
