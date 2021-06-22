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

#include "spacecreatorproject.h"

#include <QObject>
#include <QVector>
#include <projectexplorer/task.h>

namespace ProjectExplorer {
class Project;
}
namespace shared {
class EditorCore;
class ErrorItem;
}

namespace spctr {

/*!
   Does contain everything to load/store/validate Space Creator models that belong to once QtCreator project
 */
class SpaceCreatorProjectImpl : public scs::SpaceCreatorProject
{
public:
    explicit SpaceCreatorProjectImpl(ProjectExplorer::Project *project, QObject *parent = nullptr);
    ~SpaceCreatorProjectImpl();

    ProjectExplorer::Project *project() const { return m_project; }

    QStringList projectFiles(const QString &suffix) const override;

private Q_SLOTS:
    void checkAsnFileRename();
    void saveIfNotOpen(shared::EditorCore *core);
    void reportAsn1Error(const QString &fileName, const QStringList &errors);
    void reportError(const shared::ErrorItem &error);
    void clearAllErrors();
    void clearTasksForFile(const QString &fileName);

private:
    bool isOpenInEditor(shared::EditorCore *core) const;

    ProjectExplorer::Project *m_project = nullptr;
    QStringList m_asnFiles;
    QVector<ProjectExplorer::Task> m_errors;
};

} // namespace spctr
