/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2024 N7 Space Sp. z o.o.
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
#include <QStringList>
#include <asn1library/asn1/asn1model.h>
#include <asn1library/asn1/sequencecomponentvisitor.h>
#include <promela/PromelaModel/variableref.h>

namespace promela::translator {
class SequenceComponentVisitor final : public Asn1Acn::SequenceComponentVisitor
{
public:
    enum class Direction
    {
        FROM_PROMELA_TO_C,
        FROM_C_TO_PROMELA,
    };

public:
    SequenceComponentVisitor(Direction direction, const Asn1Acn::Asn1Model *asn1Model, QString target, QString source);

    void visit(const Asn1Acn::AsnSequenceComponent &component) override;
    void visit(const Asn1Acn::AcnSequenceComponent &component) override;

    bool hasComponent() const;
    const QString &getComponentName() const;
    bool isOptional() const;
    const QString &getContent() const;
    QList<promela::model::VariableRef> takeListOfFields();

private:
    const Direction m_direction;
    const Asn1Acn::Asn1Model *m_asn1Model;
    const QString m_target;
    const QString m_source;
    bool m_componentVisited;
    QString m_componentName;
    bool m_isOptional;
    QString m_content;
    QList<promela::model::VariableRef> m_fields;
};
}
