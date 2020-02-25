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

#include "tab_aadl/aadlcommonprops.h"
#include "tab_aadl/aadlobjectsmodel.h"

#include <QPointer>
#include <QRect>
#include <QUndoCommand>
#include <QVariant>

namespace taste3 {
namespace aadl {

class AADLObjectIfaceRequired;
class AADLObjectConnection;
namespace cmd {

class CmdRequiredIfacePropertyChange : public QUndoCommand
{
public:
    explicit CmdRequiredIfacePropertyChange(AADLObjectIfaceRequired *entity, const QString &propName,
                                            const QVariant &value);
    ~CmdRequiredIfacePropertyChange();

    void redo() override;
    void undo() override;
    bool mergeWith(const QUndoCommand *command) override;
    int id() const override;

private:
    QPointer<AADLObjectIfaceRequired> m_ri;
    QPointer<AADLObjectsModel> m_model;
    QVector<AADLObjectConnection *> m_relatedConnections;
    const QString m_propertyName;
    const meta::Props::Token m_propertyToken;
    const QVariant m_oldValue, m_newValue;
    QVector<QUndoCommand *> m_cmdRmConnection;

private:
    void setPropLabelInherited(const QVariant &labelInherited);
    void removeConnections();
    void restoreConnections();

    void prepareRemoveConnectionCommands();
};

} // namespace cmd
} // namespace aadl
} // namespace taste3
