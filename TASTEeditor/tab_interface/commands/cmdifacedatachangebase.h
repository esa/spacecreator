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

#include "commandids.h"
#include "tab_aadl/aadlobjectsmodel.h"

#include <QPointer>
#include <QUndoCommand>
#include <QVariant>

namespace taste3 {
namespace aadl {
namespace cmd {

class CmdIfaceDataChangeBase : public QUndoCommand
{
public:
    ~CmdIfaceDataChangeBase() override;

protected:
    CmdIfaceDataChangeBase() = delete;
    CmdIfaceDataChangeBase(const CmdIfaceDataChangeBase &other) = delete;
    explicit CmdIfaceDataChangeBase(AADLObjectIface *iface, const QString &targetName, const QVariant &targetValue,
                                    const QVariant &prevValue, QUndoCommand *parent = nullptr);

    QPointer<AADLObjectIface> m_iface;
    QPointer<AADLObjectsModel> m_model;
    QVector<AADLObjectConnection *> m_relatedConnections;
    const QString m_targetName;
    const meta::Props::Token m_targetToken;
    const QVariant m_oldValue, m_newValue;
    QVector<QUndoCommand *> m_cmdRmConnection;

    virtual QVector<QPointer<AADLObjectIface>> getRelatedIfaces();
    virtual QVector<AADLObjectConnection *> getRelatedConnections();
    virtual bool connectionMustDie(const AADLObjectConnection *connection) const = 0;
    static AADLObjectIface *getConnectionOtherSide(const AADLObjectConnection *connection,
                                                   AADLObjectIface *changedIface);

    void prepareRemoveConnectionCommands();
};

} // ns cmd
} // ns aadl
} // ns taste3
