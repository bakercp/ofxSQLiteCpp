//
// Copyright (c) 2015 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "SQLiteUtils.h"
#include "SQLiteCpp/Column.h"
#include "Poco/Base64Encoder.h"
#include <sstream>


namespace SQLite {


ofJson SQLiteUtils::execute(Statement& query)
{
    ofJson columns;

    while (query.executeStep())
    {
        ofJson row;

        for (int i = 0; i < query.getColumnCount(); ++i)
        {
            const Column& column = query.getColumn(i);

            switch (column.getType())
            {
                case SQLITE_INTEGER:
                {
                    row[column.getName()] = column.getInt64();
                    break;
                }
                case SQLITE_FLOAT:
                {
                    row[column.getName()] = column.getDouble();
                    break;
                }
                case SQLITE_NULL:
                {
                    row[column.getName()] = nullptr;
                    break;
                }
                case SQLITE_TEXT:
                {
                    row[column.getName()] = column.getText();
                    break;
                }
                case SQLITE_BLOB:
                {
                    // Encode blob w/ base64.
                    const char* pBlob = static_cast<const char*>(column.getBlob());
                    int nBytes = column.getBytes();
                    std::ostringstream ss;
                    Poco::Base64Encoder _encoder(ss);
                    _encoder.write(pBlob, nBytes);
                    _encoder.close(); // Flush bytes.
                    row[column.getName()] = ss.str();
                    break;
                }
                default:
                {
                    row[column.getName()] = column.getText();
                }
            }
        }

        columns.push_back(row);
    }

    // Reset the query to use it again.
    query.reset();

    return columns;
}



} // nampace SQLite
