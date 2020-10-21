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

#include "aadlmodelstorage.h"

#include "aadlmscchecks.h"
#include "interface/interfacedocument.h"
#include "iveditorcore.h"

#include <QDebug>

namespace spctr {

AadlModelStorage::AadlModelStorage(QObject *parent)
    : QObject(parent)
{
}

void AadlModelStorage::setChecker(AadlMscChecks *checks)
{
    m_checks = checks;
}

/*!
   Returns the IVEditorCore object for the given file
   If the object does not exist yet, one will be created and the data be loaded
 */
QSharedPointer<aadlinterface::IVEditorCore> AadlModelStorage::ivData(const QString &fileName)
{
    if (!m_store.contains(fileName)) {
        QSharedPointer<aadlinterface::IVEditorCore> data(new aadlinterface::IVEditorCore());
        data->document()->setChecker(m_checks);
        data->document()->load(fileName);
        setIvData(fileName, data);
        return data;
    }

    return m_store[fileName];
}

/*!
   Sets the IVEditorCore object for the given file.
   If the object was already used for another file, that old file/object connection is removed.
 */
void AadlModelStorage::setIvData(const QString &fileName, QSharedPointer<aadlinterface::IVEditorCore> ivData)
{
    const QString oldKey = m_store.key(ivData, "");
    if (!oldKey.isEmpty()) {
        if (m_store[fileName] == ivData) {
            return;
        }
        QSharedPointer<aadlinterface::IVEditorCore> oldData = m_store.take(oldKey);
        disconnect(oldData.data(), nullptr, this, nullptr);
    }

    m_store[fileName] = ivData;
    connect(ivData.data(), &shared::EditorCore::editedExternally, this, &spctr::AadlModelStorage::editedExternally);
}

}
