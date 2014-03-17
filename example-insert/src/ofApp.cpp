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
        // Open a database file in create/write mode
        SQLite::Database db(exampleDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

        ofLogNotice() << "SQLite database file '" << db.getFilename() << "' opened successfully";

        db.exec("DROP TABLE IF EXISTS test");
        db.exec("CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)");

        // first row
        int nb = db.exec("INSERT INTO test VALUES (NULL, \"test\")");

        ofLogNotice() << "INSERT INTO test VALUES (NULL, \"test\")\", returned " << nb;

        // second row
        nb = db.exec("INSERT INTO test VALUES (NULL, \"second\")");

        ofLogNotice() << "INSERT INTO test VALUES (NULL, \"second\")\", returned " << nb;

        // update the second row
        nb = db.exec("UPDATE test SET value=\"second-updated\" WHERE id='2'");

        ofLogNotice() << "UPDATE test SET value=\"second-updated\" WHERE id='2', returned " << nb;

        // Check the results : expect two row of result
        SQLite::Statement query(db, "SELECT * FROM test");

        ofLogNotice() << "SELECT * FROM test :";

        while (query.executeStep())
        {
            ofLogNotice() << "row (" << query.getColumn(0) << ", \"" << query.getColumn(1) << "\")";
        }

        db.exec("DROP TABLE test");
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
