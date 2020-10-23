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

#include "mscsystemchecks.h"

#include "aadlchecks.h"
#include "aadlmodelstorage.h"
#include "chartlayoutmanager.h"
#include "interface/commands/cmdentityattributechange.h"
#include "iveditorcore.h"
#include "mainmodel.h"
#include "mscchart.h"
#include "msceditorcore.h"
#include "mscinstance.h"
#include "mscmodel.h"
#include "mscmodelstorage.h"

#include <QDebug>
#include <QMessageBox>
#include <QUndoStack>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>
#include <utils/fileutils.h>

namespace spctr {

MscSystemChecks::MscSystemChecks(QObject *parent)
    : QObject(parent)
{
}

void MscSystemChecks::setMscStorage(MscModelStorage *mscStorage)
{
    m_mscStorage = mscStorage;
}

void MscSystemChecks::setAadlStorage(AadlModelStorage *aadlStorage)
{
    m_aadlStorage = aadlStorage;
}

/*!
   Returns if at least one instance in one of the .msc files has the name \p name
 */
bool MscSystemChecks::mscInstancesExists(const QString &name)
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
void MscSystemChecks::changeMscInstanceName(const QString &oldName, const QString &name)
{
    for (QSharedPointer<msc::MSCEditorCore> mscCore : allMscCores()) {
        mscCore->changeMscInstanceName(oldName, name);
    }
}

void MscSystemChecks::checkInstances()
{
    QSharedPointer<aadlinterface::IVEditorCore> ivp = ivCore();
    if (!ivp) {
        return;
    }

    QVector<QSharedPointer<msc::MSCEditorCore>> mscCores = allMscCores();

    for (QSharedPointer<msc::MSCEditorCore> mplugin : mscCores) {
        mplugin->aadlChecker()->setIvCore(ivp);
    }

    // Check for names
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> resultNames;
    for (QSharedPointer<msc::MSCEditorCore> mplugin : mscCores) {
        resultNames += mplugin->aadlChecker()->checkInstanceNames();
    }

    // Check for nested functions usage
    QVector<QPair<msc::MscChart *, msc::MscInstance *>> resultRelations;
    for (QSharedPointer<msc::MSCEditorCore> mplugin : mscCores) {
        resultRelations += mplugin->aadlChecker()->checkInstanceRelations();
    }

    QString text;
    if (!resultNames.isEmpty()) {
        text += tr("Following instances have no corresponding aadl function:\n");
        for (auto item : resultNames) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
    }
    if (!resultRelations.isEmpty()) {
        if (!text.isEmpty()) {
            text += "\n\n";
        }
        text += tr("Following instances are used with parent/hild of nested functions:\n");
        for (auto item : resultRelations) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
    }

    if (resultNames.isEmpty() && resultRelations.isEmpty()) {
        QMessageBox::information(nullptr, tr("All instaces are ok"), tr("All instaces are ok"));
    } else {
        QMessageBox::information(nullptr, tr("Non conforming instances"), text);
    }
}

void MscSystemChecks::checkMessages()
{
    QSharedPointer<aadlinterface::IVEditorCore> ivp = ivCore();
    if (!ivp) {
        return;
    }

    QVector<QSharedPointer<msc::MSCEditorCore>> mscCores = allMscCores();

    // check messages
    QVector<QPair<msc::MscChart *, msc::MscMessage *>> resultNames;
    for (QSharedPointer<msc::MSCEditorCore> mplugin : mscCores) {
        mplugin->aadlChecker()->setIvCore(ivp);
        resultNames += mplugin->aadlChecker()->checkMessages();
    }

    QString text;
    if (!resultNames.isEmpty()) {
        text += tr("Following messages have no corresponding aadl connection:\n");
        for (auto item : resultNames) {
            if (!text.isEmpty()) {
                text += "\n";
            }
            text += QString("%1 from chart %2").arg(item.second->name(), item.first->name());
        }
    }
    if (resultNames.isEmpty()) {
        QMessageBox::information(nullptr, tr("All messages are ok"), tr("All messages are ok"));
    } else {
        QMessageBox::information(nullptr, tr("Non conforming messages"), text);
    }
}

QSharedPointer<aadlinterface::IVEditorCore> MscSystemChecks::ivCore() const
{
    QStringList aadlFiles = allAadlFiles();
    if (aadlFiles.empty()) {
        qWarning() << "No AADL file in the projec";
        return {};
    }

    return m_aadlStorage->ivData(aadlFiles.first());
}

/*!
   Returns all MSCEditorCore objects, that are used in the current project
 */
QVector<QSharedPointer<msc::MSCEditorCore>> MscSystemChecks::allMscCores() const
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
QStringList MscSystemChecks::allAadlFiles() const
{
    return projectFiles("interfaceview.xml");
}

/*!
   Returns all msc files of the current project
 */
QStringList MscSystemChecks::allMscFiles() const
{
    return projectFiles(".msc");
}

/*!
   Returns all asn files of the current project
 */
QStringList MscSystemChecks::allAsn1Files() const
{
    return projectFiles(".asn");
}

/*!
   Returns all files of the current project endig with the given \p suffix
 */
QStringList MscSystemChecks::projectFiles(const QString &suffix) const
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

void MscSystemChecks::onEntityNameChanged(
        aadl::AADLObject *entity, const QString &oldName, aadlinterface::cmd::UndoCommand *command)
{
    if (!command->isFirstChange() && !command->checkSystem()) {
        return;
    }

    auto cmdAttribChange = dynamic_cast<aadlinterface::cmd::CmdEntityAttributeChange *>(command);
    if (cmdAttribChange) {
        if (mscInstancesExists(oldName)) {
            if (command->isFirstChange()) {
                const int result = QMessageBox::question(
                        nullptr, tr("Update instances"), tr("Do you want to update MSC instances?"));
                if (result == QMessageBox::Yes) {
                    cmdAttribChange->setSystemCheck(true);
                    changeMscInstanceName(oldName, entity->title());
                }
            } else {
                changeMscInstanceName(oldName, entity->title());
            }
        }
    }
}

}
