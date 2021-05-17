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

#pragma once

#include "abstractexportableobject.h"
#include "entityattribute.h"

#include <QVariant>

namespace dvm {
class DVObject;
}

namespace dve {

/**
 * @brief The ExportableDVObject is a common class to export dvm::DVObject and its successors
 */
class ExportableDVObject : public templating::AbstractExportableObject
{
    Q_GADGET
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QVariantList attributes READ attributes)
    Q_PROPERTY(QVariantList properties READ properties)

public:
    explicit ExportableDVObject(const dvm::DVObject *dvObject = nullptr);

    QString name() const;

    QVariantList attributes() const;
    QVariantList properties() const;

    static QVariant createFrom(const dvm::DVObject *dvObject);

protected:
    static QVariantList generateProperties(const EntityAttributes &attributes, bool isProperty);
};

}

Q_DECLARE_METATYPE(dve::ExportableDVObject)
Q_DECLARE_TYPEINFO(dve::ExportableDVObject, Q_MOVABLE_TYPE);

template<>
inline dve::ExportableDVObject qvariant_cast<dve::ExportableDVObject>(const QVariant &v)
{
    return *static_cast<const dve::ExportableDVObject *>(v.constData());
}
