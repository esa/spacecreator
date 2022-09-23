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

#include "channelevent.h"

namespace spintrail::model {

ChannelEvent::ChannelEvent(Type type, QString proctypeName, QString channelName, QList<QString> parameters)
    : m_type(type)
    , m_proctypeName(std::move(proctypeName))
    , m_channelName(std::move(channelName))
    , m_parameters(std::move(parameters))
{
}

void ChannelEvent::setType(Type type)
{
    m_type = type;
}

ChannelEvent::Type ChannelEvent::getType() const
{
    return m_type;
}

void ChannelEvent::setProctypeName(QString proctypeName)
{
    m_proctypeName = std::move(proctypeName);
}

const QString &ChannelEvent::getProctypeName() const
{
    return m_proctypeName;
}

void ChannelEvent::setChannelName(QString channelName)
{
    m_channelName = std::move(channelName);
}

const QString &ChannelEvent::getChannelName() const
{
    return m_channelName;
}

void ChannelEvent::setParameters(QList<QString> parameters)
{
    m_parameters = std::move(parameters);
}

const QList<QString> &ChannelEvent::getParameters() const
{
    return m_parameters;
}
}
