/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2022 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "TestGenerator.h"

#include "TestGeneratorException.h"

#include <QDebug>
#include <sstream>

namespace testgenerator {

auto TestGenerator::generateTestDriver(const csv::CsvModel &testData, const ivm::IVInterface &interface)
        -> std::stringstream
{
    (void)interface;

    if (testData.records().empty()) {
        throw TestGeneratorException("Given test data is empty");
    } else {
        qDebug() << "loaded CSV not empty";
    }

    std::stringstream ss;
    ss << "/**\n";
    ss << " * This file was generated automatically by TestGenerator\n";
    ss << " */\n";
    ss << "\n";
    ss << "#include \"testdriver.h\"\n";
    ss << "//#include <stdio.h>";
    ss << "\n";
    ss << "\n";
    ss << QString("#define TEST_DATA_SIZE %1").arg(testData.records().size()).toStdString();

    QString testDriver = "";

    // TODO
    ss << testDriver.toStdString();

    return ss;
}

} // testgenerator
