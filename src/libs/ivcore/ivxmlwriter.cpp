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

#include "ivxmlwriter.h"

#include "archetypes/archetypeintegrityhelper.h"
#include "ivobject.h"
#include "ivpropertytemplateconfig.h"
#include "templating/exportableivobject.h"

#include <QDebug>
#include <QIODevice>
#include <QSaveFile>
#include <shared/errorhub.h>

namespace ivm {

IVXMLWriter::IVXMLWriter(QObject *parent)
    : templating::ObjectsExporter(parent)
    , m_uiExporter(new templating::UIExporter(ivm::IVPropertyTemplateConfig::instance(), this))
{
    ensureDefaultTemplatesDeployed(QLatin1String(":/defaults/templating/xml_templates"));
    ensureDefaultTemplatesDeployed(QLatin1String(":/xml_templates"));
}

QString IVXMLWriter::templatePath(const QString &templateName)
{
    return QString("%1/aadl_xml/%2.%3").arg(templatesPath(), templateName, templateFileExtension());
}

QString IVXMLWriter::defaultTemplatePath() const
{
    return QString("%1/aadl_xml/interfaceview.%2").arg(templatesPath(), templateFileExtension());
}

/**
   @brief XmlDocExporter::exportObjects writes the document as xml to the given buffer
   @param objects the set of IV(AADL) entities
   @param outBuffer the buffer that is open and ready to be written to
   @param archetypesModel model of archetypes for archetypes check, if null then no check
   @param templatePath the grantlee template to use for the export. If empty, the default one is used
   @return true when the export was successful.
 */
bool IVXMLWriter::exportObjects(const QList<IVObject *> &objects, QIODevice *outBuffer,
        ivm::ArchetypeModel *archetypesModel, const QString &pathToTemplate)
{
    checkArchetypeIntegrity(objects, archetypesModel);

    QHash<QString, QVariant> ivObjects = collectObjects(objects);
    const QHash<QString, QVariant> uiObjects = m_uiExporter->collectObjects(objects);
    ivObjects.insert(uiObjects);
    return exportData(ivObjects, pathToTemplate, outBuffer);
}

bool IVXMLWriter::exportObjectsSilently(const QList<IVObject *> &objects, const QString &outPath,
        ivm::ArchetypeModel *archetypesModel, const QString &templatePath)
{
    if (outPath.isEmpty()) {
        return false;
    }

    QString usedTemplate(templatePath);
    if (usedTemplate.isEmpty()) {
        usedTemplate = defaultTemplatePath();
    }

    QSaveFile saveFile(outPath);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Can't open device for writing:" << saveFile.errorString();
        shared::ErrorHub::addError(shared::ErrorItem::Error, saveFile.errorString());
        return false;
    }
    if (exportObjects(objects, &saveFile, archetypesModel, usedTemplate)) {
        saveFile.commit();
        Q_EMIT exported(outPath, true);
        return true;
    }
    return false;
}

QVariant IVXMLWriter::createFrom(const shared::VEObject *object) const
{
    return ive::ExportableIVObject::createFrom(object->as<const ivm::IVObject *>());
}

QString IVXMLWriter::groupName(const shared::VEObject *object) const
{
    if (auto ivObject = object->as<const ivm::IVObject *>()) {
        switch (ivObject->type()) {
        case ivm::IVObject::Type::InterfaceGroup:
            return {};
        case ivm::IVObject::Type::Function:
        case ivm::IVObject::Type::FunctionType:
            return QStringLiteral("Functions");
        case ivm::IVObject::Type::RequiredInterface:
        case ivm::IVObject::Type::ProvidedInterface:
            return QStringLiteral("Interfaces");
        case ivm::IVObject::Type::Comment:
            return QStringLiteral("Comments");
        case ivm::IVObject::Type::Connection:
            return QStringLiteral("Connections");
        case ivm::IVObject::Type::ConnectionGroup:
            return QStringLiteral("ConnectionGroups");
        case ivm::IVObject::Type::ConnectionLayer:
            return QStringLiteral("ConnectionLayers");
        case ivm::IVObject::Type::ArchetypeLibraryReference:
            return QStringLiteral("ArchetypeLibraryReferences");
        default:
            break;
        }
    }
    return QString();
}

void IVXMLWriter::checkArchetypeIntegrity(const QList<IVObject *> &ivObjects, ivm::ArchetypeModel *archetypesModel)
{
    if (archetypesModel == nullptr) {
        return;
    }

    const QStringList warningList = ivm::ArchetypeIntegrityHelper::checkArchetypeIntegrity(ivObjects, archetypesModel);
    for (const QString &warning : qAsConst(warningList)) {
        shared::ErrorHub::addError(shared::ErrorItem::Warning, warning);
    }
}
}
