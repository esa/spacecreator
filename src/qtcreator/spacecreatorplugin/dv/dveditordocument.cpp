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

#include "dveditordocument.h"

#include "dvappmodel.h"
#include "dveditorcore.h"
#include "dvsystemqueries.h"
#include "errorhub.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorproject.h"
#include "spacecreatorprojectimpl.h"
#include "spacecreatorprojectmanager.h"

#include <QFileInfo>
#include <QUndoStack>
#include <fileutils.h>
#include <id.h>

using namespace Utils;
using namespace Core;

namespace spctr {

DVEditorDocument::DVEditorDocument(SpaceCreatorProjectManager *projectManager, QObject *parent)
    : Core::IDocument(parent)
    , m_projectManager(projectManager)
{
    setMimeType(QLatin1String(spctr::Constants::DV_MIMETYPE));
    setId(Id(spctr::Constants::K_DV_EDITOR_ID));
}

Core::IDocument::OpenResult DVEditorDocument::open(
        QString *errorString, const Utils::FilePath &fileName, const Utils::FilePath &realFileName)
{
    const QFileInfo fi = fileName.toFileInfo();
    Q_UNUSED(errorString)
    Q_UNUSED(realFileName)

    if (fileName.isEmpty() || !m_projectManager) {
        return OpenResult::ReadError;
    }

    const QString absfileName = fi.absoluteFilePath();

    SpaceCreatorProjectImpl *project = m_projectManager->project(absfileName);
    scs::SpaceCreatorProject *storage = project ? project : m_projectManager->orphanStorage();
    m_plugin = storage->dvData(absfileName);
    if (m_plugin.isNull()) {
        return OpenResult::ReadError;
    }

    setFilePath(Utils::FilePath::fromString(absfileName));
    connect(m_plugin->undoStack(), &QUndoStack::cleanChanged, this, [this](bool) { Q_EMIT changed(); });
    Q_EMIT dvDataLoaded(absfileName, m_plugin);
    m_plugin->centerOnView();

    QMetaObject::invokeMethod(this, "checkForErrors", Qt::QueuedConnection);

    return OpenResult::Success;
}

bool DVEditorDocument::save(QString *errorString, const Utils::FilePath &name, bool autoSave)
{
    Q_UNUSED(errorString)
    if (m_plugin.isNull()) {
        return false;
    }

    const FilePath newName = name;

    const auto oldFileName = filePath();
    const auto actualName = name.isEmpty() ? oldFileName : newName;
    if (actualName.isEmpty()) {
        return false;
    }
    bool dirty = isModified();

    checkForErrors();

    dve::DVAppModel *model = m_plugin->appModel();
    model->setPath(actualName.toString());
    if (!m_plugin->save()) {
        model->setPath(oldFileName.toString());
        return false;
    }

    if (autoSave) {
        model->setPath(oldFileName.toString());
        return true;
    } else {
        m_plugin->undoStack()->setClean();
    }

    setFilePath(actualName);

    if (dirty != isModified()) {
        Q_EMIT changed();
    }

    return true;
}

void DVEditorDocument::setFilePath(const FilePath &newName)
{
    IDocument::setFilePath(newName);
}

bool DVEditorDocument::shouldAutoSave() const
{
    return false;
}

bool DVEditorDocument::isSaveAsAllowed() const
{
    return true;
}

bool DVEditorDocument::isModified() const
{
    return m_plugin && !m_plugin->undoStack()->isClean();
}

bool DVEditorDocument::reload(QString *errorString, ReloadFlag flag, ChangeType type)
{
    if (flag == FlagIgnore) {
        return true;
    }
    checkPermissions();
    Q_EMIT aboutToReload();
    Q_EMIT reloadRequested(errorString, filePath().toString());
    bool success = false;
    if (m_plugin) {
        success = m_plugin->appModel()->load(filePath().toString());
        m_plugin->centerOnView();
    }
    Q_EMIT reloadFinished(success);
    return success;
}

DVEditorCorePtr DVEditorDocument::dvEditorCore() const
{
    return m_plugin;
}

void DVEditorDocument::checkForErrors()
{
    if (!m_projectManager || !m_plugin) {
        return;
    }

    const QString fileName = m_plugin->filePath();
    shared::ErrorHub::clearFileErrors(fileName);
    SpaceCreatorProjectImpl *project = m_projectManager->project(fileName);
    scs::SpaceCreatorProject *storage = project ? project : m_projectManager->orphanStorage();
    storage->dvChecks()->checkDVFile(m_plugin);
}
}
