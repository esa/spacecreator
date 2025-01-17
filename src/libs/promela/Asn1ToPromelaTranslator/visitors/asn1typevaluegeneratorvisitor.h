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

#include <asn1library/asn1/asnsequencecomponent.h>
#include <asn1library/asn1/constraints/constraintlist.h>
#include <asn1library/asn1/types/choice.h>
#include <asn1library/asn1/types/type.h>
#include <asn1library/asn1/types/typereadingvisitor.h>
#include <optional>
#include <promela/PromelaModel/proctypeelement.h>
#include <promela/PromelaModel/promelamodel.h>

using promela::model::Expression;

namespace promela::translator {

/**
 * @brief Visitor for ASN.1 top-level types
 *
 * Visitor generates Promela inlines responsible for value generation for inlines
 *
 * This is a part of Asn1ToPromelaTranslator
 */
class Asn1TypeValueGeneratorVisitor : public Asn1Acn::Types::TypeReadingVisitor
{
public:
    /**

     * @brief Constructor
     *
     * @param   promelaModel    target promela model
     * @param   name            name of ASN.1 type
     * @param   overridenType   ASN.1 type that this type overrides by subtyping
     * @param   delta           delta for real values generation
     */
    Asn1TypeValueGeneratorVisitor(model::PromelaModel &promelaModel, QString name,
            const Asn1Acn::Types::Type *overridenType, const std::optional<float> delta);

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
     * @param type octest string value
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
    class InlineDefAdder final
    {
    public:
        static const QString lengthMemberName;
        static const QString octetGeneratorName;
        static const QString ia5StringGeneratorName;

        static void addRangedIntegerGeneratorToModel(
                const QString &inlineName, model::PromelaModel &model, long minSize, long maxSize);

        static Expression getValueLengthAsExpression(const QString &valueVariableName);
        static Expression getValueLengthMinusConstAsExpression(const QString &valueVariableName, int x);
    };

    auto createValueGenerationInline(model::Sequence sequence) -> void;
    auto getAsnSequenceComponentType(Asn1Acn::AsnSequenceComponent *component) -> Asn1Acn::Types::Type *;
    auto getChoiceComponentType(Asn1Acn::Types::ChoiceAlternative *component) -> Asn1Acn::Types::Type *;
    auto getSequenceComponentTypeName(const Asn1Acn::AsnSequenceComponent &asnComponent, const QString &sequenceName)
            -> QString;
    auto getChoiceComponentTypeName(const Asn1Acn::Types::ChoiceAlternative &choiceComponent, const QString &choiceName)
            -> QString;
    auto generateAsnSequenceComponentInlineCall(Asn1Acn::AsnSequenceComponent *asnSequenceComponent,
            const QString &argumentName) -> std::unique_ptr<model::ProctypeElement>;
    auto modelContainsInlineGenerator(const QString &inlineGeneratorName) -> bool;
    auto isEmbeddedType(const Asn1Acn::Types::Type &type) -> bool;
    auto getInlineInitName(const QString &typeName) -> QString;
    auto getInlineGeneratorName(const QString &typeName) -> QString;
    auto getInlineArgumentName() -> QString;

    template<typename ValueType>
    auto handleOverridenType(const QString &typeName,
            const Asn1Acn::Constraints::ConstraintList<ValueType> &constraints, const QString &initInlineName,
            const QString &valueName, const QString &iteratorName, const size_t minSize, const size_t maxSize,
            model::Sequence *sequence) const -> void;

private:
    model::PromelaModel &m_promelaModel;
    QString m_name;
    const Asn1Acn::Types::Type *m_overridenType;
    const std::optional<float> m_delta;

    enum PresenceFlag
    {
        Absent = 0,
        Present = 1
    };
};

} // namespace promela::translator
