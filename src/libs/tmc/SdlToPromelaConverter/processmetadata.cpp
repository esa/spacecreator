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

#include "processmetadata.h"

namespace tmc::converter {
ProcessMetadata::ProcessMetadata(QString name, std::optional<QFileInfo> systemStructure, QFileInfo process,
        QFileInfo datamodel, QList<QFileInfo> context)
    : m_name(std::move(name))
    , m_systemStructure(std::move(systemStructure))
    , m_process(std::move(process))
    , m_datamodel(std::move(datamodel))
    , m_context(std::move(context))
{
}

const QString &ProcessMetadata::getName() const noexcept
{
    return m_name;
}

const std::optional<QFileInfo> &ProcessMetadata::getSystemStructure() const noexcept
{
    return m_systemStructure;
}

const QFileInfo &ProcessMetadata::getProcess() const noexcept
{
    return m_process;
}

const QFileInfo &ProcessMetadata::getDatamodel() const noexcept
{
    return m_datamodel;
}

const QList<QFileInfo> &ProcessMetadata::getContext() const noexcept
{
    return m_context;
}
}
