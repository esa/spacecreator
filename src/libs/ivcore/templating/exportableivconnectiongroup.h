/*
   Copyright (C) 2020 European Space Agency - <maxime.perrotin@esa.int>

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

#include "exportableivobject.h"

namespace ivm {
class IVConnectionGroup;
}

namespace ive {

/**
 * @brief The ExportableIVConnectionGroup is a class to export IVConnectionGroup
 */
class ExportableIVConnectionGroup : public ExportableIVObject
{
    Q_GADGET
    Q_PROPERTY(QString name READ connectionGroupName)
    Q_PROPERTY(QString source READ sourceName)
    Q_PROPERTY(QString target READ targetName)
    Q_PROPERTY(QVariantMap sifaces READ sourceInterfaces)
    Q_PROPERTY(QVariantMap tifaces READ targetInterfaces)
    Q_PROPERTY(QVariantList connections READ connections)

public:
    ExportableIVConnectionGroup(const ivm::IVConnectionGroup *connection = nullptr);

    QString connectionGroupName() const;
    QString sourceName() const;
    QString targetName() const;
    QVariantMap sourceInterfaces() const;
    QVariantMap targetInterfaces() const;
    QVariantList connections() const;
};

} // namespace ive

Q_DECLARE_METATYPE(ive::ExportableIVConnectionGroup)
Q_DECLARE_TYPEINFO(ive::ExportableIVConnectionGroup, Q_MOVABLE_TYPE);
