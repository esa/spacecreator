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

#include "dvfunction.h"

#include "abstractsystemchecks.h"
#include "dvmodel.h"
#include "dvpartition.h"

#include <QDebug>

namespace dvm {

DVFunction::DVFunction(DVObject *parent)
    : DVObject(DVObject::Type::Function, {}, parent)
{
}

DVPartition *DVFunction::partition() const
{
    return qobject_cast<DVPartition *>(parent());
}

QString DVFunction::usedImplementation() const
{
    QString value = implementation();
    if (value.isEmpty()) {
        return defaultImplementation();
    }
    return value;
}

void DVFunction::setImplementation(const QString &name)
{
    if (name == implementation()) {
        return;
    }

    setEntityAttribute(meta::Props::token(meta::Props::Token::selected_implementation), name);

    Q_EMIT implementationChanged();
    Q_EMIT usedImplementationChanged();
}

QString DVFunction::implementation() const
{
    QString value = entityAttributeValue(meta::Props::token(meta::Props::Token::selected_implementation)).toString();
    if (model() && model()->ivQueries()) {
        if (!model()->ivQueries()->implementationsForFunction(title()).contains(value)) {
            return "";
        }
    }
    return value;
}

QStringList DVFunction::availableImplementations() const
{
    if (model() && model()->ivQueries()) {
        return model()->ivQueries()->implementationsForFunction(title());
    }
    return {};
}

/*!
   The implementation used, when no explicit one is set
 */
QString DVFunction::defaultImplementation() const
{
    if (model() && model()->ivQueries()) {
        return model()->ivQueries()->defaultImplementationForFunction(title());
    }
    return {};
}

/*!
   Returns the full ath of the function (relevant for nested functions)
   Does update the information from interfaceview if possible.
 */
QStringList DVFunction::path() const
{
    QStringList path;
    const QString tokenName = meta::Props::token(meta::Props::Token::path);
    if (model() && model()->ivQueries()) {
        path = model()->ivQueries()->functionPath(title());

        auto self = const_cast<DVFunction *>(this);
        self->setEntityAttribute(tokenName, QVariant::fromValue(path.join("::")));
    } else {
        path = entityAttributeValue<QString>(tokenName, title()).split("::");
    }
    return path;
}

void DVFunction::updateForExport()
{
    path();
}

} // namespace dvm
