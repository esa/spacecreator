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

#include <QTreeView>
#include <QVBoxLayout>

namespace dve {

DVMessageBindingsWidget::DVMessageBindingsWidget(QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QVBoxLayout(this);
    setLayout(layout);

    m_treeView = new QTreeView(this);
    m_treeView->setHeaderHidden(true);
    layout->addWidget(m_treeView);
}

void DVMessageBindingsWidget::initModel(dvm::DVConnection *connection)
{
    // set up model
    if (!m_model) {
        m_model = new dve::DVMessageBindingsModel(this);
        m_treeView->setModel(m_model);
    }
    m_connection = connection;
    m_model->initModel(m_connection);
}

} // namespace dve
