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

#include "ivexporter.h"

#include "archetypes/archetypeintegrityhelper.h"
#include "archetypes/archetypemodel.h"
#include "common.h"
#include "errorhub.h"
#include "interfacedocument.h"
#include "ivobject.h"
#include "templating/exportableivobject.h"

#include <QBuffer>
#include <QDebug>
#include <QFileDialog>
#include <QMetaEnum>
#include <QObject>
#include <QScopedPointer>
#include <QtDebug>

namespace ive {

/*!
 * \class ive::XmlDocExporter
 * \brief The helper that incorporates templating::StringTemplate-related stuff.
 */

QString IVExporter::defaultTemplatePath() const
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
bool IVExporter::exportObjects(const QList<shared::VEObject *> &objects, QBuffer *outBuffer,
        ivm::ArchetypeModel *archetypesModel, const QString &templatePath)
{
    checkArchetypeIntegrity(objects, archetypesModel);
    const QHash<QString, QVariant> ivObjects = collectObjects(objects);
    return exportData(ivObjects, templatePath, outBuffer);
}

bool IVExporter::exportDocSilently(InterfaceDocument *doc, const QString &outPath, const QString &templatePath)
{
    checkArchetypeIntegrity(doc->objects().values(), doc->archetypesModel());
    const QHash<QString, QVariant> ivObjects = collectInterfaceObjects(doc);
    return exportData(ivObjects, outPath.isEmpty() ? doc->path() : outPath, templatePath, InteractionPolicy::Silently);
}

bool IVExporter::exportDocInteractively(
        InterfaceDocument *doc, const QString &outPath, const QString &templatePath, QWidget *root)
{
    checkArchetypeIntegrity(doc->objects().values(), doc->archetypesModel());
    const QHash<QString, QVariant> ivObjects = collectInterfaceObjects(doc);
    return exportData(
            ivObjects, outPath.isEmpty() ? doc->path() : outPath, templatePath, InteractionPolicy::Interactive, root);
}

IVExporter::IVExporter(QObject *parent)
    : templating::ObjectsExporter(parent)
{
    ensureDefaultTemplatesDeployed(QLatin1String(":/defaults/templating/xml_templates"));
}

QVariant IVExporter::createFrom(const shared::VEObject *object) const
{
    return ExportableIVObject::createFrom(object->as<const ivm::IVObject *>());
}

QString IVExporter::groupName(const shared::VEObject *object) const
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

QHash<QString, QVariant> IVExporter::collectInterfaceObjects(InterfaceDocument *doc)
{
    QHash<QString, QVariant> grouppedObjects = collectObjects(doc->objects().values());
    QHash<QString, QVariant> grouppedLayers = collectObjects(doc->layersObjects().values());

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    grouppedObjects.unite(grouppedLayers);
#else
    grouppedObjects.insert(grouppedLayers);
#endif

    // Add meta-data
    for (const QString &asnFileName : doc->asn1FilesNames()) {
        grouppedObjects["Asn1FileName"] = QVariant::fromValue(asnFileName);
    }
    if (!doc->mscFileName().isEmpty()) {
        grouppedObjects["MscFileName"] = QVariant::fromValue(doc->mscFileName());
    }

    return grouppedObjects;
}

void IVExporter::checkArchetypeIntegrity(QList<shared::VEObject *> ivObjects, ivm::ArchetypeModel *archetypesModel)
{
    if (archetypesModel == nullptr) {
        return;
    }

    QStringList warningList = ivm::ArchetypeIntegrityHelper::checkArchetypeIntegrity(ivObjects, archetypesModel);
    for (QString warning : warningList) {
        shared::ErrorHub::addError(shared::ErrorItem::Warning, warning);
    }
}
}
