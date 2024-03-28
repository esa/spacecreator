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
#include "iveditorcore.h"
#include "ivrefactorhandler.h"
#include "msceditorcore.h"
#include "refactor/adarefactor.h"
#include "refactor/cpprefactor.h"
#include "refactor/crefactor.h"
#include "refactor/pythonrefactor.h"
#include "refactor/sdlrefactor.h"

#include <QDebug>
#include <QDir>
#include <algorithm>
#include <editormanager/documentmodel.h>
#include <projectexplorer/project.h>
#include <projectexplorer/taskhub.h>

namespace spctr {

const char TASK_CATEGORY_SPACE_CREATOR[] = "Task.Category.SpaceCreator";

SpaceCreatorProjectImpl::SpaceCreatorProjectImpl(ProjectExplorer::Project *project, QObject *parent)
    : scs::SpaceCreatorProject(parent)
    , m_project(project)
    , m_adaRefactor(std::make_unique<AdaRefactor>())
    , m_cRefactor(std::make_unique<CRefactor>())
    , m_cppRefactor(std::make_unique<CppRefactor>())
    , m_pythonRefactor(std::make_unique<PythonRefactor>())
    , m_sdlRefactor(std::make_unique<SdlRefactor>())
{
    Q_ASSERT(m_project);

    connect(this, &scs::SpaceCreatorProject::editedExternally, this, &spctr::SpaceCreatorProjectImpl::saveIfNotOpen);

    connect(m_project, &ProjectExplorer::Project::fileListChanged, this,
            &spctr::SpaceCreatorProjectImpl::purgeNonProjectData);

    m_asnFiles = allAsn1Files();
    connect(m_project, &ProjectExplorer::Project::fileListChanged, this,
            &spctr::SpaceCreatorProjectImpl::checkAsnFileRename);
    m_asn1Storage->watchFiles(m_asnFiles);

    static bool hubInitialized = false;
    if (!hubInitialized) {
#if QTC_VERSION >= 1200
        const ProjectExplorer::TaskCategory category { TASK_CATEGORY_SPACE_CREATOR, tr("ASN error"), tr("") };
        ProjectExplorer::TaskHub::instance()->addCategory(category);
#else
        ProjectExplorer::TaskHub::instance()->addCategory(TASK_CATEGORY_SPACE_CREATOR, tr("ASN error"));
#endif
        hubInitialized = true;
    }

    connect(shared::ErrorHub::instance(), &shared::ErrorHub::errorAdded, this, &SpaceCreatorProjectImpl::reportError);
    connect(shared::ErrorHub::instance(), &shared::ErrorHub::cleared, this, &SpaceCreatorProjectImpl::clearAllErrors);
    connect(shared::ErrorHub::instance(), &shared::ErrorHub::clearedFile, this,
            &SpaceCreatorProjectImpl::clearTasksForFile);

    m_adaRefactor->setStorage(this);
    m_ivRefactorHandler->registerRefactor(m_adaRefactor.get());
    m_cRefactor->setStorage(this);
    m_ivRefactorHandler->registerRefactor(m_cRefactor.get());
    m_cppRefactor->setStorage(this);
    m_ivRefactorHandler->registerRefactor(m_cppRefactor.get());
    m_pythonRefactor->setStorage(this);
    m_ivRefactorHandler->registerRefactor(m_pythonRefactor.get());
    m_sdlRefactor->setStorage(this);
    m_ivRefactorHandler->registerRefactor(m_sdlRefactor.get());
}

SpaceCreatorProjectImpl::~SpaceCreatorProjectImpl() { }

/*!
   Returns all files of the current project endig with the given \p suffix
 */
QStringList SpaceCreatorProjectImpl::projectFiles(const QString &suffix) const
{
    QStringList result;
    for (const Utils::FilePath &fileName : m_project->files(ProjectExplorer::Project::AllFiles)) {
        if (fileName.toString().endsWith(suffix, Qt::CaseInsensitive)) {
            result.append(fileName.toString());
        }
    }
    // fallback (if project wasn't fully loaded yet
    if (result.isEmpty()) {
        QDir dir(m_project->rootProjectDirectory().toString());
        for (const QString &entry : dir.entryList({ QString("*%1").arg(suffix) })) {
            result.append(dir.filePath(entry));
        }
    }
    return result;
}

/*!
   Checks if one asn1 file was renamed. If yes, update the filename in all msc and iv files.
 */
void SpaceCreatorProjectImpl::checkAsnFileRename()
{
    QStringList asnFiles = allAsn1Files();

    m_asn1Storage->watchFiles(asnFiles);

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
        for (MSCEditorCorePtr &mscCore : allMscCores()) {
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
    ProjectExplorer::Task task(type, error.m_description, Utils::FilePath::fromString(error.m_fileName), error.m_line,
            TASK_CATEGORY_SPACE_CREATOR);
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
