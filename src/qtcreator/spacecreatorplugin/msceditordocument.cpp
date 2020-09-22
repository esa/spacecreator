/*
   Copyright (C) 2018 - 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "msceditordocument.h"

#include "mscmainwidget.h"
#include "spacecreatorpluginconstants.h"

#include <QFileInfo>
#include <QTextCodec>
#include <QTextDocument>
#include <fileutils.h>

using namespace Utils;

namespace spctr {

MscEditorDocument::MscEditorDocument(MscMainWidget *designWidget, QObject *parent)
    : m_designWidget(designWidget)
{
    setMimeType(QLatin1String(spctr::Constants::MSC_MIMETYPE));
    setParent(parent);
    setId(Core::Id(spctr::Constants::K_MSC_EDITOR_ID));

    // Designer needs UTF-8 regardless of settings.
    setCodec(QTextCodec::codecForName("UTF-8"));
    connect(m_designWidget.data(), &MscMainWidget::dirtyChanged, this, [this] { Q_EMIT changed(); });
}

Core::IDocument::OpenResult MscEditorDocument::open(
        QString *errorString, const QString &fileName, const QString &realFileName)
{
    Q_UNUSED(realFileName)

    if (fileName.isEmpty())
        return OpenResult::ReadError;

    if (!m_designWidget)
        return OpenResult::ReadError;

    const QFileInfo fi(fileName);
    const QString absfileName = fi.absoluteFilePath();
    if (!m_designWidget->load(absfileName)) {
        if (errorString != nullptr)
            *errorString = m_designWidget->errorMessage();
        return OpenResult::CannotHandle;
    }

    setFilePath(Utils::FileName::fromString(absfileName));

    return OpenResult::Success;
}

bool MscEditorDocument::save(QString *errorString, const QString &name, bool autoSave)
{
    const FileName oldFileName = filePath();
    const FileName actualName = name.isEmpty() ? oldFileName : FileName::fromString(name);
    if (actualName.isEmpty())
        return false;
    bool dirty = m_designWidget->isDirty();

    m_designWidget->setFileName(actualName.toString());
    if (!m_designWidget->save()) {
        if (errorString != nullptr)
            *errorString = m_designWidget->errorMessage();
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

void MscEditorDocument::setFilePath(const FileName &newName)
{
    m_designWidget->setFileName(newName.toString());
    IDocument::setFilePath(newName);
}

bool MscEditorDocument::shouldAutoSave() const
{
    return false;
}

bool MscEditorDocument::isSaveAsAllowed() const
{
    return true;
}

MscMainWidget *MscEditorDocument::designWidget() const
{
    return m_designWidget;
}

bool MscEditorDocument::isModified() const
{
    return m_designWidget && m_designWidget->isDirty();
}

bool MscEditorDocument::reload(QString *errorString, ReloadFlag flag, ChangeType type)
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

QString MscEditorDocument::designWidgetContents() const
{
    return m_designWidget->textContents();
}

void MscEditorDocument::syncXmlFromDesignWidget()
{
    document()->setPlainText(designWidgetContents());
}
}
