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

#include "memberset.h"

#include <vector>
#include <optional>
#include <QString>

namespace simulink::model {

class WorkspaceElement final
{
public:
    WorkspaceElement() = default;
    WorkspaceElement(WorkspaceElement &&) = default;
    WorkspaceElement &operator=(WorkspaceElement &&) = default;

public:
    auto name() const -> const QString &;
    auto setName(QString name) -> void;

    auto workspaceDataType() const -> const QString &;
    auto setWorkspaceDataType(QString workspaceDataType) -> void;

    auto baseType() const -> const QString &;
    auto setBaseType(QString baseType) -> void;

    auto dataScope() const -> const QString &;
    auto setDataScope(QString dataScope) -> void;

    auto description() const -> const QString &;
    auto setDescription(QString description) -> void;

    auto headerFile() const -> const QString &;
    auto setHeaderFile(QString headerFile) -> void;

    auto memberSet() const -> const std::optional<MemberSet> &;
    auto setMemberSet(MemberSet memberSet) -> void;

private:
    QString m_name;
    QString m_workspaceDataType;
    QString m_baseType;
    QString m_dataScope;
    QString m_description;
    QString m_headerFile;

    std::optional<MemberSet> m_memberSet;
};

} // namespace simulink::model
