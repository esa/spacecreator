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

#pragma once

#include <QHash>
#include <QObject>
#include <QSharedPointer>

namespace msc {
class MSCEditorCore;
}

namespace shared {
class EditorCore;
}

namespace spctr {

/*!
   Stores shared pointers to all msc file objects. And creates a new one if needed
 */
class MscModelStorage : public QObject
{
    Q_OBJECT
public:
    explicit MscModelStorage(QObject *parent = nullptr);

    QSharedPointer<msc::MSCEditorCore> mscData(const QString &fileName);

Q_SIGNALS:
    void editedExternally(shared::EditorCore *);
    void coreAdded(QSharedPointer<msc::MSCEditorCore> mscCore);

private:
    void setMscData(const QString &fileName, QSharedPointer<msc::MSCEditorCore> mscData);

    QHash<QString, QSharedPointer<msc::MSCEditorCore>> m_store;
};

}
