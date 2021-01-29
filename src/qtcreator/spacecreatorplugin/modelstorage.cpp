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

#include "modelstorage.h"

#include "aadlsystemchecks.h"
#include "commandsstack.h"
#include "dveditorcore.h"
#include "interface/commands/cmdentityattributechange.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"
#include "mainmodel.h"
#include "msceditorcore.h"

#include <QDebug>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>
#include <utils/fileutils.h>

namespace spctr {

ModelStorage::ModelStorage(QObject *parent)
    : QObject(parent)
{
}

QSharedPointer<dve::DVEditorCore> ModelStorage::dvData(const QString &fileName) const
{
    Q_UNUSED(fileName)
    return {};
}

/*!
   Returns the IVEditorCore object for the given file
   If the object does not exist yet, one will be created and the data be loaded
 */
QSharedPointer<ive::IVEditorCore> ModelStorage::ivData(const QString &fileName) const
{
    if (!m_ivStore.contains(fileName)) {
        QSharedPointer<ive::IVEditorCore> data(new ive::IVEditorCore());
        data->registerBasicActions();
        data->document()->customActions(); // There some further actions are registered

        data->document()->load(fileName);
        const_cast<ModelStorage *>(this)->setIvData(fileName, data);
        return data;
    }

    return m_ivStore[fileName];
}

/*!
   Returns the MSCEditorCore object for the given file
   If the object does not exist yet, one will be created and the data be loaded
 */
QSharedPointer<msc::MSCEditorCore> ModelStorage::mscData(const QString &fileName) const
{
    if (!m_mscStore.contains(fileName)) {
        QSharedPointer<msc::MSCEditorCore> data(new msc::MSCEditorCore());
        data->showToolbars(false);
        data->mainModel()->loadFile(fileName);
        const_cast<ModelStorage *>(this)->setMscData(fileName, data);
        return data;
    }

    return m_mscStore[fileName];
}

/*!
   Removes the file \p fileName if included in the store
 */
void ModelStorage::remove(const QString &fileName)
{
    m_ivStore.remove(fileName);
}

QSharedPointer<ive::IVEditorCore> ModelStorage::ivCore() const
{
    QStringList aadlFiles = allAadlFiles();
    if (aadlFiles.empty()) {
        qWarning() << "No AADL file in the project";
        return {};
    }

    return ivData(aadlFiles.first());
}

/*!
   Returns all MSCEditorCore objects, that are used in the current project
 */
QVector<QSharedPointer<msc::MSCEditorCore>> ModelStorage::allMscCores() const
{
    QStringList mscFiles = allMscFiles();
    QVector<QSharedPointer<msc::MSCEditorCore>> allMscCores;
    for (const QString &mscFile : mscFiles) {
        QSharedPointer<msc::MSCEditorCore> core = mscData(mscFile);
        if (core) {
            allMscCores.append(core);
        }
    }
    return allMscCores;
}

/*!
   Returns if the given \core is in this storage
 */
bool ModelStorage::contains(QSharedPointer<shared::EditorCore> core) const
{
    for (QSharedPointer<ive::IVEditorCore> ivCore : m_ivStore) {
        if (core == ivCore) {
            return true;
        }
    }
    for (QSharedPointer<msc::MSCEditorCore> mscCore : m_mscStore) {
        if (core == mscCore) {
            return true;
        }
    }
    return false;
}

/*!
   Returns all aald files of the current project
 */
QStringList ModelStorage::allAadlFiles()
{
    return projectFiles("interfaceview.xml");
}

/*!
   Returns all msc files of the current project
 */
QStringList ModelStorage::allMscFiles()
{
    return projectFiles(".msc");
}

/*!
   Returns all asn files of the current project
 */
QStringList ModelStorage::allAsn1Files()
{
    return projectFiles(".asn");
}

/*!
   Returns all files of the current project endig with the given \p suffix
 */
QStringList ModelStorage::projectFiles(const QString &suffix)
{
    ProjectExplorer::Project *project = ProjectExplorer::ProjectTree::currentProject();
    if (!project) {
        return {};
    }

    QStringList result;
    for (const Utils::FileName &fileName : project->files(ProjectExplorer::Project::AllFiles)) {
        if (fileName.toString().endsWith(suffix, Qt::CaseInsensitive)) {
            result.append(fileName.toString());
        }
    }

    return result;
}

/*!
   Sets the IVEditorCore object for the given file.
   If the object was already used for another file, that old file/object connection is removed.
 */
void ModelStorage::setIvData(const QString &fileName, QSharedPointer<ive::IVEditorCore> ivData)
{
    const QString oldKey = m_ivStore.key(ivData, "");
    if (!oldKey.isEmpty()) {
        if (m_ivStore[fileName] == ivData) {
            return;
        }
        QSharedPointer<ive::IVEditorCore> oldData = m_ivStore.take(oldKey);
        disconnect(oldData.data(), nullptr, this, nullptr);
    }

    m_ivStore[fileName] = ivData;
    connect(ivData.data(), &shared::EditorCore::editedExternally, this, &spctr::ModelStorage::editedExternally);
    Q_EMIT ivCoreAdded(ivData);
}

/*!
   Sets the MSCEditorCore object for the given file.
   If the object was already used for another file, that old file/object connection is removed.
 */
void ModelStorage::setMscData(const QString &fileName, QSharedPointer<msc::MSCEditorCore> mscData)
{
    const QString oldKey = m_mscStore.key(mscData, "");
    if (!oldKey.isEmpty()) {
        if (m_mscStore[fileName] == mscData) {
            return;
        }
        QSharedPointer<msc::MSCEditorCore> oldData = m_mscStore.take(oldKey);
        disconnect(oldData.data(), nullptr, this, nullptr);
    }

    m_mscStore[fileName] = mscData;
    connect(mscData.data(), &shared::EditorCore::editedExternally, this, &spctr::ModelStorage::editedExternally);
    mscData->aadlChecker()->setIvCore(ivCore());
    Q_EMIT mscCoreAdded(mscData);
}

}
