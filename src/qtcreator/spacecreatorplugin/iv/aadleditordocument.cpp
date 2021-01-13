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
#include "aadlmodelstorage.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"
#include "spacecreatorpluginconstants.h"
#include "xmldocexporter.h"

#include <QFileInfo>
#include <QUndoStack>
#include <coreplugin/id.h>
#include <fileutils.h>

using namespace Utils;

namespace spctr {

AadlEditorDocument::AadlEditorDocument(AadlModelStorage *aadlStorage, QObject *parent)
    : Core::IDocument(parent)
    , m_aadlStorage(aadlStorage)
{
    setMimeType(QLatin1String(spctr::Constants::AADL_MIMETYPE));
    setId(Core::Id(spctr::Constants::K_AADL_EDITOR_ID));
}

Core::IDocument::OpenResult AadlEditorDocument::open(
        QString *errorString, const QString &fileName, const QString &realFileName)
{
    Q_UNUSED(errorString)
    Q_UNUSED(realFileName)

    if (fileName.isEmpty() || !m_aadlStorage) {
        return OpenResult::ReadError;
    }

    const QFileInfo fi(fileName);
    const QString absfileName = fi.absoluteFilePath();
    m_plugin = m_aadlStorage->ivData(absfileName);
    if (m_plugin.isNull()) {
        return OpenResult::ReadError;
    }

    setFilePath(Utils::FileName::fromString(absfileName));

    connect(m_plugin->undoStack(), &QUndoStack::cleanChanged, this, [this](bool) { Q_EMIT changed(); });
    Q_EMIT ivDataLoaded(absfileName, m_plugin);

    return OpenResult::Success;
}

bool AadlEditorDocument::save(QString *errorString, const QString &name, bool autoSave)
{
    Q_UNUSED(errorString)
    if (m_plugin.isNull()) {
        return false;
    }

    const FileName oldFileName = filePath();
    const FileName actualName = name.isEmpty() ? oldFileName : FileName::fromString(name);
    if (actualName.isEmpty()) {
        return false;
    }
    bool dirty = isModified();

    aadlinterface::InterfaceDocument *ivDocument = m_plugin->document();
    ivDocument->setPath(actualName.toString());
    if (!aadlinterface::XmlDocExporter::exportDocSilently(m_plugin->document(), actualName.toString(), {})) {
        ivDocument->setPath(oldFileName.toString());
        return false;
    }

    if (autoSave) {
        ivDocument->setPath(oldFileName.toString());
        aadlinterface::XmlDocExporter::exportDocSilently(m_plugin->document(), actualName.toString(), {});
        return true;
    }

    setFilePath(actualName);

    if (dirty != isModified()) {
        Q_EMIT changed();
    }

    return true;
}

bool AadlEditorDocument::shouldAutoSave() const
{
    return false;
}

bool AadlEditorDocument::isSaveAsAllowed() const
{
    return true;
}

bool AadlEditorDocument::isModified() const
{
    return m_plugin && m_plugin->document()->isDirty();
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
        if (errorString != nullptr) {
            success = errorString->isEmpty();
        }
        Q_EMIT reloadFinished(success);
        return success;
    }

    return true;
}

QSharedPointer<aadlinterface::IVEditorCore> AadlEditorDocument::ivEditorCore() const
{
    return m_plugin;
}

void AadlEditorDocument::setFilePath(const FileName &newName)
{
    IDocument::setFilePath(newName);
}

}
