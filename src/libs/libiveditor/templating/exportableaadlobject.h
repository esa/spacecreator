/*
   Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#pragma once

#include "abstractexportableobject.h"

#include <QVariant>

namespace aadl {
class AADLObject;
}

namespace templating {

/**
 * @brief The ExportableAADLObject is a common class to export AADLObject and its successors
 */
class ExportableAADLObject : public AbstractExportableObject
{
    Q_GADGET
    Q_PROPERTY(QString groupName READ groupName)
    Q_PROPERTY(QVariantList attributes READ attributes)
    Q_PROPERTY(QVariantList properties READ properties)

public:
    explicit ExportableAADLObject(const aadl::AADLObject *aadlObject = nullptr);

    QString groupName() const override;

    static QVariant createFrom(const aadl::AADLObject *aadlObject);

    QVariantList attributes() const;
    QVariantList properties() const;

protected:
    static QVariantList generateProperties(const QHash<QString, QVariant> &props);
};

}

Q_DECLARE_METATYPE(templating::ExportableAADLObject)
Q_DECLARE_TYPEINFO(templating::ExportableAADLObject, Q_MOVABLE_TYPE);

template<>
inline templating::ExportableAADLObject qvariant_cast<templating::ExportableAADLObject>(const QVariant &v)
{
    return *static_cast<const templating::ExportableAADLObject *>(v.constData());
}
