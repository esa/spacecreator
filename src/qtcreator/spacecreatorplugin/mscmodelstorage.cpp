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

#include "mscmodelstorage.h"

#include "mainmodel.h"
#include "msceditorcore.h"

namespace spctr {

MscModelStorage::MscModelStorage(QObject *parent)
    : QObject(parent)
{
}

/*!
   Returns the MSCEditorCore object for the given file
   If the object does not exist yet, one will be created and the data be loaded
 */
QSharedPointer<msc::MSCEditorCore> MscModelStorage::mscData(const QString &fileName)
{
    if (!m_store.contains(fileName)) {
        QSharedPointer<msc::MSCEditorCore> data(new msc::MSCEditorCore());
        data->mainModel()->loadFile(fileName);
        setMscData(fileName, data);
        return data;
    }

    return m_store[fileName];
}

/*!
   Clears the whole store
 */
void MscModelStorage::remove(const QString &fileName)
{
    m_store.remove(fileName);
}

/*!
   Sets the MSCEditorCore object for the given file.
   If the object was already used for another file, that old file/object connection is removed.
 */
void MscModelStorage::setMscData(const QString &fileName, QSharedPointer<msc::MSCEditorCore> mscData)
{
    const QString oldKey = m_store.key(mscData, "");
    if (!oldKey.isEmpty()) {
        if (m_store[fileName] == mscData) {
            return;
        }
        QSharedPointer<msc::MSCEditorCore> oldData = m_store.take(oldKey);
        disconnect(oldData.data(), nullptr, this, nullptr);
    }

    m_store[fileName] = mscData;
    connect(mscData.data(), &shared::EditorCore::editedExternally, this, &spctr::MscModelStorage::editedExternally);
    Q_EMIT coreAdded(mscData);
}

}
