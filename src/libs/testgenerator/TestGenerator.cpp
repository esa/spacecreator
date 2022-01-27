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
#include "parameter.h"

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

namespace testgenerator {

std::vector<unsigned int> TestGenerator::mappings;

auto TestGenerator::generateTestDriver(const csv::CsvModel &testData, const ivm::IVInterface &interface,
        const Asn1Acn::Asn1Model &asn1Model) -> std::stringstream
{
    if (testData.records().empty()) {
        throw TestGeneratorException("Given test data is empty");
    }

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

    const auto testRecordsSize = testData.records().size();
    mappings = std::vector<unsigned int>(testRecordsSize, 0);

    const auto &headerFields = testData.header().fields();
    const auto &ifParams = interface.params();
    if (!headerFields.empty()) {
        if (static_cast<int>(headerFields.size()) != ifParams.size() - 1) {
            throw TestGeneratorException("Imported CSV contains invalid number of data columns");
        }
        std::vector<bool> elementsFound(testRecordsSize, false);
        for (unsigned int i = 0; i < static_cast<unsigned int>(ifParams.size()); i++) {
            const auto &param = ifParams.at(static_cast<int>(i));
            const QString &name = param.name();
            if (param.direction() == shared::InterfaceParameter::Direction::OUT) {
                elementsFound.at(i) = true;
                continue;
            }
            for (unsigned int j = 0; j < headerFields.size(); j++) {
                const auto &field = headerFields.at(j);
                if (name.compare(field) == 0) {
                    elementsFound.at(i) = true;
                    mappings.at(i) = j;
                    break;
                }
            }
        }
        if (std::any_of(elementsFound.begin(), elementsFound.end(), [](const auto &found) -> bool { return !found; })) {
            // throw TestGeneratorException("Header fields do not match interface parameter names");
            qDebug() << "something not found";
        }
    } else {
        for (unsigned int i = 0; i < testRecordsSize; i++) {
            mappings[i] = i;
        }
    }

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

    for (unsigned long int i = 0; i < testRecordsSize - 1; i++) {
        ss << getAssignmentsForRecords(interface, asn1Model, testData, i).toStdString();
        ss << "\n";
    }
    ss << getAssignmentsForRecords(interface, asn1Model, testData, testRecordsSize - 1).toStdString();

    ss << "}\n"
          "\n"
          "void testdriver_PI_StartTest(void)\n"
          "{\n"
          "    for (unsigned int i = 0; i < TEST_DATA_SIZE; i++) {\n"
          "        // clang-format off\n";
    ss << QString("        testdriver_RI_%1(\n").arg(removePiPrefix(interface.title())).toStdString();
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

auto TestGenerator::getAssignmentsForRecords(const ivm::IVInterface &interface, const Asn1Acn::Asn1Model &asn1Model,
        const csv::CsvModel &testData, const unsigned int index) -> QString
{
    QString result;

    const auto &ifParams = interface.params();
    if (ifParams.isEmpty()) {
        return "";
    }
    // TODO: map between header position/column and parameter position
    for (unsigned int j = 0; j < static_cast<unsigned int>(ifParams.size() - 1);
            j++) { // one of the records is a result field
        const auto &param = ifParams[static_cast<int>(j)];

        result += QString("    testData[%1].%2 = ").arg(index).arg(param.name());

        switch (getAsn1Type(param.paramTypeName(), asn1Model)) {
        case Asn1Acn::Types::Type::INTEGER:
            result += QString::number(static_cast<int>(testData.field(index, mappings.at(j)).toDouble()));
            break;
        case Asn1Acn::Types::Type::REAL:
            result += QString::number(testData.field(index, mappings.at(j)).toFloat());
            break;
        case Asn1Acn::Types::Type::BOOLEAN: {
            result += qstringToBoolSymbol(testData.field(index, mappings.at(j)));
            break;
        }
        case Asn1Acn::Types::Type::UNKNOWN:
            throw TestGeneratorException("Interface parameter type not present in ASN.1 file");
        default:
            throw TestGeneratorException("Interface parameter type not yet implemented");
        }

        result += ";\n";

        if (interface.function() == nullptr) {
            throw TestGeneratorException("Interface has no Function set.");
        }
    }

    result += QString("    testData[%1].%2 = 0;\n").arg(index).arg(ifParams.last().name());

    return result;
}

auto TestGenerator::removePiPrefix(const QString &str) -> QString
{
    constexpr int prefixLen = 3;

    if (str.left(prefixLen).compare("PI_") != 0) {
        throw TestGeneratorException("Selected interface does not have required 'PI_' prefix");
    }

    return str.right(str.size() - prefixLen);
}

} // namespace testgenerator
