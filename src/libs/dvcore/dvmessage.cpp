/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "dvmessage.h"

#include "dvconnection.h"
#include "dvnode.h"

namespace dvm {

DVMessage::DVMessage(QObject *parent)
    : DVObject(DVObject::Type::Message, {}, parent)
{
}

QString DVMessage::titleUI() const
{
    return QString("%1.%2 -> %3.%4").arg(fromFunction(), fromInterface(), toFunction(), toInterface());
}

QString DVMessage::fromFunction() const
{
    return entityAttributeValue(meta::Props::token(meta::Props::Token::from_function)).toString();
}

void DVMessage::setFromFunction(const QString &from)
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::from_function), from);
}

QString DVMessage::fromInterface() const
{
    return entityAttributeValue(meta::Props::token(meta::Props::Token::from_interface)).toString();
}

void DVMessage::setFromInterface(const QString &from)
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::from_interface), from);
}

DVNode *DVMessage::fromNode() const
{
    auto connection = qobject_cast<const DVConnection *>(this);
    if (!connection) {
        return nullptr;
    }
    return connection->sourceNode();
}

QString DVMessage::toFunction() const
{
    return entityAttributeValue(meta::Props::token(meta::Props::Token::to_function)).toString();
}

void DVMessage::setToFunction(const QString &to)
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::to_function), to);
}

QString DVMessage::toInterface() const
{
    return entityAttributeValue(meta::Props::token(meta::Props::Token::to_interface)).toString();
}

void DVMessage::setToInterface(const QString &to)
{
    setEntityAttribute(meta::Props::token(meta::Props::Token::to_interface), to);
}

DVNode *DVMessage::toNode() const
{
    auto connection = qobject_cast<const DVConnection *>(this);
    if (!connection) {
        return nullptr;
    }
    return connection->targetNode();
}

} // namespace dvm
