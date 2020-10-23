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

#include <QPointer>
#include <QSharedPointer>
#include <QVector>

namespace aadl {
class AADLObject;
}

namespace aadlinterface {
class IVEditorCore;
namespace cmd {
class UndoCommand;
}
}

namespace msc {
class MSCEditorCore;
}

namespace spctr {
class AadlModelStorage;
class MscModelStorage;

/*!
   Class to perform consitency checks an corrections between MSC and AADL
 */
class MscSystemChecks : public QObject
{
    Q_OBJECT

public:
    MscSystemChecks(QObject *parent = nullptr);

    // Setup functions
    void setMscStorage(MscModelStorage *mscStorage);
    void setAadlStorage(AadlModelStorage *aadlStorage);

    // Check functions
    bool mscInstancesExists(const QString &name);
    void changeMscInstanceName(const QString &oldName, const QString &name);

    void checkInstances();
    void checkMessages();

    // Query functions
    QSharedPointer<aadlinterface::IVEditorCore> ivCore() const;
    QVector<QSharedPointer<msc::MSCEditorCore>> allMscCores() const;

    QStringList allAadlFiles() const;
    QStringList allMscFiles() const;
    QStringList allAsn1Files() const;
    QStringList projectFiles(const QString &suffix) const;

public Q_SLOTS:
    void onEntityNameChanged(
            aadl::AADLObject *entity, const QString &oldName, aadlinterface::cmd::UndoCommand *command);

private:
    QPointer<MscModelStorage> m_mscStorage;
    QPointer<AadlModelStorage> m_aadlStorage;
};

}
