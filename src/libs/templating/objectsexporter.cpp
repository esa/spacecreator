/*
   Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "objectsexporter.h"

#include "diskutils.h"
#include "standardpaths.h"
#include "stringtemplate.h"
#include "templateeditor.h"

#include <QFileDialog>
#include <QIODevice>
#include <QPointer>
#include <errorhub.h>

namespace templating {

const char *kTemplateFileExtension = "tmplt";

ObjectsExporter::ObjectsExporter(QObject *parent)
    : QObject(parent)
{
}

QString ObjectsExporter::templatesPath()
{
    return QString("%1/export_templates")
            .arg(shared::StandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
}

QString ObjectsExporter::templateFileExtension()
{
    return QString::fromLatin1(kTemplateFileExtension);
}

void ObjectsExporter::ensureDefaultTemplatesDeployed(
        const QString &defaultsPath, ObjectsExporter::RolloutDefaultsPolicy policy)
{
    const QString targetFilePath = QFileInfo(defaultTemplatePath()).path();
    const shared::FileCopyingMode copyMode =
            policy == RolloutDefaultsPolicy::Overwrite ? shared::Overwrite : shared::Keep;
    shared::copyDir(defaultsPath, targetFilePath, copyMode);
}

bool ObjectsExporter::showExportDialog(const QHash<QString, QVariant> &content, const QString &templateFileName,
        const QString &outFileName, QWidget *parentWindow)
{
    static QPointer<templating::TemplateEditor> previewDialog;
    if (!previewDialog) {
        previewDialog = new templating::TemplateEditor(outFileName, parentWindow);
        previewDialog->setAttribute(Qt::WA_DeleteOnClose);
        connect(previewDialog, &templating::TemplateEditor::fileSaved, this, &ObjectsExporter::exported);
    }

    return previewDialog->parseTemplate(content, templateFileName);
}

bool ObjectsExporter::exportData(
        const QHash<QString, QVariant> &data, const QString &templatePath, QIODevice *outDevice)
{
    if (!outDevice) {
        return false;
    }

    QString usedTemplatePath(templatePath);
    if (templatePath.isEmpty()) {
        usedTemplatePath = defaultTemplatePath();
    }

    QScopedPointer<templating::StringTemplate> strTemplate(templating::StringTemplate::create());
    return strTemplate->parseFile(data, usedTemplatePath, outDevice);
}

} // namespace templating
