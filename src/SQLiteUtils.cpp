// =============================================================================
//
// Copyright (c) 2015-2016 Christopher Baker <http://christopherbaker.net>
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
