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

#include "abstractsystemchecks.h"
#include "dvconnection.h"
#include "dvfunction.h"
#include "dvmodel.h"
#include "dvnode.h"
#include "errorhub.h"

#include <QDebug>

namespace dvm {

DVMessage::DVMessage(QObject *parent)
    : DVObject(DVObject::Type::Message, {}, parent)
{
}

bool DVMessage::postInit()
{
    auto nodeHasFunction = [&](DVNode *node, const QString &func) -> bool {
        if (!node) {
            shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Message %1 has invalid node").arg(title()));
            return false;
        }
        if (!node->containsFunction(func)) {
            shared::ErrorHub::addError(shared::ErrorItem::Error,
                    tr("Message %1 has invalid source function '%2.%3'").arg(title(), node->title(), func));
            return false;
        }
        return true;
    };

    return nodeHasFunction(fromNode(), fromFunction()) && nodeHasFunction(toNode(), toFunction());
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

QStringList DVMessage::fromFunctionPath() const
{
    return pathOfFunction(fromFunction(), fromNode());
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
    auto connection = qobject_cast<const DVConnection *>(parent());
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

QStringList DVMessage::toFunctionPath() const
{
    return pathOfFunction(toFunction(), toNode());
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
    auto connection = qobject_cast<const DVConnection *>(parent());
    if (!connection) {
        return nullptr;
    }
    return connection->targetNode();
}

QStringList DVMessage::pathOfFunction(const QString &functionName, DVNode *node) const
{
    Q_ASSERT(node != nullptr);

    for (const DVPartition *partition : node->partitions()) {
        for (const DVFunction *func : partition->functions()) {
            if (func->title() == functionName) {
                return func->path();
            }
        }
    }

    return { functionName };
}

} // namespace dvm
