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
    std::string transactionDb = ofToDataPath("transaction.sqlite", true);

    try
    {
        // Open a database file in create/write mode.
        SQLite::Database db(transactionDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

        ofLogNotice("ofApp::setup()") << "SQLite database file '" << db.getFilename() << "' opened successfully.";

        db.exec("DROP TABLE IF EXISTS test");

        // Example of a successful transaction :
        try
        {
            // Begin transaction
            SQLite::Transaction transaction(db);

            db.exec("CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)");

            int nb = db.exec("INSERT INTO test VALUES (NULL, \"test\")");

            ofLogNotice("ofApp::setup()") << "INSERT INTO test VALUES (NULL, \"test\")\", returned " << nb;

            // Commit transaction
            transaction.commit();
        }
        catch (const std::exception& e)
        {
            ofLogError("ofApp::setup()") << "SQLite exception: " << e.what();
        }

        // Example of a rollbacked transaction :
        try
        {
            // Begin transaction
            SQLite::Transaction transaction(db);

            int nb = db.exec("INSERT INTO test VALUES (NULL, \"second\")");

            ofLogNotice("ofApp::setup()") << "INSERT INTO test VALUES (NULL, \"second\")\", returned " << nb;

            nb = db.exec("INSERT INTO test ObviousError");

            ofLogNotice("ofApp::setup()") << "INSERT INTO test \"error\", returned " << nb;


            ofLogNotice("ofApp::setup()") << "SQLite SHOULD have exited ... ";

            // Commit transaction
            transaction.commit();
        }
        catch (const std::exception& e)
        {
            ofLogError("ofApp::setup()") << "EXPECTED: SQLite exception: " << e.what();
            // expected error, see above
        }

        // Check the results (expect only one row of result, as the second one has been rollbacked by the error)
        SQLite::Statement   query(db, "SELECT * FROM test");

        ofLogNotice("ofApp::setup()") << "SELECT * FROM test :";

        while (query.executeStep())
        {
            ofLogNotice("ofApp::setup()") << "row (" << query.getColumn(0) << ", \"" << query.getColumn(1) << "\")";
        }
    }
    catch (const std::exception& e)
    {
        ofLogError("ofApp::setup()") << "SQLite exception: " << e.what();
    }

    // Clean up.
    ofFile::removeFile(transactionDb);
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    ofDrawBitmapStringHighlight("See console for output.", ofPoint(30, 30));

}
