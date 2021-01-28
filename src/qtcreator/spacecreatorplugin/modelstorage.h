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

namespace dve {
class DVEditorCore;
}
namespace ive {
class IVEditorCore;
}
namespace msc {
class MSCEditorCore;
}

namespace shared {
class EditorCore;
}

namespace spctr {
class MscSystemChecks;

/*!
   Stores shared pointers to all aadl/msc file objects. And creates a new one if needed
 */
class ModelStorage : public QObject
{
    Q_OBJECT

public:
    explicit ModelStorage(QObject *parent = nullptr);

    void setChecker(MscSystemChecks *checks);

    QSharedPointer<dve::DVEditorCore> dvData(const QString &fileName);
    QSharedPointer<ive::IVEditorCore> ivData(const QString &fileName);
    QSharedPointer<msc::MSCEditorCore> mscData(const QString &fileName);

    void remove(const QString &fileName);

Q_SIGNALS:
    void editedExternally(shared::EditorCore *);
    void ivCoreAdded(QSharedPointer<ive::IVEditorCore> mscCore);
    void mscCoreAdded(QSharedPointer<msc::MSCEditorCore> mscCore);

private:
    void setIvData(const QString &fileName, QSharedPointer<ive::IVEditorCore> ivData);
    void setMscData(const QString &fileName, QSharedPointer<msc::MSCEditorCore> mscData);

    QHash<QString, QSharedPointer<ive::IVEditorCore>> m_ivStore;
    QHash<QString, QSharedPointer<msc::MSCEditorCore>> m_mscStore;
    QPointer<MscSystemChecks> m_checks;
};

}
