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

#include "ivmainwidget.h"

#include "actionsbar.h"
#include "commandsstack.h"
#include "errorhub.h"
#include "interfacedocument.h"
#include "ivappwidget.h"

#include <QHBoxLayout>
#include <QMessageBox>

namespace spctr {

IVMainWidget::IVMainWidget(QWidget *parent)
    : QWidget(parent)
{
}

IVMainWidget::~IVMainWidget()
{
    if (m_plugin && m_plugin->mainwidget() && m_plugin->mainwidget()->parent() == this) {
        m_plugin->mainwidget()->setParent(nullptr);
    }
}

bool IVMainWidget::init(IVEditorCorePtr data)
{
    m_plugin = data;
    init();
    return true;
}

void IVMainWidget::setMinimapVisible(bool visible)
{
    if (m_plugin.isNull()) {
        return;
    }
    m_plugin->minimapView()->setVisible(visible);
}

IVEditorCorePtr IVMainWidget::ivPlugin() const
{
    return m_plugin;
}

void IVMainWidget::init()
{
    if (m_plugin.isNull()) {
        return;
    }

    auto layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);
    m_plugin->document()->init();

    layout->addWidget(m_plugin->mainwidget());
    connect(m_plugin->actionToggleE2EView(), &QAction::triggered, this, &IVMainWidget::requestE2EDataflow);

    m_plugin->setupMiniMap();
}

}
