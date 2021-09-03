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

#pragma once

#include "dveditorcore.h"
#include "iveditorcore.h"

#include <QList>
#include <QObject>
#include <QSharedPointer>
#include <memory>

namespace ProjectExplorer {
class Project;
}

namespace msc {
class MSCEditorCore;
}
namespace shared {
class EditorCore;
}
namespace scs {
class SpaceCreatorProject;
}

namespace spctr {
class SpaceCreatorProjectImpl;

/*!
   Manages all SpaceCreatorProject that are uses to store and ananlyse space creator data per project.
   It can be used to query for the project responsible for a specific file of EditorCore
 */
class SpaceCreatorProjectManager : public QObject
{
    Q_OBJECT
public:
    explicit SpaceCreatorProjectManager(QObject *parent = nullptr);
    ~SpaceCreatorProjectManager();

    DVEditorCorePtr dvData(const QString &fileName) const;
    IVEditorCorePtr ivData(const QString &fileName) const;
    QSharedPointer<msc::MSCEditorCore> mscData(const QString &fileName) const;

    SpaceCreatorProjectImpl *project(const QString &fileName) const;
    SpaceCreatorProjectImpl *project(const QSharedPointer<shared::EditorCore> &core) const;

    scs::SpaceCreatorProject *orphanStorage() const;

private Q_SLOTS:
    void addProject(ProjectExplorer::Project *project);
    void removeProject(ProjectExplorer::Project *project);

private:
    QList<SpaceCreatorProjectImpl *> m_projects;
    std::unique_ptr<scs::SpaceCreatorProject> m_orphanStorage;
};

} // namespace spctr
