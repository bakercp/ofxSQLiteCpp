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
    std::string transactionDb = ofToDataPath("transaction.sqlite", true);

    try
    {
        // Open a database file in create/write mode
        SQLite::Database db(transactionDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

        ofLogNotice() << "SQLite database file '" << db.getFilename() << "' opened successfully.";

        db.exec("DROP TABLE IF EXISTS test");

        // Example of a successful transaction :
        try
        {
            // Begin transaction
            SQLite::Transaction transaction(db);

            db.exec("CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)");

            int nb = db.exec("INSERT INTO test VALUES (NULL, \"test\")");

            ofLogNotice() << "INSERT INTO test VALUES (NULL, \"test\")\", returned " << nb;

            // Commit transaction
            transaction.commit();
        }
        catch (std::exception& e)
        {
            ofLogError() << "SQLite exception: " << e.what();
        }

        // Example of a rollbacked transaction :
        try
        {
            // Begin transaction
            SQLite::Transaction transaction(db);

            int nb = db.exec("INSERT INTO test VALUES (NULL, \"second\")");

            ofLogNotice() << "INSERT INTO test VALUES (NULL, \"second\")\", returned " << nb;

            nb = db.exec("INSERT INTO test ObviousError");

            ofLogNotice() << "INSERT INTO test \"error\", returned " << nb;


            ofLogNotice() << "SQLite SHOULD have exited ... ";

            // Commit transaction
            transaction.commit();
        }
        catch (std::exception& e)
        {
            ofLogError() << "EXPECTED: SQLite exception: " << e.what();
            // expected error, see above
        }

        // Check the results (expect only one row of result, as the second one has been rollbacked by the error)
        SQLite::Statement   query(db, "SELECT * FROM test");

        ofLogNotice() << "SELECT * FROM test :";

        while (query.executeStep())
        {
            ofLogNotice() << "row (" << query.getColumn(0) << ", \"" << query.getColumn(1) << "\")";
        }
    }
    catch (std::exception& e)
    {
        ofLogError() << "SQLite exception: " << e.what();
    }

    // Clean up.
    ofFile::removeFile(transactionDb);
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));
    
}
