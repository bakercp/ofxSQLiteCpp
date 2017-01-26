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
    std::string mbTilesFilename = ofToDataPath("blank-1-3.mbtiles", true);

    try
    {
        // Open a database file in readonly mode
        SQLite::Database db(mbTilesFilename);  // SQLITE_OPEN_READONLY

        int zoomLevel = 1;
        int tileColumn = 0;
        int tileRow = 0;

        // We use string stream to create a long query string, but one
        // could just as easily use a string with the += operator.  It's style.
        std::stringstream ss;

        ss << "SELECT zoom_level, tile_column, tile_row, tile_data ";
        ss << "FROM tiles ";
        ss << "WHERE zoom_level = ? ";
        ss << "AND tile_column = ? ";
        ss << "AND tile_row = ?";

        // Create the query with the database.
        SQLite::Statement query(db, ss.str());

        query.bind(1, zoomLevel);  // Bind the zoom level to the first ? in the query.
        query.bind(2, tileColumn); // Bind the tile column to the second ? in the query.
        query.bind(3, tileRow); // Bind the tile row to the third ? in the query.

        // Loop to execute the query step by step, to get one a row of results at a time
        while (query.executeStep())
        {
            SQLite::Column colBlob = query.getColumn(3); // We know that column 3 is the image bytes
            const void* blob = colBlob.getBlob(); // Get a pointer to the bytes.
            std::size_t size = colBlob.getBytes(); // Get the number of bytes.
            ofBuffer buffer(static_cast<const char*>(blob), size); // Create a buffer with those bytes.

            img.load(buffer); // Load the image from the buffer.
        }

    }
    catch (const std::exception& e)
    {
        ofLogError("ofApp::setup") << "SQLite exception: " << e.what();
    }
}


void ofApp::draw()
{
    ofBackgroundGradient(ofColor::white, ofColor::black);

    // Draw the tile image.
    img.draw(0, 0);
}
