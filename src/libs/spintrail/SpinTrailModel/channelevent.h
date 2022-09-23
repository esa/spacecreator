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

#include <QList>
#include <QString>

namespace spintrail::model {
class ChannelEvent
{
public:
    enum class Type
    {
        Send,
        Recv,
    };

    ChannelEvent(Type type, QString proctypeName, QString channelName, QList<QString> parameters);

    void setType(Type type);
    Type getType() const;

    void setProctypeName(QString proctypeName);
    const QString &getProctypeName() const;

    void setChannelName(QString channelName);
    const QString &getChannelName() const;

    void setParameters(QList<QString> parameters);
    const QList<QString> &getParameters() const;

private:
    Type m_type;
    QString m_proctypeName;
    QString m_channelName;
    QList<QString> m_parameters;
};
}
