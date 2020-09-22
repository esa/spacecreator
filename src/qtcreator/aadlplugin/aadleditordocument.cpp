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

#include "aadleditordocument.h"

#include "aadlmainwidget.h"
#include "aadlpluginconstants.h"

#include <QFileInfo>
#include <QTextCodec>
#include <QTextDocument>
#include <fileutils.h>

using namespace Utils;

namespace spctr {

AadlEditorDocument::AadlEditorDocument(AadlMainWidget *designWidget, QObject *parent)
    : m_designWidget(designWidget)
{
    setMimeType(QLatin1String(spctr::Constants::AADL_MIMETYPE));
    setParent(parent);
    setId(Core::Id(spctr::Constants::K_AADL_EDITOR_ID));

    // Designer needs UTF-8 regardless of settings.
    setCodec(QTextCodec::codecForName("UTF-8"));
    connect(m_designWidget.data(), &AadlMainWidget::dirtyChanged, this, [this] { Q_EMIT changed(); });
}

Core::IDocument::OpenResult AadlEditorDocument::open(
        QString *errorString, const QString &fileName, const QString &realFileName)
{
    Q_UNUSED(errorString)
    Q_UNUSED(realFileName)

    if (fileName.isEmpty())
        return OpenResult::ReadError;

    if (!m_designWidget)
        return OpenResult::ReadError;

    const QFileInfo fi(fileName);
    const QString absfileName = fi.absoluteFilePath();
    if (!m_designWidget->load(absfileName)) {
        return OpenResult::CannotHandle;
    }

    setFilePath(Utils::FileName::fromString(absfileName));

    return OpenResult::Success;
}

bool AadlEditorDocument::save(QString *errorString, const QString &name, bool autoSave)
{
    Q_UNUSED(errorString)
    const FileName oldFileName = filePath();
    const FileName actualName = name.isEmpty() ? oldFileName : FileName::fromString(name);
    if (actualName.isEmpty())
        return false;
    bool dirty = m_designWidget->isDirty();

    m_designWidget->setFileName(actualName.toString());
    if (!m_designWidget->save()) {
        m_designWidget->setFileName(oldFileName.toString());
        return false;
    }

    if (autoSave) {
        m_designWidget->setFileName(oldFileName.toString());
        m_designWidget->save();
        return true;
    }

    setFilePath(actualName);

    if (dirty != m_designWidget->isDirty())
        Q_EMIT changed();

    return true;
}

void AadlEditorDocument::setFilePath(const FileName &newName)
{
    m_designWidget->setFileName(newName.toString());
    IDocument::setFilePath(newName);
}

bool AadlEditorDocument::shouldAutoSave() const
{
    return false;
}

bool AadlEditorDocument::isSaveAsAllowed() const
{
    return true;
}

AadlMainWidget *AadlEditorDocument::designWidget() const
{
    return m_designWidget;
}

bool AadlEditorDocument::isModified() const
{
    return m_designWidget && m_designWidget->isDirty();
}

bool AadlEditorDocument::reload(QString *errorString, ReloadFlag flag, ChangeType type)
{
    if (flag == FlagIgnore) {
        return true;
    }
    if (type == TypePermissions) {
        Q_EMIT changed();
    } else {
        Q_EMIT aboutToReload();
        Q_EMIT reloadRequested(errorString, filePath().toString());
        bool success = true;
        if (errorString != nullptr)
            success = errorString->isEmpty();
        Q_EMIT reloadFinished(success);
        return success;
    }

    return true;
}

QString AadlEditorDocument::designWidgetContents() const
{
    return m_designWidget->textContents();
}

void AadlEditorDocument::syncXmlFromDesignWidget()
{
    document()->setPlainText(designWidgetContents());
}

}
