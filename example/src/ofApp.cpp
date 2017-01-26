//
// Copyright (c) 2014 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    ofSetFrameRate(30);
    ofEnableAlphaBlending();

    // Example database.
    std::string exampleDB = ofToDataPath("example.sqlite", true);

    try
    {
        // Open a database file in readonly mode
        SQLite::Database db(exampleDB);  // SQLITE_OPEN_READONLY

        // Test if the 'test' table exists
        bool testExists = db.tableExists("test");

        if (testExists)
        {
            ofLogNotice("ofApp::setup()") << "Test table did exist!";
        }
        else
        {
            ofLogNotice("ofApp::setup()") << "Test table did NOT exist!";
        }


        // Get a single value result with an easy to use shortcut
        std::string value = db.execAndGet("SELECT value FROM test WHERE id = 2");

        ofLogNotice("ofApp::setup()") << "Value was: " << value;

        std::string queryString = "";
        queryString += "SELECT id AS test_id, value AS test_val, weight AS test_weight ";
        queryString += "FROM test ";
        queryString += "WHERE weight > ?"; // We will bind a variable to the ?.

        // Compile a SQL query, containing one parameter (index 1)
        SQLite::Statement query(db, queryString);

        // See how many columns are in our prepared statement:

        ofLogNotice("ofApp::setup()") << "SQLite statement '" << query.getQuery() << "' compiled (" << query.getColumnCount() << " columns in the result)";

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

                ofLogNotice("ofApp::setup()") << "aliased result [\"" << name0 << "\", \"" << name1 << "\", \"" << name2 << "\"]";

                bFirst = false;
            }

            ofLogNotice("ofApp::setup()") << "row (" << id << ", \"" << value2 << "\" "  << bytes << " bytes, " << weight << ")";
        }

        // Reset the query to use it again.
        query.reset();

        // Bind the string value "6" to the first parameter of the SQL query
        query.bind(1, "6");

        while (query.executeStep())
        {
            // Demonstrate that inserting column value in a std:ostream is natural
            ofLogNotice("ofApp::setup()") << "row (" << query.getColumn(0) << ", \"" << query.getColumn(1) << "\", " << query.getColumn(2) << ")";
        }
    }
    catch (const std::exception& e)
    {
        ofLogError("ofApp::setup()") << "SQLite exception: " << e.what();
    }

}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);
    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));
}
