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
#include "types/type.h"

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

    const auto testRecordsSize = testData.records().size();

    std::stringstream ss;
    ss << "/**\n"
          " * This file was generated automatically by TestGenerator\n"
          " */\n"
          "\n"
          "#include \"testdriver.h\"\n"
          "//#include <stdio.h>"
          "\n"
          "\n";
    ss << QString("#define TEST_DATA_SIZE %1").arg(testRecordsSize).toStdString();
    ss << "\n"
          "\n"
          "typedef struct {\n";

    for (const auto &param : interface.params()) {
        ss << "    asn1Scc" << param.paramTypeName().toStdString() << " " << param.name().toStdString() << ";\n";
    }

    ss << "} TestVector;\n"
          "\n"
          "TestVector testData[TEST_DATA_SIZE] = { 0 };\n"
          "\n"
          "void notifyTestFinished(void)\n"
          "{\n"
          "    volatile int a;\n"
          "    while (true) {\n"
          "        a = 0;\n"
          "        break;\n"
          "    }\n"
          "}\n"
          "\n"
          "void testdriver_startup(void)\n"
          "{\n";

    for (unsigned long int i = 0; i < testRecordsSize; i++) {
        // switch on parameter type
        const auto &ifParams = interface.params();
        for (const auto &param : ifParams) {
            (void)param;
            ss << QString("    testData[%1].%2 = ").arg(i).arg(param.name()).toStdString();
            // qDebug() << param.paramTypeName(); // TODO: get applicable type (float/int/bool)
            ss << "\n";
        }
    }

    QString testDriver = "";

    // TODO
    ss << testDriver.toStdString();

    return ss;
}

} // testgenerator
