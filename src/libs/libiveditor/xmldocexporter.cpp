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

#include "xmldocexporter.h"

#include "ivobject.h"
#include "ivcomment.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "ivfunctiontype.h"
#include "ivinterface.h"
#include "common.h"
#include "interface/interfacedocument.h"
#include "stringtemplate.h"
#include "templateeditor.h"
#include "templating/exportableivobject.h"

#include <QBuffer>
#include <QDebug>
#include <QFileDialog>
#include <QMetaEnum>
#include <QObject>
#include <QScopedPointer>
#include <QStandardPaths>

namespace ive {

/*!
 * \class ive::XmlDocExporter
 * \brief The helper that incorporates templating::StringTemplate-related stuff.
 */

const char *TemplateFileExtension = "tmplt";

QString XmlDocExporter::templatesPath()
{
    return QString("%1/export_templates").arg(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
}

QString XmlDocExporter::interfaceDefaultTemplate()
{
    return QString("%1/aadl_xml/interfaceview.%2").arg(templatesPath(), TemplateFileExtension);
}

void XmlDocExporter::ensureDefaultTemplatesDeployed_interface(RolloutDefaultsPolicy policy)
{
    const QStringList fileNames { QStringLiteral("interfaceview"), QStringLiteral("interface"),
        QStringLiteral("function"), QStringLiteral("connection"), QStringLiteral("connectiongroup"),
        QStringLiteral("comment") };

    const QString sourceFile(":/defaults/templating/xml_templates/%1.%2");
    const QString targetFilePath = QFileInfo(interfaceDefaultTemplate()).path();

    const shared::FileCopyingMode kopyMode =
            policy == RolloutDefaultsPolicy::Overwrite ? shared::Overwrite : shared::Keep;

    auto rollOutDefaultTemplate = [kopyMode, sourceFile, targetFilePath](const QString &fileName) {
        const QString fileFrom(sourceFile.arg(fileName, TemplateFileExtension));
        const QString fileTo(QString("%1/%2.%3").arg(targetFilePath, fileName, TemplateFileExtension));
        shared::copyResourceFile(fileFrom, fileTo, kopyMode);
    };

    for (int i = 0; i < fileNames.size(); ++i) {
        const QString &fileName(fileNames[i]);
        rollOutDefaultTemplate(fileName);
        if (i)
            rollOutDefaultTemplate(fileName + QStringLiteral("s"));
    }
}

XmlDocExporter::XmlDocExporter() { }

bool XmlDocExporter::exportDocSilently(InterfaceDocument *doc, const QString &outPath, const QString &templatePath)
{
    return exportDoc(doc, nullptr, outPath, templatePath, InteractionPolicy::Silently);
}

bool XmlDocExporter::exportDocInteractive(
        InterfaceDocument *doc, QWidget *root, const QString &outPath, const QString &templatePath)
{
    return exportDoc(doc, root, outPath, templatePath, InteractionPolicy::Interactive);
}

/**
   @brief XmlDocExporter::exportDoc writes the document as xml to the given buffer
   @param doc the IV(AADL) document
   @param outBuffer the buffer that is open and ready to be written to
   @param templatePath the grantlee template to use for the export. If empty, the default one is used
   @return true when the export was successful.
 */
bool XmlDocExporter::exportDoc(InterfaceDocument *doc, QBuffer *outBuffer, const QString &templatePath)
{
    if (!doc || !outBuffer || !outBuffer->isWritable()) {
        return false;
    }

    ensureDefaultTemplatesDeployed_interface();
    QString usedTemplatePath(templatePath);
    if (templatePath.isEmpty()) {
        usedTemplatePath = XmlDocExporter::interfaceDefaultTemplate();
    }

    const QHash<QString, QVariant> ivObjects = collectInterfaceObjects(doc);
    QScopedPointer<templating::StringTemplate> strTemplate(templating::StringTemplate::create());
    return strTemplate->parseFile(ivObjects, usedTemplatePath, outBuffer);
}
/**
   @brief XmlDocExporter::exportObjects writes the document as xml to the given buffer
   @param doc the set of IV(AADL) entities
   @param outBuffer the buffer that is open and ready to be written to
   @param templatePath the grantlee template to use for the export. If empty, the default one is used
   @return true when the export was successful.
 */
bool XmlDocExporter::exportObjects(
        const QList<ivm::IVObject *> &objects, QBuffer *outBuffer, const QString &templatePath)
{
    if (objects.isEmpty() || !outBuffer || !outBuffer->isWritable()) {
        return false;
    }

    ensureDefaultTemplatesDeployed_interface();
    QString usedTemplatePath(templatePath);
    if (templatePath.isEmpty()) {
        usedTemplatePath = XmlDocExporter::interfaceDefaultTemplate();
    }

    const QHash<QString, QVariant> ivObjects = collectInterfaceObjects(objects);
    QScopedPointer<templating::StringTemplate> strTemplate(templating::StringTemplate::create());
    return strTemplate->parseFile(ivObjects, usedTemplatePath, outBuffer);
}

bool XmlDocExporter::exportDoc(InterfaceDocument *doc, QWidget *root, const QString &outPath,
        const QString &templatePath, InteractionPolicy interaction)
{
    if (!doc)
        return false;

    ensureDefaultTemplatesDeployed_interface();

    QString usedTemplate(templatePath);
    if (interaction == InteractionPolicy::Interactive) {
        if (usedTemplate.isEmpty())
            usedTemplate = QFileDialog::getOpenFileName(root, QObject::tr("Select a template for Interface doc"),
                    QFileInfo(interfaceDefaultTemplate()).path(), QString("*.%1").arg(TemplateFileExtension));
        if (usedTemplate.isEmpty())
            return false;
    }

    return exportDocInterface(doc, root, outPath, usedTemplate, interaction);
}

bool XmlDocExporter::exportDocInterface(InterfaceDocument *doc, QWidget *parentWindow, const QString &outPath,
        const QString &templatePath, InteractionPolicy interaction)
{
    if (!doc)
        return false;

    QString savePath(outPath);
    if (savePath.isEmpty())
        savePath = doc->path();

    if (savePath.isEmpty()) {
        QFileDialog dialog(parentWindow, QObject::tr("Export Interface to an XML file"), doc->path(),
                doc->supportedFileExtensions());
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setDefaultSuffix(".xml");
        if (dialog.exec() == QDialog::Accepted) {
            savePath = dialog.selectedUrls().value(0).toLocalFile();
        }
    }

    if (savePath.isEmpty())
        return false;

    QString usedTemplatePath(templatePath);
    if (templatePath.isEmpty())
        usedTemplatePath = XmlDocExporter::interfaceDefaultTemplate();

    QHash<QString, QVariant> ivObjects = collectInterfaceObjects(doc);

    if (InteractionPolicy::Silently == interaction)
        return runExportSilently(doc, ivObjects, usedTemplatePath, savePath);
    else
        return showExportDialog(doc, parentWindow, ivObjects, usedTemplatePath, savePath);
}

bool XmlDocExporter::runExportSilently(InterfaceDocument *doc, const QHash<QString, QVariant> &content,
        const QString &templateFileName, const QString &outFileName)
{
    QScopedPointer<templating::StringTemplate> strTemplate(templating::StringTemplate::create());
    QFile out(outFileName);
    const bool saved = strTemplate->parseFile(content, templateFileName, &out);
    doc->onSavedExternally(outFileName, saved);

    return saved;
}

bool XmlDocExporter::showExportDialog(InterfaceDocument *doc, QWidget *parentWindow,
        const QHash<QString, QVariant> &content, const QString &templateFileName, const QString &outFileName)
{
    templating::TemplateEditor *previewDialog = new templating::TemplateEditor(outFileName, parentWindow);
    previewDialog->setAttribute(Qt::WA_DeleteOnClose);

    const bool templateParsed = previewDialog->parseTemplate(content, templateFileName);
    if (doc && templateParsed) {
        QObject::connect(
                previewDialog, &templating::TemplateEditor::fileSaved, doc, &InterfaceDocument::onSavedExternally);
    }
    return templateParsed;
}

QHash<QString, QVariant> XmlDocExporter::collectInterfaceObjects(InterfaceDocument *doc)
{
    QHash<QString, QVariant> grouppedObjects = collectInterfaceObjects(doc->objects().values());
    // Add meta-data
    if (!doc->asn1FileName().isEmpty()) {
        grouppedObjects["Asn1FileName"] = QVariant::fromValue(doc->asn1FileName());
    }
    if (!doc->mscFileName().isEmpty()) {
        grouppedObjects["MscFileName"] = QVariant::fromValue(doc->mscFileName());
    }

    return grouppedObjects;
}

QHash<QString, QVariant> XmlDocExporter::collectInterfaceObjects(const QList<ivm::IVObject *> &objects)
{
    QHash<QString, QVariant> grouppedObjects;

    for (const auto ivObject : objects) {
        const ivm::IVObject::Type t = ivObject->type();
        switch (t) {
        case ivm::IVObject::Type::InterfaceGroup:
            continue;
        case ivm::IVObject::Type::FunctionType:
        case ivm::IVObject::Type::Function:
        case ivm::IVObject::Type::Comment:
        case ivm::IVObject::Type::ConnectionGroup:
        case ivm::IVObject::Type::Connection:
        case ivm::IVObject::Type::Unknown: {
            if (t == ivm::IVObject::Type::Unknown || (ivObject->isNested() && objects.size() > 1))
                continue;
            break;
        }
        default:
            break;
        }
        const QVariant &exportedObject = ExportableIVObject::createFrom(ivObject);
        const auto &o = exportedObject.value<ExportableIVObject>();
        const QString groupName = o.groupName();
        if (!grouppedObjects.contains(groupName)) {
            grouppedObjects[groupName] = QVariant::fromValue(QList<QVariant>());
        }
        QVariantList objects = grouppedObjects[o.groupName()].toList();
        objects << exportedObject;
        grouppedObjects[o.groupName()] = objects;
    }
    return grouppedObjects;
}

}
