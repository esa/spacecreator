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

#include "integersubset.h"

#include <asn1library/asn1/types/typereadingvisitor.h>
#include <optional>
#include <promela/PromelaModel/promelamodel.h>
#include <vector>

namespace promela::translator {
/**
 * @brief Visitor for type in SequenceOf
 *
 * This is a part of Asn1ToPromelaTranslator.
 *
 */
class Asn1ItemTypeVisitor : public Asn1Acn::Types::TypeReadingVisitor
{
public:
    /**
     * @brief Constructor
     *
     * @param promelaModel target promela model
     * @param baseTypeName base name for new types
     * @param name name for new types
     * @param generateInits if true, then generate initialization inlines
     * @param enhancedSpinSupport  if true, then generate model for enhanced spin
     */
    Asn1ItemTypeVisitor(model::PromelaModel &promelaModel, std::optional<QString> baseTypeName, QString name,
            bool generateInits, bool enhancedSpinSupport);

    /**
     * @brief Getter for result promela data type
     *
     * @return result data type
     */
    const std::optional<model::DataType> &getResultDataType() const noexcept;

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
    inline static const QString m_assignValueInlineSuffix = "_assign_value";
    inline static const QString m_initializeValueInlineSuffix = "_init_value";
    inline static const QString m_rangeCheckInlineSuffix = "_range_check";
    inline static const QString m_sizeCheckInlineSuffix = "_size_check";

private:
    QString constructTypeName(QString name);
    void addSimpleValueAssignmentInline(const QString &typeName);
    void addSimpleArrayAssignInlineValue(const QString &typeName, int length, bool lengthFieldPresent);
    void addAssignValueInline(const QString &typeName, model::Sequence sequence);

    void addSimpleValueInitializationInline(const QString &typeName, model::InlineCall::Argument initValue);
    void addIA5StringElementInitializationInline();
    void addOctetStringElementInitializationInline();
    void addEmptyValueInitializationInline(const QString &typeName);
    void addInitializeValueInline(const QString &typeName, model::Sequence sequence);

    void addBoolRangeCheckInline(const Asn1Acn::Types::Boolean &type, const QString &typeName);
    void addEnumRangeCheckInline(const QString &typeName, const std::vector<QString> &allowedValues);
    void addIntegerRangeCheckInline(const QString &typeName, const IntegerSubset &rangeSubsets);
    void addRangeCheckInline(const model::Expression &type, const QString &typeName);
    void addSizeCheckInline(const std::size_t minValue, const std::size_t maxValue, const QString &typeName);
    QString getAssignValueInlineNameForNestedType(const QString &utype, const QString &field) const;
    QString buildCheckArgumentName(const QString &typeName, const QString &postfix) const;

    model::ForLoop createSequenceOfDataLoop(const QString &utypeName, const model::Expression loopRangeEnd) const;
    model::ForLoop createSequenceOfZeroingLoop(const QString &utypeName, const std::size_t loopRangeEnd) const;

private:
    model::PromelaModel &m_promelaModel;
    std::optional<QString> m_baseTypeName;
    QString m_name;
    bool m_generateInits;
    bool m_enhancedSpinSupport;
    std::optional<model::DataType> m_resultDataType;
};
}
