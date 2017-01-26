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
        // Open a database file in create/write mode.
        SQLite::Database db(exampleDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

        ofLogNotice("ofApp::setup()") << "SQLite database file '" << db.getFilename() << "' opened successfully";

        db.exec("DROP TABLE test");
        db.exec("CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)");

        // first row
        int nb = db.exec("INSERT INTO test VALUES (NULL, `test`)");

        ofLogNotice("ofApp::setup()") << "INSERT INTO test VALUES (NULL, \"test\")\", returned " << nb;

        // second row
        nb = db.exec("INSERT INTO test VALUES (NULL, \"second\")");

        ofLogNotice("ofApp::setup()") << "INSERT INTO test VALUES (NULL, \"second\")\", returned " << nb;

        // update the second row
        nb = db.exec("UPDATE test SET value=\"second-updated\" WHERE id='2'");

        ofLogNotice("ofApp::setup()") << "UPDATE test SET value=\"second-updated\" WHERE id='2', returned " << nb;

        // Check the results : expect two row of result
        SQLite::Statement query(db, "SELECT * FROM test");

        ofLogNotice("ofApp::setup()") << "SELECT * FROM test :";

        while (query.executeStep())
        {
            ofLogNotice("ofApp::setup()") << "row (" << query.getColumn(0) << ", \"" << query.getColumn(1) << "\")";
        }

        db.exec("DROP TABLE test");
    }
    catch (const std::exception& e)
    {
        ofLogError() << "SQLite exception: " << e.what();
    }

}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);
    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));
}
