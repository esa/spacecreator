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
#include "mscsystemchecks.h"

#include <QDebug>

namespace spctr {

ModelStorage::ModelStorage(QObject *parent)
    : QObject(parent)
{
}

void ModelStorage::setChecker(MscSystemChecks *checks)
{
    m_checks = checks;
}

QSharedPointer<dve::DVEditorCore> ModelStorage::dvData(const QString &fileName)
{
    Q_UNUSED(fileName)
    return {};
}

/*!
   Returns the IVEditorCore object for the given file
   If the object does not exist yet, one will be created and the data be loaded
 */
QSharedPointer<ive::IVEditorCore> ModelStorage::ivData(const QString &fileName)
{
    if (!m_ivStore.contains(fileName)) {
        QSharedPointer<ive::IVEditorCore> data(new ive::IVEditorCore());
        data->registerBasicActions();
        data->document()->customActions(); // There some further actions are registered

        data->document()->load(fileName);
        setIvData(fileName, data);
        return data;
    }

    return m_ivStore[fileName];
}

/*!
   Returns the MSCEditorCore object for the given file
   If the object does not exist yet, one will be created and the data be loaded
 */
QSharedPointer<msc::MSCEditorCore> ModelStorage::mscData(const QString &fileName)
{
    if (!m_mscStore.contains(fileName)) {
        QSharedPointer<msc::MSCEditorCore> data(new msc::MSCEditorCore());
        data->showToolbars(false);
        data->mainModel()->loadFile(fileName);
        setMscData(fileName, data);
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
    connect(ivData->commandsStack(), &ive::cmd::CommandsStack::nameChanged, m_checks,
            &spctr::MscSystemChecks::onEntityNameChanged);
    connect(ivData->commandsStack(), &ive::cmd::CommandsStack::entityRemoved, m_checks,
            &spctr::MscSystemChecks::onEntityRemoved);
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
    mscData->aadlChecker()->setIvCore(m_checks->ivCore());
    Q_EMIT mscCoreAdded(mscData);
}

}
