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

#include "dvexporter.h"

#include "dvobject.h"
#include "dvpropertytemplateconfig.h"
#include "errorhub.h"
#include "scversion.h"
#include "templating/exportabledvobject.h"
#include "uiexporter.h"

#include <QBuffer>
#include <QFileDialog>
#include <QSaveFile>

namespace dve {

QString DVExporter::defaultTemplatePath() const
{
    return QString("%1/aadl_xml/deploymentview.%2").arg(templatesPath(), templateFileExtension());
}

QString DVExporter::templatePath(const QString &templateName)
{
    return QString("%1/aadl_xml/%2.%3").arg(templatesPath(), templateName, templateFileExtension());
}

bool DVExporter::exportObjects(
        const QList<shared::VEObject *> &objects, QIODevice *outBuffer, const QString &templatePath)
{
    const QHash<QString, QVariant> dvObjects = collectObjects(objects);
    return exportData(dvObjects, templatePath, outBuffer);
}

bool DVExporter::exportObjectsInteractively(const QList<shared::VEObject *> &objects, const QString &creatorGitHash,
        const QString &outPath, const QString &templatePath, const QString &requirementsURL, QWidget *root)
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

    QHash<QString, QVariant> dvObjects = collectDvObjects(objects, creatorGitHash, requirementsURL);
    dvObjects.insert(m_uiExporter->collectObjects(objects));

    return showExportDialog(dvObjects, usedTemplate, savePath, root);
}

bool DVExporter::exportObjectsSilently(const QList<shared::VEObject *> &objects, const QString &creatorGitHash,
        const QString &outPath, const QString &pathToTemplate, const QString &requirementsURL, const QString &uiFile)
{
    if (outPath.isEmpty()) {
        return false;
    }

    QString usedTemplate(pathToTemplate);
    if (usedTemplate.isEmpty()) {
        usedTemplate = defaultTemplatePath();
    }

    QSaveFile saveFile(outPath);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "Can't open device for writing:" << saveFile.errorString();
        shared::ErrorHub::addError(shared::ErrorItem::Error, saveFile.errorString());
        return false;
    }

    QHash<QString, QVariant> dvObjects = collectDvObjects(objects, creatorGitHash, requirementsURL);
    const QHash<QString, QVariant> uiObjects = m_uiExporter->collectObjects(objects);
    if (uiFile.isEmpty()) {
        dvObjects.insert(uiObjects);
    } else {
        dvObjects.insert(QLatin1String("UiFile"), QVariant::fromValue(uiFile));
    }
    bool result = exportData(dvObjects, usedTemplate, &saveFile);
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
    return true;
}

DVExporter::DVExporter(QObject *parent)
    : templating::ObjectsExporter(parent)
    , m_uiExporter(new templating::UIExporter(dvm::DVPropertyTemplateConfig::instance(), this))
{
    ensureDefaultTemplatesDeployed(QLatin1String(":/defaults/templating/xml_templates"));
}

QVariant DVExporter::createFrom(const shared::VEObject *object) const
{
    return ExportableDVObject::createFrom(object->as<const dvm::DVObject *>());
}

QString DVExporter::groupName(const shared::VEObject *object) const
{
    if (auto dvObject = object->as<const dvm::DVObject *>()) {
        switch (dvObject->type()) {
        case dvm::DVObject::Type::Node:
            return QStringLiteral("Nodes");
        case dvm::DVObject::Type::Connection:
            return QStringLiteral("Connections");
        case dvm::DVObject::Type::Bus:
            return QStringLiteral("Buses");
        default:
            return {};
        }
    }
    return {};
}

/*!
 * \brief DVExporter::collectDvObjects Returns all data objects and meta data
 * \note Does not include UI objects
 */
QHash<QString, QVariant> DVExporter::collectDvObjects(
        const QList<shared::VEObject *> &objects, const QString &creatorGitHash, const QString &requirementsURL) const
{
    QHash<QString, QVariant> dvObjects = collectObjects(objects);
    dvObjects.insert(QLatin1String("creatorHash"),
            QVariant::fromValue(creatorGitHash.isEmpty() ? spaceCreatorGitHash : creatorGitHash));
    dvObjects.insert(QLatin1String("modifierHash"), QVariant::fromValue(spaceCreatorGitHash));
    if (!requirementsURL.isEmpty()) {
        dvObjects[QLatin1String("requirementsURL")] = QVariant::fromValue(requirementsURL);
    }
    return dvObjects;
}

} // namespace dve
