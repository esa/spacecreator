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

namespace MscPlugin {

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
        m_store[fileName] = data;
        return data;
    }

    return m_store[fileName];
}

/*!
   Sets the MSCEditorCore object for the given file.
   If the object was already used for another file, that old file/object connection is removed.
 */
void MscModelStorage::setMscData(const QString &fileName, QSharedPointer<msc::MSCEditorCore> mscData)
{
    const QString oldKey = m_store.key(mscData, "");
    if (!oldKey.isEmpty()) {
        m_store.remove(oldKey);
    }

    m_store[fileName] = mscData;
}

}
