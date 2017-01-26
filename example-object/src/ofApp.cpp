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
        // Open the database and compile the query.
        MyObject example(exampleDB);

        // Demonstrate the way to use the same query with different parameter values.
        example.listGreaterThan(8);
        example.listGreaterThan(6);
        example.listGreaterThan(2);
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
