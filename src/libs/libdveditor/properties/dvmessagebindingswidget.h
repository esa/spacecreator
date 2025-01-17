/*
  Copyright (C) 2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "commandsstackbase.h"

#include <QPointer>
#include <QWidget>

class QLabel;
class QTreeView;

namespace dvm {
class AbstractSystemChecks;
class DVConnection;
}

namespace dve {
class DVMessageBindingsModel;

class DVMessageBindingsWidget : public QWidget
{
    Q_OBJECT
public:
    DVMessageBindingsWidget(shared::cmd::CommandsStackBase::Macro *macro, QWidget *parent = nullptr);

    void initModel(dvm::DVConnection *connection, dvm::AbstractSystemChecks *systemChecker);

private Q_SLOTS:
    void bindAll();
    void unbindAll();

private:
    void setMessagesChecked(bool value);

    QLabel *m_sourceTargetName = nullptr;
    QTreeView *m_treeView = nullptr;
    DVMessageBindingsModel *m_model = nullptr;
    QPointer<dvm::DVConnection> m_connection;
    shared::cmd::CommandsStackBase::Macro *m_cmdMacro { nullptr };
};

} // namespace dve
