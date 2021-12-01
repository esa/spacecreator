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

#include "iveditordocument.h"

#include "commandsstack.h"
#include "errorhub.h"
#include "interfacedocument.h"
#include "ivexporter.h"
#include "ivmainwidget.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorproject.h"
#include "spacecreatorprojectimpl.h"
#include "spacecreatorprojectmanager.h"

#include <QFileInfo>
#include <QUndoStack>
#include <fileutils.h>
#include <id.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>
#include <qtcreatorutils.h>

using namespace Utils;
using namespace Core;

namespace spctr {

IVEditorDocument::IVEditorDocument(SpaceCreatorProjectManager *projectManager, QObject *parent)
    : Core::IDocument(parent)
    , m_projectManager(projectManager)
{
    setMimeType(QLatin1String(spctr::Constants::IV_MIMETYPE));
    setId(Id(spctr::Constants::K_IV_EDITOR_ID));
}

#if QTC_VERSION < 500
Core::IDocument::OpenResult IVEditorDocument::open(
        QString *errorString, const QString &fileName, const QString &realFileName)
{
    const QFileInfo fi(fileName);
#else
Core::IDocument::OpenResult IVEditorDocument::open(
        QString *errorString, const Utils::FilePath &fileName, const Utils::FilePath &realFileName)
{
    const QFileInfo fi = fileName.toFileInfo();
#endif
    Q_UNUSED(errorString)
    Q_UNUSED(realFileName)

    if (fileName.isEmpty() || !m_projectManager) {
        return OpenResult::ReadError;
    }

    const QString absfileName = fi.absoluteFilePath();

    SpaceCreatorProjectImpl *project = m_projectManager->project(absfileName);
    scs::SpaceCreatorProject *storage = project ? project : m_projectManager->orphanStorage();
    m_plugin = storage->ivData(absfileName);
    if (m_plugin.isNull()) {
        return OpenResult::ReadError;
    }

    connect(m_plugin->commandsStack(), &ive::cmd::CommandsStack::asn1FilesImported, this,
            [project](const QStringList &asn1FilesAdded) {
                project->project()->rootProjectNode()->addFiles(toProjectsFiles(asn1FilesAdded));
            });
    connect(m_plugin->commandsStack(), &ive::cmd::CommandsStack::asn1FileRemoved, this,
            [project](const QStringList &asn1FilesRemoved) {
                project->project()->rootProjectNode()->removeFiles(toProjectsFiles(asn1FilesRemoved));
            });

#if QTC_VERSION < 409
    setFilePath(Utils::FileName::fromString(absfileName));
#else
    setFilePath(Utils::FilePath::fromString(absfileName));
#endif

    connect(m_plugin->undoStack(), &QUndoStack::cleanChanged, this, [this](bool) { Q_EMIT changed(); });
    Q_EMIT ivDataLoaded(absfileName, m_plugin);
    m_plugin->centerOnView();

    return OpenResult::Success;
}

#if QTC_VERSION < 500
bool IVEditorDocument::save(QString *errorString, const QString &name, bool autoSave)
{
#else
bool IVEditorDocument::save(QString *errorString, const Utils::FilePath &name, bool autoSave)
{
#endif
    Q_UNUSED(errorString)
    if (m_plugin.isNull()) {
        return false;
    }
#if QTC_VERSION < 500
#if QTC_VERSION < 409
    const FileName newName = Utils::FileName::fromString(name);
#else
    const FilePath newName = Utils::FilePath::fromString(name);
#endif
#else
    const FilePath newName = name;
#endif

    const auto oldFileName = filePath();
    const auto actualName = name.isEmpty() ? oldFileName : newName;
    if (actualName.isEmpty()) {
        return false;
    }
    bool dirty = isModified();

    shared::ErrorHub::clearFileErrors(actualName.toString());

    ive::InterfaceDocument *ivDocument = m_plugin->document();
    ivDocument->setPath(actualName.toString());
    if (!ivDocument->exporter()->exportDocSilently(m_plugin->document(), actualName.toString())) {
        ivDocument->setPath(oldFileName.toString());
        return false;
    }

    if (autoSave) {
        ivDocument->setPath(oldFileName.toString());
        return true;
    } else {
        ivDocument->undoStack()->setClean();
    }

    setFilePath(actualName);

    if (dirty != isModified()) {
        Q_EMIT changed();
    }

    return true;
}

#if QTC_VERSION < 409
void IVEditorDocument::setFilePath(const FileName &newName)
{
    IDocument::setFilePath(newName);
}
#else
void IVEditorDocument::setFilePath(const FilePath &newName)
{
    IDocument::setFilePath(newName);
}
#endif

bool IVEditorDocument::shouldAutoSave() const
{
    return false;
}

bool IVEditorDocument::isSaveAsAllowed() const
{
    return true;
}

bool IVEditorDocument::isModified() const
{
    return m_plugin && m_plugin->document()->isDirty();
}

bool IVEditorDocument::reload(QString *errorString, ReloadFlag flag, ChangeType type)
{
    if (flag == FlagIgnore) {
        return true;
    }
    checkPermissions();
    Q_EMIT aboutToReload();
    Q_EMIT reloadRequested(errorString, filePath().toString());
    bool success = false;
    if (m_plugin) {
        success = m_plugin->document()->load(filePath().toString());
    }
    Q_EMIT reloadFinished(success);
    return success;
}

IVEditorCorePtr IVEditorDocument::ivEditorCore() const
{
    return m_plugin;
}

}
