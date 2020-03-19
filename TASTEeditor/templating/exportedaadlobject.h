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

#include "genericexportedobject.h"

#include <QVariant>

namespace taste3 {

namespace aadl {
    class AADLObject;
}

namespace templating {

class ExportedAADLObject : public GenericExportedObject
{
    Q_GADGET
    Q_PROPERTY(QVariantList properties READ attributes)
    Q_PROPERTY(QVariantList attributes READ properties)

public:
    explicit ExportedAADLObject(aadl::AADLObject *aadlObject = nullptr);

    static QVariant createFrom(aadl::AADLObject *aadlObject);

    QVariantList attributes() const;
    QVariantList properties() const;

protected:
    static QVariantList generateProperties(const QHash<QString, QVariant> &props);
};

} // ns templating
} // ns taste3

DECLARE_EXPORTED_TYPE(ExportedAADLObject)
