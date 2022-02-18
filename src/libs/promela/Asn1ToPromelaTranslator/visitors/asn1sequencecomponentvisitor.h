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

#include <QString>
#include <asn1library/asn1/sequencecomponentvisitor.h>
#include <promela/PromelaModel/promelamodel.h>
#include <promela/PromelaModel/utype.h>

namespace promela::translator {
/**
 * @brief Visitor for ASN.1 SequenceComponent
 *
 * This is a part of Asn1ToPromelaTranslator
 */
class Asn1SequenceComponentVisitor final : public ::Asn1Acn::SequenceComponentVisitor
{
public:
    /**
     * @brief Contructor.
     *
     * @param promelaModel target promela model
     * @param baseTypeName base name for nested types
     * @param enhancedSpinSupport  if true, then generate model for enhanced spin
     */
    Asn1SequenceComponentVisitor(
            ::promela::model::PromelaModel &promelaModel, QString baseTypeName, bool enhancedSpinSupport);

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

    /**
     * @brief Check if ASN.1 Sequence component was visited.
     *
     * @return true if component was visited, otherwise false.
     */
    bool isComponentPresent() const;
    /**
     * @brief Getter for name of visited sequence component.
     *
     * @return Name of sequence component.
     * @throws std::bad_optional_access if component was not visited.
     */
    QString getComponentName() const;
    /**
     * @brief Getter for type of visited sequence component.
     *
     * @return Type of sequence component.
     * @throws std::bad_optional_access if component was not visited.
     */
    ::promela::model::DataType getComponentType() const;
    /**
     * @brief Check if visited component is optional in SEQUENCE.
     *
     * @return true if component is optional, false if component is mandatory of component was not visited.
     */
    bool isComponentOptional() const;

private:
    ::promela::model::PromelaModel &m_promelaModel;
    const QString m_baseTypeName;
    const bool m_enhancedSpinSupport;

    std::optional<QString> m_componentName;
    std::optional<::promela::model::DataType> m_componentType;
    bool m_optionalComponent;
};
}
