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

#include "aadlcommonprops.h" // for Props, Props::Token
#include "undocommand.h"

#include <QPointer>
#include <QVariant>
#include <QVector>

namespace ivm {
class IVObject;
class IVConnection;
class IVInterface;
class IVModel;
}

namespace ive {
namespace cmd {

class CmdIfaceDataChangeBase : public shared::UndoCommand
{
    Q_OBJECT

public:
    ~CmdIfaceDataChangeBase() override;

    virtual QVector<ivm::IVConnection *> getRelatedConnections();
    ivm::IVInterface *interface() const;

Q_SIGNALS:
    void nameChanged(ivm::IVObject *entity, const QString &oldName, shared::UndoCommand *command);

protected:
    CmdIfaceDataChangeBase() = delete;
    CmdIfaceDataChangeBase(const CmdIfaceDataChangeBase &other) = delete;
    explicit CmdIfaceDataChangeBase(ivm::IVInterface *iface, const QString &targetName,
            const QVariant &targetValue, const QVariant &prevValue, QUndoCommand *parent = nullptr);

    QPointer<ivm::IVInterface> m_iface;
    QPointer<ivm::IVModel> m_model;
    QVector<ivm::IVConnection *> m_relatedConnections;
    const QString m_targetName;
    const ivm::meta::Props::Token m_targetToken;
    const QVariant m_oldValue, m_newValue;
    QVector<QUndoCommand *> m_cmdRmConnection;

    virtual QVector<QPointer<ivm::IVInterface>> getRelatedIfaces();
    virtual bool connectionMustDie(const ivm::IVConnection *connection) const = 0;
    static ivm::IVInterface *getConnectionOtherSide(
            const ivm::IVConnection *connection, ivm::IVInterface *changedIface);

    void prepareRemoveConnectionCommands();
};

}
}
