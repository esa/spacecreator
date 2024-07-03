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

#include "sequencecomponentvisitor.h"

#include "helper.h"

#include <QDebug>
#include <asn1library/asn1/asnsequencecomponent.h>
#include <conversion/common/escaper/escaper.h>
#include <promela/PromelaCommon/namehelper.h>

namespace promela::translator {

using conversion::Escaper;
using promela::common::PromelaNameHelper;

SequenceComponentVisitor::SequenceComponentVisitor(Operation operation, const Asn1Acn::Asn1Model *asn1Model,
        QString target, QString source, const QString sequenceName)
    : m_operation(operation)
    , m_asn1Model(asn1Model)
    , m_target(std::move(target))
    , m_source(std::move(source))
    , m_sequenceName(std::move(sequenceName))
    , m_componentVisited(false)
    , m_isOptional(false)
    , m_content()
{
}

void SequenceComponentVisitor::visit(const Asn1Acn::AsnSequenceComponent &component)
{
    m_componentVisited = true;
    m_componentName = Escaper::escapeCName(component.name());
    m_isOptional = component.isOptional();

    // TODO escape
    Helper helper(m_asn1Model, m_target + "." + m_componentName, m_source + "." + m_componentName);

    switch (m_operation) {
    case Operation::FROM_PROMELA_TO_C:
        m_content = helper.createAssignmentTemplateFromPromelaToC(
                PromelaNameHelper::createChildTypeNameForCCode(m_sequenceName, m_componentName), component.type());
        break;
    case Operation::FROM_C_TO_PROMELA:
        m_content = helper.createAssignmentTemplateFromCToPromela(
                PromelaNameHelper::createChildTypeNameForCCode(m_sequenceName, m_componentName), component.type());
        break;
    case Operation::LIST_PROMELA_FIELDS: {
        Helper fieldHelper(m_asn1Model, m_target, "");
        m_fields = fieldHelper.generateListOfFields(component.type());
    } break;
    }
}

void SequenceComponentVisitor::visit(const Asn1Acn::AcnSequenceComponent &component)
{
    Q_UNUSED(component);
}

bool SequenceComponentVisitor::hasComponent() const
{
    return m_componentVisited;
}

const QString &SequenceComponentVisitor::getComponentName() const
{
    return m_componentName;
}

bool SequenceComponentVisitor::isOptional() const
{
    return m_isOptional;
}

const QString &SequenceComponentVisitor::getContent() const
{
    return m_content;
}

QList<promela::model::VariableRef> SequenceComponentVisitor::takeListOfFields()
{
    return std::move(m_fields);
}
}
