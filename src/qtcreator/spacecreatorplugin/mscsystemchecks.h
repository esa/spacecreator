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
}

namespace msc {
class MSCEditorCore;
}

namespace shared {
class UndoCommand;
}

namespace spctr {
class AadlModelStorage;
class MscModelStorage;

/*!
   Class to perform consistency checks for MSC against a AADL model
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
    bool mscInstancesExist(const QString &name);
    void changeMscInstanceName(const QString &oldName, const QString &name);

    bool mscMessagesExist(const QString &messageName, const QString &sourceName, const QString &targetName);
    void changeMscMessageName(
            const QString &oldName, const QString &name, const QString &sourceName, const QString &targetName);

    void checkInstances();
    void checkMessages();

    // Query functions
    QSharedPointer<aadlinterface::IVEditorCore> ivCore() const;
    QVector<QSharedPointer<msc::MSCEditorCore>> allMscCores() const;

    static QStringList allAadlFiles();
    static QStringList allMscFiles();
    static QStringList allAsn1Files();
    static QStringList projectFiles(const QString &suffix);

public Q_SLOTS:
    void onEntityNameChanged(aadl::AADLObject *entity, const QString &oldName, shared::UndoCommand *command);
    void onMscEntityNameChanged(QObject *entity, const QString &oldName, shared::UndoCommand *command);

private:
    QPointer<MscModelStorage> m_mscStorage;
    QPointer<AadlModelStorage> m_aadlStorage;
    Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;
    bool m_nameUpdateRunning = false;
};

}
