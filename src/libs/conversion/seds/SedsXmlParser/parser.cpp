/** @file
 * This file is part of the SpaceCreator.
 *
 * @copyright (C) 2021 N7 Space Sp. z o.o.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.
 */

#include "parser.h"

#include "exceptions.h"
#include "specialized/componentsparser.h"
#include "specialized/coreparser.h"
#include "specialized/datatypesparser.h"
#include "specialized/devicesparser.h"
#include "specialized/interfacesparser.h"

#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <converter/import/exceptions.h>
#include <converter/model.h>
#include <seds/SedsModel/datasheet.h>
#include <seds/SedsModel/package/package.h>
#include <seds/SedsModel/packagefile.h>
#include <seds/SedsModel/sedsmodel.h>

using converter::import::FileNotFound;
using std::placeholders::_1;

namespace seds::parser {

std::unique_ptr<converter::Model> Parser::parse(const QString &inputSedsFilename)
{
    const QFileInfo inputSedsFileInfo(inputSedsFilename);
    if (!inputSedsFileInfo.exists()) {
        throw FileNotFound(inputSedsFilename, "File doesn't exist");
    }

    QFile inputSedsFile(inputSedsFilename);
    if (!inputSedsFile.open(QIODevice::ReadOnly)) {
        throw ParserException(inputSedsFile.errorString());
    }

    auto xmlReader = QXmlStreamReader(&inputSedsFile);

    xmlReader.readNextStartElement();

    if (xmlReader.name() == "PackageFile") {
        return std::make_unique<model::SedsModel>(readPackageFile(xmlReader));
    } else if (xmlReader.name() == "DataSheet") {
        return std::make_unique<model::SedsModel>(readDataSheet(xmlReader));
    } else {
        throw UnhandledElement(xmlReader.name(), "Root");
    }
}

model::PackageFile Parser::readPackageFile(QXmlStreamReader &xmlReader)
{
    model::PackageFile packageFile;

    for (const auto &attribute : xmlReader.attributes()) {
        if (attribute.name() == QStringLiteral("schemaLocation")) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Package")) {
            packageFile.setPackage(readPackage(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "PackageFile");
        }
    }

    return packageFile;
}

model::DataSheet Parser::readDataSheet(QXmlStreamReader &xmlReader)
{
    model::DataSheet dataSheet;

    for (const auto &attribute : xmlReader.attributes()) {
        if (attribute.name() == QStringLiteral("schemaLocation")) {
            continue;
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (xmlReader.name() == QStringLiteral("Device")) {
            dataSheet.setDevice(DevicesParser::readDevice(xmlReader));
        } else if (xmlReader.name() == QStringLiteral("Package")) {
            dataSheet.addPackage(readPackage(xmlReader));
        } else {
            throw UnhandledElement(xmlReader.name(), "DataSheet");
        }
    }

    return dataSheet;
}

model::Package Parser::readPackage(QXmlStreamReader &xmlReader)
{
    model::Package package;

    for (const auto &attribute : xmlReader.attributes()) {
        if (CoreParser::processForDescription(&package, attribute)) {
            continue;
        } else if (attribute.name() == QStringLiteral("name")) {
            package.setName(attribute.value().toString());
        } else {
            throw UnhandledAttribute(attribute.name(), xmlReader.name());
        }
    }

    while (xmlReader.readNextStartElement()) {
        if (CoreParser::processForDescription(&package, xmlReader)) {
            continue;
        } else if (xmlReader.name() == QStringLiteral("DataTypeSet")) {
            DataTypesParser::readDataTypeSet(std::bind(&model::Package::addDataType, &package, _1), xmlReader);
        } else if (xmlReader.name() == QStringLiteral("DeclaredInterfaceSet")) {
            InterfacesParser::readDeclaredInterfaceSet(
                    std::bind(&model::Package::addInterfaceDeclaration, &package, _1), xmlReader);
        } else if (xmlReader.name() == QStringLiteral("ComponentSet")) {
            ComponentsParser::readComponentSet(package, xmlReader);
        } else {
            throw UnhandledElement(xmlReader.name(), "Package");
        }
    }

    return package;
}

} // namespace seds::parser
