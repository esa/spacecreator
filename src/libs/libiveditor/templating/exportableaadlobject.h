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

namespace aadlinterface {

/**
 * @brief The ExportableAADLObject is a common class to export AADLObject and its successors
 */
class ExportableAADLObject : public templating::AbstractExportableObject
{
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString groupName READ groupName)
    Q_PROPERTY(QVariantList attributes READ attributes)
    Q_PROPERTY(QVariantList properties READ properties)
    Q_PROPERTY(QStringList path READ path)

public:
    explicit ExportableAADLObject(const aadl::AADLObject *aadlObject = nullptr);

    QString groupName() const override;
    QString name() const;

    static QVariant createFrom(const aadl::AADLObject *aadlObject);

    QVariantList attributes() const;
    QVariantList properties() const;
    QStringList path() const;

protected:
    static QVariantList generateProperties(const QHash<QString, QVariant> &props);
};

}

Q_DECLARE_METATYPE(aadlinterface::ExportableAADLObject)
Q_DECLARE_TYPEINFO(aadlinterface::ExportableAADLObject, Q_MOVABLE_TYPE);

template<>
inline aadlinterface::ExportableAADLObject qvariant_cast<aadlinterface::ExportableAADLObject>(const QVariant &v)
{
    return *static_cast<const aadlinterface::ExportableAADLObject *>(v.constData());
}
