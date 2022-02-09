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
#include "dvsystemchecks.h"
#include "spacecreatorprojectimpl.h"
#include "spacecreatorprojectmanager.h"

#include <QHBoxLayout>
#include <QMessageBox>

namespace spctr {

DVMainWidget::DVMainWidget(SpaceCreatorProjectManager *projectManager, QWidget *parent)
    : QWidget(parent)
    , m_projectManager(projectManager)
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
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);
    m_plugin->mainwidget()->setParent(this);
    layout->addWidget(m_plugin->mainwidget());

    connect(m_plugin->actionCheckFunctions(), &QAction::triggered, this, &DVMainWidget::checkDVFunctionsValidity);
    connect(m_plugin->actionCheckMessages(), &QAction::triggered, this, &DVMainWidget::checkDVMessagesValidity);
}

void DVMainWidget::checkDVFunctionsValidity()
{
    SpaceCreatorProjectImpl *project = m_projectManager->project(dvPlugin());
    if (project) {
        bool ok = project->dvChecks()->checkFunctionBindings();
        if (ok) {
            QMessageBox::information(
                    m_plugin->mainwidget(), tr("DV function check"), tr("All deployment view functions are ok."));
        } else {
            QMessageBox::warning(m_plugin->mainwidget(), tr("DV function check"),
                    tr("Deployment view functions have errors.\nSee the issue panel for more details"));
        }
    }
}

void DVMainWidget::checkDVMessagesValidity()
{
    SpaceCreatorProjectImpl *project = m_projectManager->project(dvPlugin());
    if (project) {
        bool ok = project->dvChecks()->checkMessageBindings();
        if (ok) {
            QMessageBox::information(
                    m_plugin->mainwidget(), tr("DV messages check"), tr("All deployment view messages are ok."));
        } else {
            QMessageBox::warning(m_plugin->mainwidget(), tr("DV messages check"),
                    tr("Deployment view messages have errors.\nSee the issue panel for more details"));
        }
    }
}

}
