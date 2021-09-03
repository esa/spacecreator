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

#include "spacecreatorprojectmanager.h"

#include "spacecreatorprojectimpl.h"

#include <projectexplorer/project.h>
#include <projectexplorer/session.h>

namespace spctr {

SpaceCreatorProjectManager::SpaceCreatorProjectManager(QObject *parent)
    : QObject(parent)
    , m_orphanStorage(new scs::SpaceCreatorProject(this))
{
    connect(ProjectExplorer::SessionManager::instance(), &ProjectExplorer::SessionManager::projectAdded, this,
            &spctr::SpaceCreatorProjectManager::addProject);
    connect(ProjectExplorer::SessionManager::instance(), &ProjectExplorer::SessionManager::aboutToRemoveProject, this,
            &spctr::SpaceCreatorProjectManager::removeProject);
}

SpaceCreatorProjectManager::~SpaceCreatorProjectManager()
{
    qDeleteAll(m_projects);
    m_projects.clear();
}

/*!
   Returns tha editor core that represents the file fileName.
   In case the core does not exist yet, it is created in the correct SpaceCreatorProject
 */
DVEditorCorePtr SpaceCreatorProjectManager::dvData(const QString &fileName) const
{
    if (SpaceCreatorProjectImpl *data = project(fileName)) {
        return data->dvData(fileName);
    } else {
        return m_orphanStorage->dvData(fileName);
    }
}

/*!
   Returns tha editor core that represents the file fileName.
   In case the core does not exist yet, it is created in the correct SpaceCreatorProject
 */
IVEditorCorePtr SpaceCreatorProjectManager::ivData(const QString &fileName) const
{
    if (SpaceCreatorProjectImpl *data = project(fileName)) {
        return data->ivData(fileName);
    } else {
        return m_orphanStorage->ivData(fileName);
    }
}

/*!
   Returns tha editor core that represents the file fileName.
   In case the core does not exist yet, it is created in the correct SpaceCreatorProject
 */
QSharedPointer<msc::MSCEditorCore> SpaceCreatorProjectManager::mscData(const QString &fileName) const
{
    if (SpaceCreatorProjectImpl *data = project(fileName)) {
        return data->mscData(fileName);
    } else {
        return m_orphanStorage->mscData(fileName);
    }
}

/*!
   Returns the project, that contains the given \p fileName
 */
SpaceCreatorProjectImpl *SpaceCreatorProjectManager::project(const QString &fileName) const
{
    for (SpaceCreatorProjectImpl *project : m_projects) {
#if QTC_VERSION == 415
        if (project->project()->isKnownFile(Utils::FilePath::fromString(fileName))) {
            return project;
        }
#elif QTC_VERSION == 48
        if (project->project()->isKnownFile(Utils::FileName::fromString(fileName))) {
            return project;
        }
#endif
    }

    // Fallback needed during project startup, when the files are not all populated in the project
    // Or when a new file is not yet added in the project (parsed)
    for (SpaceCreatorProjectImpl *project : m_projects) {
        if (fileName.startsWith(project->project()->projectDirectory().toString())) {
            return project;
        }
    }

    return nullptr;
}

/*!
   Returns the project, that contains the given \p editorCore
 */
SpaceCreatorProjectImpl *SpaceCreatorProjectManager::project(const QSharedPointer<shared::EditorCore> &core) const
{
    for (SpaceCreatorProjectImpl *project : m_projects) {
        if (project->contains(core)) {
            return project;
        }
    }

    return nullptr;
}

/*!
   Retruns the project that is used to store all data/files that do not belong to one of the open projects
 */
scs::SpaceCreatorProject *SpaceCreatorProjectManager::orphanStorage() const
{
    return m_orphanStorage.get();
}

/*!
   Adds a new SpaceCreatorProject that represents the given QtCreator \p project
 */
void SpaceCreatorProjectManager::addProject(ProjectExplorer::Project *project)
{
    m_projects.append(new SpaceCreatorProjectImpl(project, this));
}

/*!
   Removes a new SpaceCreatorProject that represents the given QtCreator \p project
 */
void SpaceCreatorProjectManager::removeProject(ProjectExplorer::Project *project)
{
    for (SpaceCreatorProjectImpl *data : qAsConst(m_projects)) {
        if (data->project() == project) {
            m_projects.removeAll(data);
            delete data;
            return;
        }
    }
}

} // namespace spctr
