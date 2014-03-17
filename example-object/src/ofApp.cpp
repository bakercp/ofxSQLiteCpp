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
        // Open the database and compile the query
        MyObject example(exampleDB);

        // Demonstrate the way to use the same query with different parameter values
        example.listGreaterThan(8);
        example.listGreaterThan(6);
        example.listGreaterThan(2);
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
