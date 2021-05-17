/*
  Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivutils.h"

#include "asn1/definitions.h"
#include "asn1/file.h"

#include <QDir>
#include <QStandardPaths>

namespace ive {

QString dynamicPropertiesFilePath()
{
    static const QString kDefaultPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("default_attributes.xml");

    return qEnvironmentVariable("TASTE_DEFAULT_ATTRIBUTES_PATH", kDefaultPath);
}

QString componentsLibraryPath()
{
    static const QString kDefaultPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("components_library") + QDir::separator();

    return qEnvironmentVariable("TASTE_COMPONENTS_LIBRARY", kDefaultPath);
}

QString sharedTypesPath()
{
    static const QString kDefaultPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
            + QDir::separator() + QLatin1String("shared_types") + QDir::separator();

    return qEnvironmentVariable("TASTE_SHARED_TYPES", kDefaultPath);
}

QStringList asn1Names(const Asn1Acn::File *dataTypes)
{
    QStringList names;
    if (dataTypes) {
        for (const std::unique_ptr<Asn1Acn::Definitions> &definitions : dataTypes->definitionsList()) {
            for (const std::unique_ptr<Asn1Acn::TypeAssignment> &assignment : definitions->types()) {
                names.append(assignment->name());
            }
        }
    }
    return names;
}

} // namespace ive
