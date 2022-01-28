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
#include <algorithm>
#include <asn1/asn1model.h>
#include <csv/CsvModel/csvmodel.h>
#include <ivcore/ivfunctiontype.h>
#include <ivcore/ivinterface.h>
#include <ivcore/ivmodel.h>
#include <ivcore/ivobject.h>
#include <qdebug.h>
#include <shared/common.h>
#include <shared/sharedlibrary.h>
#include <sstream>

using IfDirection = shared::InterfaceParameter::Direction;

namespace testgenerator {

const QString TestGenerator::testDriverHeaderFilename = "testdriver.h";
const QString TestGenerator::testDriverStartupFunctionDeclaration = "void testdriver_startup(void)";
const QString TestGenerator::testDriverStartTestFunctionDeclaration = "void testdriver_PI_StartTest(void)";

auto TestGenerator::getTestDriverRiName(const ivm::IVInterface &interface) -> QString
{
    return QString("testdriver_RI_%1").arg(removePiPrefix(interface.title()));
}

auto TestGenerator::generateTestDriver(
        const CsvModel &testData, const ivm::IVInterface &interface, const Asn1Model &asn1Model) -> std::stringstream
{
    checkTestData(testData);
    checkInterface(interface);

    const TestGeneratorContext context(testData.header().fields(), interface.params());

    const auto testRecordsSize = testData.records().size();
    const QString testDriverRiName = getTestDriverRiName(interface);

    std::stringstream ss;
    ss << "/**\n"
          " * This file was generated automatically by TestGenerator\n"
          " */\n"
          "\n";
    ss << QString("#include \"%1\"\n").arg(testDriverHeaderFilename).toStdString();
    ss << "//#include <stdio.h>"
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
          "\n";
    ss << QString("%1\n").arg(testDriverStartupFunctionDeclaration).toStdString();
    ss << "{\n";

    const unsigned long int lastTestRecordIndex = testRecordsSize - 1;
    for (unsigned long int i = 0; i < lastTestRecordIndex; i++) {
        ss << getAssignmentsForRecords(interface, asn1Model, testData, i, context).toStdString();
        ss << "\n";
    }
    ss << getAssignmentsForRecords(interface, asn1Model, testData, lastTestRecordIndex, context).toStdString();

    ss << "}\n"
          "\n";
    ss << QString("%1\n").arg(testDriverStartTestFunctionDeclaration).toStdString();
    ss << "{\n"
          "    for (unsigned int i = 0; i < TEST_DATA_SIZE; i++) {\n"
          "        // clang-format off\n";
    ss << QString("        %1(\n").arg(testDriverRiName).toStdString();
    for (auto it = interface.params().begin(); it != std::prev(interface.params().end()); it++) {
        ss << QString("                &(testData[i].%1),\n").arg(it->name()).toStdString();
    }
    ss << QString("                &(testData[i].%1)\n").arg(std::prev(interface.params().end())->name()).toStdString();
    ss << "        );\n"
          "        // clang-format on\n"
          "    }\n"
          "    notifyTestFinished();\n"
          "}\n"
          "\n";

    return ss;
}

auto TestGenerator::checkTestData(const CsvModel &testData) -> void
{
    if (testData.records().empty()) {
        throw TestGeneratorException("Given test data is empty");
    }
}

auto TestGenerator::checkInterface(const ivm::IVInterface &interface) -> void
{
    if (interface.kind() == ivm::IVInterface::OperationKind::Cyclic
            || interface.kind() == ivm::IVInterface::OperationKind::Sporadic
            || interface.kind() == ivm::IVInterface::OperationKind::Any) {
        throw TestGeneratorException("Tested interface must be of type Protected or Unprotected");
    }

    if (interface.params().isEmpty()) {
        throw TestGeneratorException("No input parameters in selected interface");
    }

    if (!interface.isProvided()) {
        throw TestGeneratorException("Only provided interface could be tested");
    }

    if (interface.function() == nullptr) {
        throw TestGeneratorException("Interface without function is invalid");
    }

    const QString functionImplementation =
            interface.function()
                    ->entityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::language))
                    .value()
                    .toString();
    if (functionImplementation.compare("C") != 0) {
        throw TestGeneratorException("Only functions with implementation in C could be tested");
    }
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

auto TestGenerator::qstringToBoolSymbol(const QString &str) -> QString
{
    const int val = str.toInt();
    if (val == 0) {
        return "0";
    } else if (val == 1) {
        return "1";
    } else {
        throw "Invalid boolean value in loaded CSV";
    }
}

auto TestGenerator::getAssignmentsForRecords(const ivm::IVInterface &interface, const Asn1Model &asn1Model,
        const CsvModel &testData, const unsigned int testDataRowIndex, const TestGeneratorContext &context) -> QString
{
    QString result;

    const auto &ifParams = interface.params();
    if (ifParams.isEmpty()) {
        return "";
    }

    for (unsigned int j = 0; j < static_cast<unsigned int>(ifParams.size()); j++) {
        const auto &param = ifParams[static_cast<int>(j)];

        result += QString("    testData[%1].%2 = ").arg(testDataRowIndex).arg(param.name());

        if (param.direction() == IfDirection::OUT) {
            result += "0;\n";
            continue;
        }

        const unsigned int srcFieldIndex = testData.header().fields().empty() ? j : context.mappings().at(j);

        switch (getAsn1Type(param.paramTypeName(), asn1Model)) {
        case Asn1Acn::Types::Type::INTEGER:
            result += QString::number(static_cast<int>(testData.field(testDataRowIndex, srcFieldIndex).toDouble()));
            break;
        case Asn1Acn::Types::Type::REAL:
            result += QString::number(testData.field(testDataRowIndex, srcFieldIndex).toFloat());
            break;
        case Asn1Acn::Types::Type::BOOLEAN: {
            result += qstringToBoolSymbol(testData.field(testDataRowIndex, srcFieldIndex));
            break;
        }
        case Asn1Acn::Types::Type::UNKNOWN:
            throw TestGeneratorException("Interface parameter type not present in ASN.1 file");
        default:
            throw TestGeneratorException("Interface parameter type not yet implemented");
        }

        result += ";\n";
    }

    return result;
}

auto TestGenerator::removePiPrefix(const QString &str) -> QString
{
    constexpr int prefixLen = 3;

    if (str.left(prefixLen).compare("PI_") != 0) {
        return str;
    }

    return str.right(str.size() - prefixLen);
}

} // namespace testgenerator
