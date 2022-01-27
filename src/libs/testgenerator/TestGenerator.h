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

#include <QTextStream>
#include <asn1/asn1model.h>
#include <asn1library/asn1/types/type.h>
#include <csv/CsvModel/csvmodel.h>
#include <ivcore/ivinterface.h>
#include <sstream>
#include <vector>

using Asn1Acn::Asn1Model;
using Asn1Acn::Types::Type;

namespace testgenerator {

class TestGenerator final
{
public:
    static auto generateTestDriver(const csv::CsvModel &testData, const ivm::IVInterface &interface,
            const Asn1Acn::Asn1Model &asn1Model) -> std::stringstream;

private:
    static auto getAsn1Type(const QString &name, const Asn1Model &model) -> Type::ASN1Type;
    static auto qstringToBoolSymbol(const QString &str) -> QString;
    static auto getAssignmentsForRecords(const ivm::IVInterface &interface, const Asn1Acn::Asn1Model &asn1Model,
            const csv::CsvModel &testData, unsigned int index) -> QString;
    static auto removePiPrefix(const QString &str) -> QString;

    // which column in CSV model matches which interface input parameter
    // mappings index - parameter number
    // mappings value - data column in CSV model
    static std::vector<unsigned int> mappings;

    static unsigned int outputParameters;
};

} // testgenerator
