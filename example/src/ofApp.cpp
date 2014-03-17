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


#include "ofApp.h"


void ofApp::setup()
{
    ofSetFrameRate(30);
    ofEnableAlphaBlending();

    /// Example Database
    std::string exampleDB = ofToDataPath("example.sqlite", true);

    try
    {
        // Open a database file in readonly mode
        SQLite::Database db(exampleDB);  // SQLITE_OPEN_READONLY

        // Test if the 'test' table exists
        bool testExists = db.tableExists("test");

        if (testExists)
        {
            ofLogNotice() << "Test table did exist!";
        }
        else
        {
            ofLogNotice() << "Test table did NOT exist!";
        }


        // Get a single value result with an easy to use shortcut
        std::string value = db.execAndGet("SELECT value FROM test WHERE id = 2");

        ofLogNotice() << "Value was: " << value;

        std::string queryString = "";
        queryString += "SELECT id AS test_id, value AS test_val, weight AS test_weight ";
        queryString += "FROM test ";
        queryString += "WHERE weight > ?"; // We will bind a variable to the ?.

        // Compile a SQL query, containing one parameter (index 1)
        SQLite::Statement query(db, queryString);

        // See how many columns are in our prepared statement:

        ofLogNotice() << "SQLite statement '" << query.getQuery() << "' compiled (" << query.getColumnCount() << " columns in the result)";

        // Bind the integer value 2 to the first parameter of the SQL query
        query.bind(1, 2);

        // Loop to execute the query step by step, to get one a row of results at a time
        while (query.executeStep())
        {
            // Demonstrate how to get some typed column value (and the equivalent explicit call)
            int         id      = query.getColumn(0); // = query.getColumn(0).getInt()
            std::string value2  = query.getColumn(1); // = query.getColumn(1).getText()
            int         bytes   = query.getColumn(1).getBytes();
            double      weight  = query.getColumn(2); // = query.getColumn(2).getInt()

            static bool bFirst = true;
            if (bFirst)
            {
                // Show how to get the aliased names of the result columns.
                std::string name0 = query.getColumn(0).getName();
                std::string name1 = query.getColumn(1).getName();
                std::string name2 = query.getColumn(2).getName();

                ofLogNotice() << "aliased result [\"" << name0 << "\", \"" << name1 << "\", \"" << name2 << "\"]";

                bFirst = false;
            }

            ofLogNotice() << "row (" << id << ", \"" << value2 << "\" "  << bytes << " bytes, " << weight << ")";
        }

        // Reset the query to use it again.
        query.reset();

        // Bind the string value "6" to the first parameter of the SQL query
        query.bind(1, "6");

        while (query.executeStep())
        {
            // Demonstrate that inserting column value in a std:ostream is natural
            ofLogNotice() << "row (" << query.getColumn(0) << ", \"" << query.getColumn(1) << "\", " << query.getColumn(2) << ")";
        }
    }
    catch (std::exception& e)
    {
        ofLogError() << "SQLite exception: " << e.what();
    }

}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));
    
}
