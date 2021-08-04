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

#include "dvmessagebindingswidget.h"

#include "dvconnection.h"
#include "dvmessagebindingsmodel.h"

#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>

namespace dve {

DVMessageBindingsWidget::DVMessageBindingsWidget(shared::cmd::CommandsStackBase::Macro *macro, QWidget *parent)
    : QWidget(parent)
    , m_cmdMacro(macro)
{
    auto layout = new QVBoxLayout(this);
    setLayout(layout);

    m_treeView = new QTreeView(this);
    m_treeView->setHeaderHidden(true);
    layout->addWidget(m_treeView);

    auto btnsLayout = new QHBoxLayout;
    btnsLayout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));

    auto button = new QPushButton(tr("Bind all"));
    connect(button, &QPushButton::pressed, this, &DVMessageBindingsWidget::bindAll);
    btnsLayout->addWidget(button);

    button = new QPushButton(tr("Unbind all"));
    connect(button, &QPushButton::pressed, this, &DVMessageBindingsWidget::unbindAll);
    btnsLayout->addWidget(button);
    layout->addLayout(btnsLayout);
}

void DVMessageBindingsWidget::initModel(dvm::DVConnection *connection, AbstractSystemChecks *systemChecker)
{
    // set up model
    if (!m_model) {
        m_model = new dve::DVMessageBindingsModel(m_cmdMacro, this);
        m_treeView->setModel(m_model);
    }
    m_connection = connection;
    m_model->initModel(m_connection, systemChecker);
}

void DVMessageBindingsWidget::bindAll()
{
    setMessagesChecked(true);
}

void DVMessageBindingsWidget::unbindAll()
{
    setMessagesChecked(false);
}

void DVMessageBindingsWidget::setMessagesChecked(bool value)
{
    for (int idx = 0; idx < m_model->rowCount(QModelIndex()); ++idx) {
        m_model->setData(
                m_model->index(idx), value ? Qt::CheckState::Checked : Qt::CheckState::Unchecked, Qt::CheckStateRole);
    }
}

} // namespace dve
