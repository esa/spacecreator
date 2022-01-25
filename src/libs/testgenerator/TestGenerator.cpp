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
#include "common.h"

#include <QDebug>
#include <ivcore/ivfunctiontype.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivobject.h>
#include <sstream>

namespace testgenerator {

auto TestGenerator::generateTestDriver(const csv::CsvModel &testData, const ivm::IVInterface &interface,
        const Asn1Acn::Asn1Model &asn1Model) -> std::stringstream
{
    if (testData.records().empty()) {
        throw TestGeneratorException("Given test data is empty");
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
        const auto &ifParams = interface.params();
        for (int j = 0; j < ifParams.size(); j++) {
            const auto &param = ifParams[j];

            ss << QString("    testData[%1].%2 = ").arg(i).arg(param.name()).toStdString();

            switch (getAsn1Type(param.paramTypeName(), asn1Model)) {
            case Asn1Acn::Types::Type::INTEGER:
                ss << "integer";
                break;
            case Asn1Acn::Types::Type::REAL:
                ss << "real (float)";
                break;
            case Asn1Acn::Types::Type::BOOLEAN: {
                ss << "bool ";

                auto valStr = "";
                const auto val = testData.records()[i]->fields()[j].toInt();
                if (val == 0) {
                    valStr = "0";
                } else if (val == 1) {
                    valStr = "1";
                } else {
                    throw TestGeneratorException("Invalid boolean value");
                }
                ss << valStr;

                break;
            }
            case Asn1Acn::Types::Type::UNKNOWN:
                throw TestGeneratorException("Interface parameter type not present in ASN.1 file");
            default:
                throw TestGeneratorException("Interface parameter type not yet implemented");
            }

            if (interface.function() == nullptr) {
                throw TestGeneratorException("Interface has no Function set.");
            }

            ss << "\n";
        }
    }

    return ss;
}

auto TestGenerator::getAsn1Type(const QString &name, const Asn1Model &model) -> Type::ASN1Type
{
    for (const auto &datum : model.data()) {
        const auto &definitionsList = datum->definitionsList();
        for (const auto &definition : definitionsList) {
            for (const auto &type : definition->types()) {
                if (type->name().compare(name) == 0) {
                    return type->typeEnum();
                }
            }
        }
    }
    return Type::ASN1Type::UNKNOWN;
}

} // testgenerator
