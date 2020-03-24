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

#include "exportableaadlobject.h"

namespace taste3 {

namespace aadl {
    class AADLObjectConnection;
}

namespace templating {

/**
 * @brief The ExportableAADLConnection is a class to export AADLObjectConnection
 */
class ExportableAADLConnection : public ExportableAADLObject
{
    Q_GADGET
    Q_PROPERTY(QString source READ sourceName)
    Q_PROPERTY(QString target READ targetName)
    Q_PROPERTY(QString siName READ sourceInterfaceName)
    Q_PROPERTY(QString tiName READ targetInterfaceName)
    Q_PROPERTY(bool siIsRequired READ sourceInterfaceIsRequired)
    Q_PROPERTY(bool siIsProvided READ sourceInterfaceIsProvided)
    Q_PROPERTY(bool tiIsRequired READ targetInterfaceIsRequired)
    Q_PROPERTY(bool tiIsProvided READ targetInterfaceIsProvided)

public:
    explicit ExportableAADLConnection(const aadl::AADLObjectConnection *connection = nullptr);

    QString sourceName() const;
    QString targetName() const;
    QString sourceInterfaceName() const;
    QString targetInterfaceName() const;
    bool sourceInterfaceIsRequired() const;
    bool sourceInterfaceIsProvided() const;
    bool targetInterfaceIsRequired() const;
    bool targetInterfaceIsProvided() const;
};

} // ns templating
} // ns taste3

DECLARE_EXPORTED_TYPE(ExportableAADLConnection)
