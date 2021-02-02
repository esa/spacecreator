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
#include <memory>

namespace ProjectExplorer {
class Project;
}
namespace scs {
class MscSystemChecks;
}
namespace shared {
class EditorCore;
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
    scs::MscSystemChecks *checks() const { return m_checks.get(); }

    QStringList projectFiles(const QString &suffix) const override;

private Q_SLOTS:
    void checkAsnFileRename();
    void saveIfNotOpen(shared::EditorCore *core);

private:
    bool isOpenInEditor(shared::EditorCore *core) const;

    ProjectExplorer::Project *m_project = nullptr;
    std::unique_ptr<scs::MscSystemChecks> m_checks;
    QStringList m_asnFiles;
};

} // namespace spctr
