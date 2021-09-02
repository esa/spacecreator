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

#include "dvmainwidget.h"

#include "actionsbar.h"
#include "commandsstack.h"

#include <QHBoxLayout>
#include <QMessageBox>

namespace spctr {

DVMainWidget::DVMainWidget(QWidget *parent)
    : QWidget(parent)
{
}

DVMainWidget::~DVMainWidget() { }

bool DVMainWidget::init(DVEditorCorePtr data)
{
    m_plugin = data;
    init();
    return true;
}

DVEditorCorePtr DVMainWidget::dvPlugin() const
{
    return m_plugin;
}

void DVMainWidget::init()
{
    if (m_plugin.isNull()) {
        return;
    }

    auto layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);
    m_plugin->mainwidget()->setParent(this);
    layout->addWidget(m_plugin->mainwidget());
}

}
