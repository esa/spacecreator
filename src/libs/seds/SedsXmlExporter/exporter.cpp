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

#include "exporter.h"

#include "specialized/componentexporter.h"
#include "specialized/datatypeexporter.h"

#include <QDomDocument>
#include <conversion/common/export/exceptions.h>
#include <conversion/common/overloaded.h>
#include <seds/SedsModel/sedsmodel.h>
#include <seds/SedsOptions/options.h>

using conversion::Model;
using conversion::ModelType;
using conversion::Options;
using conversion::exporter::IncorrectModelException;
using conversion::exporter::MissingOutputFilenameException;
using conversion::exporter::NullModelException;
using conversion::seds::SedsOptions;

using namespace seds::model;

namespace seds::exporter {

const QString SedsXmlExporter::m_schemaNsUri = R"(http://www.ccsds.org/schema/sois/seds)";
const QString SedsXmlExporter::m_schemaInstanceNsUri = R"(http://www.w3.org/2001/XMLSchema-instance)";
const QString SedsXmlExporter::m_schemaLocation = R"(http://www.ccsds.org/schema/sois/seds seds.xsd)";

void SedsXmlExporter::exportModel(const Model *const model, const Options &options) const
{
    // Set global hash seed, so that the output is deterministic
    qSetGlobalQHashSeed(0);

    if (model == nullptr) {
        throw NullModelException();
    }

    const auto *const sedsModel = dynamic_cast<const SedsModel *>(model);
    if (sedsModel == nullptr) {
        throw IncorrectModelException(ModelType::Seds, model->modelType());
    }

    // Set global hash seed, so that the output is deterministic
    qSetGlobalQHashSeed(0);

    auto sedsDocument = createSedsXmlDocument();

    // clang-format off
    std::visit(overloaded {
        [&](const PackageFile &packageFile) { exportPackageFile(packageFile, sedsDocument); },
        [&](const DataSheet &dataSheet) { exportDataSheet(dataSheet, sedsDocument); }
    }, sedsModel->data());
    // clang-format on

    const auto outputFilePath = options.value(SedsOptions::outputFilepath);
    if (!outputFilePath) {
        throw MissingOutputFilenameException(ModelType::Seds);
    }

    QSaveFile outputFile(*outputFilePath);
    writeAndCommit(outputFile, sedsDocument.toString());
}

void SedsXmlExporter::exportPackageFile(const PackageFile &packageFile, QDomDocument &sedsDocument)
{
    auto rootElement = createRootElement("PackageFile", sedsDocument);

    exportPackage(packageFile.package(), rootElement, sedsDocument);

    sedsDocument.appendChild(std::move(rootElement));
}

void SedsXmlExporter::exportDataSheet(const DataSheet &dataSheet, QDomDocument &sedsDocument)
{
    auto rootElement = createRootElement("DataSheet", sedsDocument);

    for (const auto &package : dataSheet.packages()) {
        exportPackage(package, rootElement, sedsDocument);
    }

    sedsDocument.appendChild(std::move(rootElement));
}

void SedsXmlExporter::exportPackage(const Package &package, QDomElement &parentElement, QDomDocument &sedsDocument)
{
    auto packageElement = sedsDocument.createElement(QStringLiteral("Package"));

    const auto &packageName = package.nameStr();
    packageElement.setAttribute(QStringLiteral("name"), packageName);

    if (package.dataTypes().size() > 0) {
        auto dataTypeSet = sedsDocument.createElement(QStringLiteral("DataTypeSet"));
        for (const auto &dataType : package.dataTypes()) {
            DataTypeExporter::exportDataType(dataType, dataTypeSet, sedsDocument);
        }
        packageElement.appendChild(std::move(dataTypeSet));
    }

    if (package.components().size() > 0) {
        ComponentExporter::exportComponents(package.components(), packageElement, sedsDocument);
    }

    parentElement.appendChild(std::move(packageElement));
}

QDomDocument SedsXmlExporter::createSedsXmlDocument()
{
    QDomDocument sedsDocument;

    auto processingInstruction = sedsDocument.createProcessingInstruction("xml", R"(version="1.0" encoding="utf-8")");
    sedsDocument.appendChild(std::move(processingInstruction));

    return sedsDocument;
}

QDomElement SedsXmlExporter::createRootElement(QString rootElementName, QDomDocument &sedsDocument)
{
    auto rootElement = sedsDocument.createElement(std::move(rootElementName));
    rootElement.setAttribute("xmlns", m_schemaNsUri);
    rootElement.setAttribute("xmlns:xsi", m_schemaInstanceNsUri);
    rootElement.setAttribute("xsi:schemaLocation", m_schemaLocation);

    return rootElement;
}

} // namespace seds::exporter
