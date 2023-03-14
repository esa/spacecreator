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
#include "ivpropertytemplateconfig.h"
#include "templating/exportableivobject.h"
#include "uiexporter.h"

#include <QBuffer>
#include <QDebug>
#include <QFileDialog>
#include <QMetaEnum>
#include <QObject>
#include <QSaveFile>
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

QString IVExporter::templatePath(const QString &templateName)
{
    return QString("%1/aadl_xml/%2.%3").arg(templatesPath(), templateName, templateFileExtension());
}

/**
   @brief XmlDocExporter::exportObjects writes the document as xml to the given buffer
   @param objects the set of IV(AADL) entities
   @param outBuffer the buffer that is open and ready to be written to
   @param archetypesModel model of archetypes for archetypes check, if null then no check
   @param templatePath the grantlee template to use for the export. If empty, the default one is used
   @return true when the export was successful.
 */
bool IVExporter::exportObjects(const QList<shared::VEObject *> &objects, QIODevice *outBuffer,
        ivm::ArchetypeModel *archetypesModel, const QString &pathToTemplate)
{
    checkArchetypeIntegrity(objects, archetypesModel);

    QHash<QString, QVariant> ivObjects = collectObjects(objects);
    const QHash<QString, QVariant> uiObjects = m_uiExporter->collectObjects(objects);
    ivObjects.insert(uiObjects);
    return exportData(ivObjects, pathToTemplate, outBuffer);
}

bool IVExporter::exportObjectsSilently(const QList<shared::VEObject *> &objects, const QString &outPath,
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

bool IVExporter::exportDocSilently(InterfaceDocument *doc, const QString &outPath, const QString &templatePath)
{
    if (!doc || outPath.isEmpty()) {
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

    const QList<shared::VEObject *> objects = doc->objects().values();

    checkArchetypeIntegrity(objects, doc->archetypesModel());
    QHash<QString, QVariant> ivObjects = collectInterfaceObjects(doc);
    const QHash<QString, QVariant> uiObjects = m_uiExporter->collectObjects(objects);
    const QString uiFile = doc->uiFileName();
    if (uiFile.isEmpty()) {
        ivObjects.insert(uiObjects);
    }
    bool result = exportData(ivObjects, usedTemplate, &saveFile);
    if (!uiFile.isEmpty()) {
        saveFile.commit();
        saveFile.setFileName(QFileInfo(outPath).absolutePath() + QDir::separator() + uiFile);
        if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            qWarning() << "Can't open device for writing:" << saveFile.errorString();
            shared::ErrorHub::addError(shared::ErrorItem::Error, saveFile.errorString());
            return false;
        }
        result |= m_uiExporter->exportData(uiObjects, m_uiExporter->defaultTemplatePath(), &saveFile);
    }
    result |= saveFile.commit();

    Q_EMIT exported(outPath, result);
    return result;
}

bool IVExporter::exportDocInteractively(
        InterfaceDocument *doc, const QString &outPath, const QString &templatePath, QWidget *root)
{
    QString usedTemplate(templatePath);
    if (usedTemplate.isEmpty()) {
        usedTemplate = QFileDialog::getOpenFileName(root, QObject::tr("Select a template"),
                QFileInfo(defaultTemplatePath()).path(), QString("*.%1").arg(templateFileExtension()));
        if (usedTemplate.isEmpty())
            return false;
    }

    QString savePath(outPath);
    if (savePath.isEmpty()) {
        QFileDialog dialog(root, QObject::tr("Export data to an XML file"));
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setDefaultSuffix(".xml");
        if (dialog.exec() == QDialog::Accepted) {
            savePath = dialog.selectedUrls().value(0).toLocalFile();
        }
    }

    checkArchetypeIntegrity(doc->objects().values(), doc->archetypesModel());

    QHash<QString, QVariant> ivObjects = collectInterfaceObjects(doc);
    ivObjects.insert(m_uiExporter->collectObjects(doc->objects().values()));

    return showExportDialog(ivObjects, usedTemplate, savePath, root);
}

IVExporter::IVExporter(QObject *parent)
    : templating::ObjectsExporter(parent)
    , m_uiExporter(new templating::UIExporter(ivm::IVPropertyTemplateConfig::instance(), this))
{
    ensureDefaultTemplatesDeployed(QLatin1String(":/defaults/templating/xml_templates"));
    ensureDefaultTemplatesDeployed(QLatin1String(":/xml_templates"));
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
    grouppedObjects.insert(grouppedLayers);

    // Add meta-data
    for (const QString &asnFileName : doc->asn1FilesNames()) {
        grouppedObjects["Asn1FileName"] = QVariant::fromValue(asnFileName);
    }
    if (!doc->mscFileName().isEmpty()) {
        grouppedObjects["MscFileName"] = QVariant::fromValue(doc->mscFileName());
    }
    const QString uiFilePath = doc->uiFileName();
    if (!uiFilePath.isEmpty()) {
        grouppedObjects["UiFile"] = QVariant::fromValue(uiFilePath);
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
