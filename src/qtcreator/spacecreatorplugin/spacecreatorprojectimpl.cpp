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

#include "spacecreatorprojectimpl.h"

#include "asn1modelstorage.h"
#include "editorcore.h"
#include "errorhub.h"
#include "errormessageparser.h"
#include "iveditorcore.h"
#include "msceditorcore.h"
#include "mscsystemchecks.h"

#include <QDebug>
#include <algorithm>
#include <editormanager/documentmodel.h>
#include <projectexplorer/project.h>
#include <projectexplorer/taskhub.h>

namespace spctr {

const char TASK_CATEGORY_SPACE_CREATOR[] = "Task.Category.SpaceCreator";

SpaceCreatorProjectImpl::SpaceCreatorProjectImpl(ProjectExplorer::Project *project, QObject *parent)
    : scs::SpaceCreatorProject(parent)
    , m_project(project)
{
    Q_ASSERT(m_project);

    connect(this, &scs::SpaceCreatorProject::editedExternally, this, &spctr::SpaceCreatorProjectImpl::saveIfNotOpen);

    connect(m_project, &ProjectExplorer::Project::fileListChanged, this,
            &spctr::SpaceCreatorProjectImpl::purgeNonProjectData);

    m_asnFiles = allAsn1Files();
    connect(m_project, &ProjectExplorer::Project::fileListChanged, this,
            &spctr::SpaceCreatorProjectImpl::checkAsnFileRename);
    for (const QString &file : qAsConst(m_asnFiles)) {
        m_asn1Storage->watchFile(file);
    }

    static bool hubInitialized = false;
    if (!hubInitialized) {
        ProjectExplorer::TaskHub::instance()->addCategory(TASK_CATEGORY_SPACE_CREATOR, tr("ASN error"));
        hubInitialized = true;
    }

    connect(shared::ErrorHub::instance(), &shared::ErrorHub::errorAdded, this, &SpaceCreatorProjectImpl::reportError);
    connect(shared::ErrorHub::instance(), &shared::ErrorHub::cleared, this, &SpaceCreatorProjectImpl::clearAllErrors);
    connect(shared::ErrorHub::instance(), &shared::ErrorHub::clearedFile, this,
            &SpaceCreatorProjectImpl::clearTasksForFile);
}

SpaceCreatorProjectImpl::~SpaceCreatorProjectImpl() { }

/*!
   Returns all files of the current project endig with the given \p suffix
 */
QStringList SpaceCreatorProjectImpl::projectFiles(const QString &suffix) const
{
    QStringList result;
#if QTC_VERSION == 582
    for (const Utils::FilePath &fileName : m_project->files(ProjectExplorer::Project::AllFiles)) {
        if (fileName.toString().endsWith(suffix, Qt::CaseInsensitive)) {
            result.append(fileName.toString());
        }
    }
#elif QTC_VERSION == 48
    for (const Utils::FileName &fileName : m_project->files(ProjectExplorer::Project::AllFiles)) {
        if (fileName.toString().endsWith(suffix, Qt::CaseInsensitive)) {
            result.append(fileName.toString());
        }
    }
#endif
    return result;
}

/*!
   Checks if one asn1 file was renamed. If yes, update the filename in all msc and iv files.
 */
void SpaceCreatorProjectImpl::checkAsnFileRename()
{
    QStringList asnFiles = allAsn1Files();

    for (const QString &file : qAsConst(asnFiles)) {
        m_asn1Storage->watchFile(file);
    }

    QStringList newAsnFiles;
    for (const QString &file : qAsConst(asnFiles)) {
        if (!m_asnFiles.contains(file)) {
            newAsnFiles.append(file);
        }
    }
    QStringList lostAsnFiles;
    for (const QString &file : qAsConst(m_asnFiles)) {
        if (!asnFiles.contains(file)) {
            lostAsnFiles.append(file);
        }
    }

    if (newAsnFiles.size() == 1 && lostAsnFiles.size() == 1) {
        for (QSharedPointer<msc::MSCEditorCore> &mscCore : allMscCores()) {
            mscCore->renameAsnFile(lostAsnFiles[0], newAsnFiles[0]);
        }
        ivCore()->renameAsnFile(lostAsnFiles[0], newAsnFiles[0]);
    }

    m_asnFiles = asnFiles;
}

/*!
   Saves the data of the given \p core in case it is not open in one of the QtCretaor editors
 */
void SpaceCreatorProjectImpl::saveIfNotOpen(shared::EditorCore *core)
{
    if (!isOpenInEditor(core)) {
        core->save();
    }
}

void SpaceCreatorProjectImpl::reportError(const shared::ErrorItem &error)
{
    ProjectExplorer::Task::TaskType type =
            error.m_type == shared::ErrorItem::Warning ? ProjectExplorer::Task::Warning : ProjectExplorer::Task::Error;
#if QTC_VERSION == 582
    ProjectExplorer::Task task(type, error.m_description, Utils::FilePath::fromString(error.m_fileName), error.m_line,
            TASK_CATEGORY_SPACE_CREATOR);
#elif QTC_VERSION == 48
    ProjectExplorer::Task task(type, error.m_description, Utils::FileName::fromString(error.m_fileName), error.m_line,
            TASK_CATEGORY_SPACE_CREATOR);
#endif
    m_errors.append(task);
    ProjectExplorer::TaskHub::instance()->addTask(task);
    ProjectExplorer::TaskHub::requestPopup();
}

void SpaceCreatorProjectImpl::clearAllErrors()
{
    ProjectExplorer::TaskHub::instance()->clearTasks(TASK_CATEGORY_SPACE_CREATOR);
}

void SpaceCreatorProjectImpl::clearTasksForFile(const QString &fileName)
{
    auto it = std::remove_if(m_errors.begin(), m_errors.end(), [fileName](const ProjectExplorer::Task &task) {
        const bool same = task.file.toString() == fileName;
        if (same) {
            ProjectExplorer::TaskHub::instance()->removeTask(task);
        }
        return same;
    });
    m_errors.erase(it, m_errors.end());
}

/*!
   Returns true, if the file of the given /p core is open in an editor
 */
bool SpaceCreatorProjectImpl::isOpenInEditor(shared::EditorCore *core) const
{
    QList<Core::IDocument *> openDocuments = Core::DocumentModel::openedDocuments();
    for (Core::IDocument *doc : qAsConst(openDocuments)) {
        if (doc->filePath().toString() == core->filePath()) {
            return true;
        }
    }
    return false;
}

} // namespace spctr
