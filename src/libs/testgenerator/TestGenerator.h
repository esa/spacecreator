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

#pragma once

#include "TestGeneratorContext.h"

#include <QTextStream>
#include <asn1/asn1model.h>
#include <asn1library/asn1/types/type.h>
#include <csv/CsvModel/csvmodel.h>
#include <ivcore/ivinterface.h>
#include <sstream>
#include <vector>

using Asn1Acn::Asn1Model;
using Asn1Acn::Types::Type;
using csv::CsvModel;

namespace testgenerator {

/**
 * @brief Provides a method to generate TestDriver source as a text stream
 *
 */
class TestGenerator final
{
public:
    /**
     * @brief Name of a header generated for TestDriver
     *
     */
    static const QString testDriverHeaderFilename;

    /**
     * @brief Declaration of a startup function
     *
     */
    static const QString testDriverStartupFunctionDeclaration;

    /**
     * @brief Declaration of test start function
     *
     */
    static const QString testDriverStartTestFunctionDeclaration;

    /**
     * @brief Generate a TestDriver source code text stream
     *
     * @param testData  test data containing input test vectors
     * @param interface interface under test
     * @param asn1Model ASN.1 datatypes model (with at least definitions of parameter types)
     *
     * @return Source code text stream
     */
    static auto generateTestDriver(const CsvModel &testData, const ivm::IVInterface &interface,
            const Asn1Model &asn1Model) -> std::stringstream;

    /**
     * @brief Get the TestDriver Required Interface Name
     *
     * @param interface interface under test
     *
     * @return TestDriver Required Interface Name
     */
    static auto getTestDriverRiName(const ivm::IVInterface &interface) -> QString;

private:
    static auto checkTestData(const CsvModel &testData) -> void;
    static auto checkInterface(const ivm::IVInterface &interface) -> void;
    static auto getAsn1Type(const QString &name, const Asn1Model &model) -> Type::ASN1Type;
    static auto qstringToBoolSymbol(const QString &str) -> QString;
    static auto getAssignmentsForRecords(const ivm::IVInterface &interface, const Asn1Model &asn1Model,
            const CsvModel &testData, unsigned int index, const TestGeneratorContext &context) -> QString;
    static auto removePiPrefix(const QString &str) -> QString;
};

} // namespace testgenerator
