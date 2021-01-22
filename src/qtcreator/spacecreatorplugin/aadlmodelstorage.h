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

#include <QHash>
#include <QObject>
#include <QPointer>
#include <QSharedPointer>

#pragma once

namespace ive {
class IVEditorCore;
}

namespace shared {
class EditorCore;
}

namespace spctr {
class MscSystemChecks;

/*!
   Stores shared pointers to all aadl file objects. And creates a new one if needed
 */
class AadlModelStorage : public QObject
{
    Q_OBJECT

public:
    explicit AadlModelStorage(QObject *parent = nullptr);

    void setChecker(MscSystemChecks *checks);

    QSharedPointer<ive::IVEditorCore> ivData(const QString &fileName);

    void remove(const QString &fileName);

Q_SIGNALS:
    void editedExternally(shared::EditorCore *);
    void coreAdded(QSharedPointer<ive::IVEditorCore> mscCore);

private:
    void setIvData(const QString &fileName, QSharedPointer<ive::IVEditorCore> ivData);

    QHash<QString, QSharedPointer<ive::IVEditorCore>> m_store;
    QPointer<MscSystemChecks> m_checks;
};

}
