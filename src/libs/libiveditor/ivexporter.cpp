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

#include "interfacedocument.h"
#include "scversion.h"
#include "templating/exportableivobject.h"

#include <QBuffer>
#include <QDebug>
#include <QFileDialog>
#include <QMetaEnum>
#include <QObject>
#include <QSaveFile>
#include <QScopedPointer>
#include <ivcore/archetypes/archetypeintegrityhelper.h>
#include <ivcore/archetypes/archetypemodel.h>
#include <ivcore/ivobject.h>
#include <ivcore/ivpropertytemplateconfig.h>
#include <shared/common.h>
#include <shared/errorhub.h>
#include <templating/uiexporter.h>

namespace ive {

/*!
 * \class ive::XmlDocExporter
 * \brief The helper that incorporates templating::StringTemplate-related stuff.
 */

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
    : ivm::IVXMLWriter(parent)
{
}

QHash<QString, QVariant> IVExporter::collectInterfaceObjects(InterfaceDocument *doc)
{
    QHash<QString, QVariant> grouppedObjects = collectObjects(doc->objects().values());
    QHash<QString, QVariant> grouppedLayers = collectObjects(doc->layersObjects().values());
    grouppedObjects.insert(grouppedLayers);

    // Add meta-data
    if (!doc->projectName().isEmpty()) {
        grouppedObjects[QLatin1String("ProjectName")] = QVariant::fromValue(doc->projectName());
    }
    for (const QString &asnFileName : doc->asn1FilesNames()) {
        grouppedObjects[QLatin1String("Asn1FileName")] = QVariant::fromValue(asnFileName);
    }
    if (!doc->mscFileName().isEmpty()) {
        grouppedObjects[QLatin1String("MscFileName")] = QVariant::fromValue(doc->mscFileName());
    }
    const QString uiFilePath = doc->uiFileName();
    if (!uiFilePath.isEmpty()) {
        grouppedObjects[QLatin1String("UiFile")] = QVariant::fromValue(uiFilePath);
    }
    const QString requestsURL= doc->requestsURL();
    if (!requestsURL.isEmpty()) {
        grouppedObjects[QLatin1String("requestsURL")] = QVariant::fromValue(requestsURL);
    }

    const QString creatorGitHash = doc->creatorGitHash();
    grouppedObjects[QLatin1String("creatorHash")] = QVariant::fromValue(creatorGitHash);
    grouppedObjects[QLatin1String("modifierHash")] = QVariant::fromValue(spaceCreatorGitHash);

    return grouppedObjects;
}

void IVExporter::checkArchetypeIntegrity(
        const QList<shared::VEObject *> &ivObjects, ivm::ArchetypeModel *archetypesModel)
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
