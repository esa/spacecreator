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

#include "mainmodel.h"
#include "msceditorcore.h"
#include "mscmodel.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorproject.h"
#include "spacecreatorprojectimpl.h"
#include "spacecreatorprojectmanager.h"

#include <QFileInfo>
#include <QUndoStack>
#include <id.h>
#include <utils/fileutils.h>

using namespace Utils;
using namespace Core;

namespace spctr {

MscEditorDocument::MscEditorDocument(SpaceCreatorProjectManager *projectManager, QObject *parent)
    : Core::IDocument(parent)
    , m_projectManager(projectManager)
{
    Q_ASSERT(projectManager);
    setMimeType(QLatin1String(spctr::Constants::MSC_MIMETYPE));
    setId(Id(spctr::Constants::K_MSC_EDITOR_ID));
}

Core::IDocument::OpenResult MscEditorDocument::open(
        QString *errorString, const QString &fileName, const QString &realFileName)
{
    Q_UNUSED(realFileName)

    if (fileName.isEmpty() || !m_projectManager) {
        return OpenResult::ReadError;
    }

    const QFileInfo fi(fileName);
    const QString absfileName = fi.absoluteFilePath();

    SpaceCreatorProjectImpl *project = m_projectManager->project(absfileName);
    scs::SpaceCreatorProject *storage = project ? project : m_projectManager->orphanStorage();
    m_plugin = storage->mscData(absfileName);
    if (m_plugin.isNull()) {
        return OpenResult::ReadError;
    }
    if (errorString) {
        *errorString = m_plugin->mainModel()->mscErrorMessages().join("\n");
    }

#if QTC_VERSION == 48
    setFilePath(Utils::FileName::fromString(absfileName));
#elif QTC_VERSION == 415
    setFilePath(Utils::FilePath::fromString(absfileName));
#endif

    connect(m_plugin->undoStack(), &QUndoStack::cleanChanged, this, [this](bool) { Q_EMIT changed(); });
    Q_EMIT mscDataLoaded(absfileName, m_plugin);

    return OpenResult::Success;
}

bool MscEditorDocument::save(QString *errorString, const QString &name, bool autoSave)
{
    if (m_plugin.isNull()) {
        return false;
    }

#if QTC_VERSION == 48
    const FileName newName = Utils::FileName::fromString(name);
#elif QTC_VERSION == 415
    const FilePath newName = Utils::FilePath::fromString(name);
#endif

    const auto oldFileName = filePath();
    const auto actualName = name.isEmpty() ? oldFileName : newName;
    if (actualName.isEmpty()) {
        return false;
    }
    bool dirty = isModified();

    msc::MainModel *mainModel = m_plugin->mainModel();
    mainModel->setCurrentFilePath(actualName.toString());
    if (!mainModel->saveMsc(mainModel->currentFilePath())) {
        if (errorString != nullptr) {
            *errorString = mainModel->mscErrorMessages().join("\n");
        }
        mainModel->setCurrentFilePath(oldFileName.toString());
        return false;
    }

    if (autoSave) {
        mainModel->setCurrentFilePath(oldFileName.toString());
        mainModel->saveMsc(mainModel->currentFilePath());
        return true;
    }

    setFilePath(actualName);

    if (dirty != isModified()) {
        Q_EMIT changed();
    }

    return true;
}

#if QTC_VERSION == 415
void MscEditorDocument::setFilePath(const FilePath &newName)
{
    if (!m_plugin.isNull()) {
        m_plugin->mainModel()->setCurrentFilePath(newName.toString());
    }
    IDocument::setFilePath(newName);
}

#elif QTC_VERSION == 48
void MscEditorDocument::setFilePath(const FileName &newName)
{
    if (!m_plugin.isNull()) {
        m_plugin->mainModel()->setCurrentFilePath(newName.toString());
    }
    IDocument::setFilePath(newName);
}

#endif

bool MscEditorDocument::shouldAutoSave() const
{
    return false;
}

bool MscEditorDocument::isSaveAsAllowed() const
{
    return true;
}

bool MscEditorDocument::isModified() const
{
    return m_plugin && m_plugin->mainModel()->needSave();
}

bool MscEditorDocument::reload(QString *errorString, ReloadFlag flag, ChangeType type)
{
    if (flag == FlagIgnore) {
        return true;
    }
    checkPermissions();
    Q_EMIT aboutToReload();
    Q_EMIT reloadRequested(errorString, filePath().toString());
    bool success = false;
    if (m_plugin) {
        success = m_plugin->mainModel()->loadFile(filePath().toString());
    }
    Q_EMIT reloadFinished(success);
    return success;
}

QSharedPointer<msc::MSCEditorCore> MscEditorDocument::mscEditorCore() const
{
    return m_plugin;
}

}
