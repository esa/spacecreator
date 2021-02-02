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

namespace ivm {
class AADLObject;
class AADLConnection;
class AADLFunction;
}

namespace ive {
class IVEditorCore;
}

namespace msc {
class MSCEditorCore;
}

namespace shared {
class UndoCommand;
}

namespace scs {
class EditorCoreQuery;

/*!
   Class to perform consistency checks for MSC against a AADL model
 */
class MscSystemChecks : public QObject
{
    Q_OBJECT

public:
    MscSystemChecks(QObject *parent = nullptr);

    // Setup functions
    void setStorage(scs::EditorCoreQuery *storage);

    // Check functions
    bool mscInstancesExist(const QString &name);
    void changeMscInstanceName(const QString &oldName, const QString &name);
    void removeMscInstances(ivm::AADLFunction *aadlFunction);
    bool hasCorrespondingInstances(ivm::AADLFunction *aadlFunction) const;

    bool mscMessagesExist(const QString &messageName, const QString &sourceName, const QString &targetName);
    void changeMscMessageName(
            const QString &oldName, const QString &name, const QString &sourceName, const QString &targetName);
    void removeMscMessages(ivm::AADLConnection *aadlConnection);
    bool hasCorrespondingMessages(ivm::AADLConnection *aadlConnection) const;

    void checkInstances();
    void checkMessages();

public Q_SLOTS:
    void onEntityNameChanged(ivm::AADLObject *entity, const QString &oldName, shared::UndoCommand *command);
    void onMscEntityNameChanged(QObject *entity, const QString &oldName, shared::UndoCommand *command);
    void onEntityRemoved(ivm::AADLObject *entity, shared::UndoCommand *command);

private:
    QPointer<EditorCoreQuery> m_storage;
    Qt::CaseSensitivity m_caseCheck = Qt::CaseInsensitive;
    bool m_nameUpdateRunning = false;
};

}
