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
#include "dvmainwidget.h"
#include "dvmodel.h"
#include "errorhub.h"
#include "spacecreatorpluginconstants.h"
#include "spacecreatorproject.h"
#include "spacecreatorprojectimpl.h"
#include "spacecreatorprojectmanager.h"

#include <QFileInfo>
#include <QUndoStack>
#include <coreplugin/id.h>
#include <fileutils.h>

using namespace Utils;

namespace spctr {

DVEditorDocument::DVEditorDocument(SpaceCreatorProjectManager *projectManager, QObject *parent)
    : Core::IDocument(parent)
    , m_projectManager(projectManager)
{
    setMimeType(QLatin1String(spctr::Constants::DV_MIMETYPE));
    setId(Core::Id(spctr::Constants::K_DV_EDITOR_ID));
}

Core::IDocument::OpenResult DVEditorDocument::open(
        QString *errorString, const QString &fileName, const QString &realFileName)
{
    Q_UNUSED(errorString)
    Q_UNUSED(realFileName)

    if (fileName.isEmpty() || !m_projectManager) {
        return OpenResult::ReadError;
    }

    const QFileInfo fi(fileName);
    const QString absfileName = fi.absoluteFilePath();

    SpaceCreatorProjectImpl *project = m_projectManager->project(absfileName);
    scs::SpaceCreatorProject *storage = project ? project : m_projectManager->orphanStorage();
    m_plugin = storage->dvData(absfileName);
    if (m_plugin.isNull()) {
        return OpenResult::ReadError;
    }

    setFilePath(Utils::FileName::fromString(absfileName));

    connect(m_plugin->undoStack(), &QUndoStack::cleanChanged, this, [this](bool) { Q_EMIT changed(); });
    Q_EMIT dvDataLoaded(absfileName, m_plugin);

    return OpenResult::Success;
}

bool DVEditorDocument::save(QString *errorString, const QString &name, bool autoSave)
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

    shared::ErrorHub::clearFileErrors(actualName.toString());

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
    if (type == TypePermissions) {
        Q_EMIT changed();
    } else {
        Q_EMIT aboutToReload();
        Q_EMIT reloadRequested(errorString, filePath().toString());
        bool success = false;
        if (m_plugin) {
            success = m_plugin->appModel()->load(filePath().toString());
        }
        Q_EMIT reloadFinished(success);
        return success;
    }

    return true;
}

QSharedPointer<dve::DVEditorCore> DVEditorDocument::dvEditorCore() const
{
    return m_plugin;
}

void DVEditorDocument::setFilePath(const FileName &newName)
{
    IDocument::setFilePath(newName);
}

}
