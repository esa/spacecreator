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
#include <ivcore/ivfunction.h>

namespace dvm {

DVMessage::DVMessage(QObject *parent)
    : DVObject(DVObject::Type::Message, {}, parent)
{
}

bool DVMessage::postInit()
{
    DVNode *node1 = fromNode();
    if (!node1) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Message %1 has invalid node").arg(title()));
        return false;
    }
    DVNode *node2 = toNode();
    if (!node2) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Message %1 has invalid node").arg(title()));
        return false;
    }

    bool direction1Ok = node1->containsFunction(fromFunction()) && node2->containsFunction(toFunction());
    bool direction2Ok = node2->containsFunction(fromFunction()) && node1->containsFunction(toFunction());

    if (!direction1Ok && !direction2Ok) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Message %1 has invalid function").arg(title()));
        return false;
    }
    return true;
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

QString DVMessage::resolvedTargetFunction() const
{
    const auto model = getModel();
    if (model == nullptr) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("DVMessage has no parent model"));
        return "";
    }
    if (model->ivQueries() == nullptr) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("ivQueries are not initialized in DVModel"));
        return "";
    }

    return model->ivQueries()->resolvedTargetFunction(fromFunction(), fromInterface(), toFunction(), toInterface());
}

QString DVMessage::resolvedTargetInterface() const
{
    const auto model = getModel();
    if (model == nullptr) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("DVMessage has no parent model"));
        return "";
    }
    if (model->ivQueries() == nullptr) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("ivQueries are not initialized in DVModel"));
        return "";
    }
    return model->ivQueries()->resolvedTargetInterface(fromFunction(), fromInterface(), toFunction(), toInterface());
}

DVModel *DVMessage::getModel() const
{
    QObject *parentObject = parent();
    DVModel *model = NULL;
    while (parentObject != NULL) {
        model = qobject_cast<DVModel *>(parentObject);
        if (model != NULL) {
            return model;
        }
        parentObject = parentObject->parent();
    }
    return NULL;
}

} // namespace dvm
