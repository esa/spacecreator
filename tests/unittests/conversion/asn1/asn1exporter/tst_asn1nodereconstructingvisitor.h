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
#include <asn1library/asn1/typeassignment.h>
#include <memory>

namespace Asn1Acn {
namespace Types {
class Type;
} // namespace Types;
namespace Constraints {
template<typename ValueType>
class Constraint;
template<typename ValueType>
class WithConstraints;
} // namespace Constraint;
} // namespace Asn1Acn;

namespace conversion::asn1::test {

class tst_Asn1NodeReconstructingVisitor : public QObject
{
    Q_OBJECT

private slots:
    void testSingleImportedType();
    void testMultipleImportedTypes();
    void testSingleImportedValue();
    void testMultipleImportedValues();

    void testValueAssignmentSimpleValue();
    void testValueAssignmentBitString();
    void testValueAssignmentBoolean();
    void testValueAssignmentOctetString();
    void testValueAssignmentIA5String();
    void testValueAssignmentChoice();
    void testValueAssignmentMultipleValues();
    void testValueAssignmentNamedValue();

    void testTypeAssignmentBoolean();
    void testTypeAssignmentNull();

    void testTypeAssignmentBitString();
    void testTypeAssignmentBitStringWithValue();

    void testTypeAssignmentOctetString();
    void testTypeAssignmentOctetStringWithValue();

    void testTypeAssignmentIA5String();
    void testTypeAssignmentIA5StringWithValue();

    void testTypeAssignmentNumericString();
    void testTypeAssignmentNumericStringWithValue();

    void testTypeAssignmentEnumerated();

    void testTypeAssignmentChoice();
    void testTypeAssignmentNestedChoice();

    void testTypeAssignmentSequence();
    void testTypeAssignmentNestedSequence();

    void testTypeAssignmentSequenceOf();
    void testTypeAssignmentSequenceOfWithValue();

    void testTypeAssignmentReal();
    void testTypeAssignmentRealWithValue();

    void testTypeAssignmentInteger();
    void testTypeAssignmentIntegerWithValue();

    void testTypeAssignmentUserDefined();
    void testTypeAssignmentUserDefinedWithValue();

    void testTypeAssignmentBooleanAcnParams();

private:
    std::unique_ptr<Asn1Acn::Definitions> createDefinitions(const QString &name) const;

    QString createSingleValueValueAssignmentValue(const QString &typeName, const QString &typeValue,
            std::function<QString(const QString &)> printer = {}) const;

    void testSimpleTypeAssignment(const QString &astValue, const QString &asn1Value) const;

    std::unique_ptr<Asn1Acn::TypeAssignment> createSimpleTypeAssignment(const QString &astTypeName) const;

    template<typename T>
    std::unique_ptr<Asn1Acn::TypeAssignment> createTypeAssignmentWithConstraint(
            const QString &astTypeName, Asn1Acn::Constraints::Constraint<T> *contraint) const;

    QString createMultipleValueValueAssignmentValue() const;
    QString createNamedValueAssignmentValue() const;
    QString createComponentialTypeAssignmentValue(std::unique_ptr<Asn1Acn::Types::Type> type) const;

    QString createChoiceValueAssignmentValue() const;

    QString restoreNode(const Asn1Acn::Node &assignment) const;
};

template<typename T>
std::unique_ptr<Asn1Acn::TypeAssignment> tst_Asn1NodeReconstructingVisitor::createTypeAssignmentWithConstraint(
        const QString &astTypeName, Asn1Acn::Constraints::Constraint<T> *constraint) const
{
    auto assignment = createSimpleTypeAssignment(astTypeName);
    auto &constrainedType = dynamic_cast<Asn1Acn::Constraints::WithConstraints<T> &>(*assignment->type());
    constrainedType.constraints().append(std::unique_ptr<Asn1Acn::Constraints::Constraint<T>>(constraint));

    return assignment;
}

} // namespace conversion::asn1::test
