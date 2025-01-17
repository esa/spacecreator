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

#include "dvobject.h"

namespace dvm {
class DVPartition;

class DVFunction : public DVObject
{
    Q_OBJECT
    Q_PROPERTY(QString usedImplementation READ usedImplementation NOTIFY usedImplementationChanged)
    Q_PROPERTY(QString implementation READ implementation WRITE setImplementation NOTIFY implementationChanged)
    Q_PROPERTY(QStringList path READ path NOTIFY pathChanged)

public:
    explicit DVFunction(DVObject *parent = nullptr, const shared::Id &id = shared::InvalidId);
    DVPartition *partition() const;

    QString usedImplementation() const;
    void setImplementation(const QString &name);
    QString implementation() const;
    QStringList availableImplementations() const;
    QString defaultImplementation() const;

    QStringList path() const;

    void updateForExport() override;

Q_SIGNALS:
    void usedImplementationChanged();
    void implementationChanged();
    void pathChanged();
};

} // namespace dvm

Q_DECLARE_METATYPE(dvm::DVFunction *)
