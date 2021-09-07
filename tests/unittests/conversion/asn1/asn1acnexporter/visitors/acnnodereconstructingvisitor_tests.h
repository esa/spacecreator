/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
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

#include <QObject>
#include <asn1library/asn1/definitions.h>
#include <asn1library/asn1/file.h>
#include <asn1library/asn1/project.h>
#include <asn1library/asn1/valueassignment.h>
#include <functional>
#include <memory>

namespace Asn1Acn {
namespace Tests {

class AcnNodeReconstructingVisitorTests : public QObject
{
    Q_OBJECT
public:
    explicit AcnNodeReconstructingVisitorTests(QObject *parent = 0);

private slots:
    void test_emptyModule();
    void test_boolean();
    void test_null();
    void test_integer();
    void test_real();
    void test_bitString();
    void test_octetString();
    void test_IA5String();
    void test_numericString();
    void test_enumerated();
    void test_sequenceOf();

    void test_choice();
    void test_choiceNested();
    void test_choiceParametrized();

    void test_sequenceNested();
    void test_sequence();

    void test_acnParameter();
    void test_acnArgument();

    void test_integerEncodingToString();
    void test_realEncodingToString();
    void test_asciiStringEncodingToString();

private:
    void performTest(const QString &name, const QString &expectedResult, std::unique_ptr<Types::Type> type) const;
    QString restoreNode(const Node &node) const;
};

} // namespace Tests
} // namespace Asn1Acn
