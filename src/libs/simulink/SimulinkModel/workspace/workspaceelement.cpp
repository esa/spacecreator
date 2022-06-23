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

#include "workspaceelement.h"

namespace simulink::model {

const QString &WorkspaceElement::name() const
{
    return m_name;
}

void WorkspaceElement::setName(QString name)
{
    m_name = std::move(name);
}

const QString &WorkspaceElement::workspaceDataType() const
{
    return m_workspaceDataType;
}

void WorkspaceElement::setWorkspaceDataType(QString workspaceDataType)
{
    m_workspaceDataType = std::move(workspaceDataType);
}

const QString &WorkspaceElement::baseType() const
{
    return m_baseType;
}

void WorkspaceElement::setBaseType(QString baseType)
{
    m_baseType = std::move(baseType);
}

const QString &WorkspaceElement::dataScope() const
{
    return m_dataScope;
}

void WorkspaceElement::setDataScope(QString dataScope)
{
    m_dataScope = std::move(dataScope);
}

const QString &WorkspaceElement::description() const
{
    return m_description;
}

void WorkspaceElement::setDescription(QString description)
{
    m_description = std::move(description);
}

const QString &WorkspaceElement::headerFile() const
{
    return m_headerFile;
}

void WorkspaceElement::setHeaderFile(QString headerFile)
{
    m_headerFile = std::move(headerFile);
}

const std::optional<MemberSet> &WorkspaceElement::memberSet() const
{
    return m_memberSet;
}

void WorkspaceElement::setMemberSet(MemberSet memberSet)
{
    m_memberSet = std::move(memberSet);
}

} // namespace simulink::model
