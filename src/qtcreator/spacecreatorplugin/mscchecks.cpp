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

#include "mscchecks.h"

#include "chartlayoutmanager.h"
#include "commands/cmdentitynamechange.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "msceditorcore.h"
#include "mscinstance.h"
#include "mscmodel.h"
#include "mscmodelstorage.h"

#include <QDebug>
#include <QUndoStack>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>
#include <utils/fileutils.h>

namespace spctr {

MscChecks::MscChecks(QObject *parent)
    : shared::MscChecksBase(parent)
{
}

void MscChecks::setMscStorage(MscModelStorage *mscStorage)
{
    m_mscStorage = mscStorage;
}

/*!
   Returns if at least one instance in one of the .msc files has the name \p name
 */
bool MscChecks::mscInstancesExists(const QString &name)
{
    for (QSharedPointer<msc::MSCEditorCore> mscCore : allMscCores()) {
        for (msc::MscChart *chart : mscCore->mainModel()->mscModel()->allCharts()) {
            for (msc::MscInstance *instance : chart->instances()) {
                if (instance->name() == name) {
                    return true;
                }
            }
        }
    }
    return false;
}

/*!
   Changes all instances that have the name \p oldName to have the new name \p name
 */
void MscChecks::changeMscInstanceName(const QString &oldName, const QString &name)
{
    for (QSharedPointer<msc::MSCEditorCore> mscCore : allMscCores()) {
        for (msc::MscChart *chart : mscCore->mainModel()->mscModel()->allCharts()) {
            for (msc::MscInstance *instance : chart->instances()) {
                if (instance->name() == oldName) {
                    QUndoStack *undoStack = mscCore->undoStack();
                    auto cmd = new msc::cmd::CmdEntityNameChange(
                            instance, name, &(mscCore->mainModel()->chartViewModel()));
                    undoStack->push(cmd);
                }
            }
        }
    }
}

/*!
   Returns all MSCEditorCore objects, that are used in the current project
 */
QVector<QSharedPointer<msc::MSCEditorCore>> MscChecks::allMscCores() const
{
    QStringList mscFiles = allMscFiles();
    QVector<QSharedPointer<msc::MSCEditorCore>> allMscCores;
    for (const QString &mscFile : mscFiles) {
        QSharedPointer<msc::MSCEditorCore> core = m_mscStorage->mscData(mscFile);
        if (core) {
            allMscCores.append(core);
        }
    }
    return allMscCores;
}

/*!
   Returns all aald files of the current project
 */
QStringList MscChecks::allAadlFiles() const
{
    return projectFiles("interfaceview.xml");
}

/*!
   Returns all msc files of the current project
 */
QStringList MscChecks::allMscFiles() const
{
    return projectFiles(".msc");
}

/*!
   Returns all asn files of the current project
 */
QStringList MscChecks::allAsn1Files() const
{
    return projectFiles(".asn");
}

/*!
   Returns all files of the current project endig with the given \p suffix
 */
QStringList MscChecks::projectFiles(const QString &suffix) const
{
    ProjectExplorer::Project *project = ProjectExplorer::ProjectTree::currentProject();
    if (!project) {
        return {};
    }

    QStringList result;
    for (Utils::FileName fileName : project->files(ProjectExplorer::Project::AllFiles)) {
        if (fileName.toString().endsWith(suffix, Qt::CaseInsensitive)) {
            result.append(fileName.toString());
        }
    }

    return result;
}

}
