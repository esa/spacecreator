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

#include <QList>
#include <asn1library/asn1/types/typereadingvisitor.h>
#include <asn1library/asn1/value.h>
#include <promela/PromelaModel/expression.h>
#include <promela/PromelaModel/sequence.h>
#include <promela/PromelaModel/variableref.h>

namespace promela::translator {
/**
 * @brief Visitor to generate value assignments.
 */
class ValueAssignmentVisitor : public Asn1Acn::Types::TypeReadingVisitor
{
public:
    /**
     * @brief Constructor.
     *
     * @param value value to assign to given variable with given type
     * @param sequence promela sequence to append statements
     * @param target target variable to assign the value
     * @param typeName name of the type of variable
     */
    ValueAssignmentVisitor(
            Asn1Acn::Value *value, model::Sequence &sequence, const model::VariableRef &target, QString typeName);
    /**
     * @brief Visit Asn1Acn::Types::Boolean
     *
     * @param type boolean value
     */
    void visit(const Asn1Acn::Types::Boolean &type) override;
    /**
     * @brief Visit Asn1Acn::Types::Null
     *
     * @param type null value
     */
    void visit(const Asn1Acn::Types::Null &type) override;
    /**
     * @brief Visit Asn1Acn::Types::BitString
     *
     * @param type bit string value
     */
    void visit(const Asn1Acn::Types::BitString &type) override;
    /**
     * @brief Visit Asn1Acn::Types::OctetString
     *
     * @param type octet string value
     */
    void visit(const Asn1Acn::Types::OctetString &type) override;
    /**
     * @brief Visit Asn1Acn::Types::IA5String
     *
     * @param type IA5String value
     */
    void visit(const Asn1Acn::Types::IA5String &type) override;
    /**
     * @brief Visit Asn1Acn::Types::NumericString
     *
     * @param type numeric string value
     */
    void visit(const Asn1Acn::Types::NumericString &type) override;
    /**
     * @brief Visit Asn1Acn::Types::Enumerated
     *
     * @param type enumerated value
     */
    void visit(const Asn1Acn::Types::Enumerated &type) override;
    /**
     * @brief Visit Asn1Acn::Types::Choice
     *
     * @param type choice value
     */
    void visit(const Asn1Acn::Types::Choice &type) override;
    /**
     * @brief Visit Asn1Acn::Types::Sequence
     *
     * @param type sequence value
     */
    void visit(const Asn1Acn::Types::Sequence &type) override;
    /**
     * @brief Visit Asn1Acn::Types::SequenceOf
     *
     * @param type 'sequence of' value
     */
    void visit(const Asn1Acn::Types::SequenceOf &type) override;
    /**
     * @brief Visit Asn1Acn::Types::Real
     *
     * @param type real value
     */
    void visit(const Asn1Acn::Types::Real &type) override;
    /**
     * @brief Visit Asn1Acn::Types::LabelType
     *
     * @param type label type value
     */
    void visit(const Asn1Acn::Types::LabelType &type) override;
    /**
     * @brief Visit Asn1Acn::Types::Integer
     *
     * @param type integer value
     */
    void visit(const Asn1Acn::Types::Integer &type) override;
    /**
     * @brief Visit Asn1Acn::Types::UserdefinedType
     *
     * @param type user defined type value
     */
    void visit(const Asn1Acn::Types::UserdefinedType &type) override;

private:
    QVector<QChar> getBytesFromString(const QString &str);
    QVector<unsigned char> getBytesFromOctetString(const QString &str);
    int hexdigit(int c);

private:
    Asn1Acn::Value *m_value;
    model::Sequence &m_sequence;
    model::VariableRef m_target;
    QString m_typeName;
};
}
