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

#include <asn1library/asn1/namedvalue.h>
#include <asn1library/asn1/sequencecomponentvisitor.h>
#include <promela/PromelaModel/expression.h>
#include <promela/PromelaModel/sequence.h>
#include <promela/PromelaModel/variableref.h>

namespace promela::translator {
/**
 * @brief Visitor for ASN.1 SequenceComponent, for generation of value assignments
 */
class SequenceComponentValueVisitor final : public ::Asn1Acn::SequenceComponentVisitor
{
public:
    /**
     * @brief Constructor.
     *
     * @param value value for SEQUENCE datatype
     * @param sequence promela sequence to append statements
     * @param target target variable to assign value
     * @param sequenceTypeName name of SEQUENCE datatype
     */
    SequenceComponentValueVisitor(const ::Asn1Acn::NamedValue *value, ::promela::model::Sequence &sequence,
            const ::promela::model::VariableRef &target, QString sequenceTypeName);
    /**
     * @brief Visit ::Asn1Acn::AsnSequenceComponent
     *
     * @param component component to visit
     */
    void visit(const ::Asn1Acn::AsnSequenceComponent &component) override;
    /**
     * @brief Visit ::Asn1Acn::AcnSequenceComponent
     *
     * @param component component to visit
     */
    void visit(const ::Asn1Acn::AcnSequenceComponent &component) override;

private:
    const ::Asn1Acn::NamedValue *m_value;
    ::promela::model::Sequence &m_sequence;
    ::promela::model::VariableRef m_target;
    QString m_sequenceTypeName;
};
}
