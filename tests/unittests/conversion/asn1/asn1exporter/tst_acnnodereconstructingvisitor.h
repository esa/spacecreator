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
#include <memory>

namespace Asn1Acn {
class Node;
namespace Types {
class Type;
} // namespace Types
} // namespace Asn1Acn

namespace conversion::asn1::test {

class tst_AcnNodeReconstructingVisitor : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testEmptyModule();
    void testBoolean();
    void testNull();
    void testInteger();
    void testReal();
    void testBitString();
    void testOctetString();
    void testIA5String();
    void testNumericString();
    void testEnumerated();
    void testSequenceOf();

    void testChoice();
    void testChoiceNested();
    void testChoiceParametrized();

    void testSequenceNested();
    void testSequence();

    void testAcnParameter();
    void testAcnArgument();

    void testIntegerEncodingToString();
    void testRealEncodingToString();
    void testAsciiStringEncodingToString();

private:
    void performTest(
            const QString &name, const QString &expectedResult, std::unique_ptr<Asn1Acn::Types::Type> type) const;
    QString restoreNode(const Asn1Acn::Node &node) const;
};

} // namespace conversion::asn1::test
