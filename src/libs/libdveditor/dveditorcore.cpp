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

#include "dveditorcore.h"

#include "baseitems/graphicsview.h"
#include "deploymentmodel.h"
#include "ui/graphicsviewbase.h"

#include <QUndoCommand>

namespace deploymentinterface {

struct DVEditorCore::DeploymentInterfacePrivate {
    DeploymentInterfacePrivate(const DeploymentInterfacePrivate &) = delete;
    DeploymentInterfacePrivate &operator=(const DeploymentInterfacePrivate &) = delete;
    DeploymentInterfacePrivate()
        : m_commandsStack(new QUndoStack)
        , m_scene(new QGraphicsScene)
        , m_model(new DeploymentModel)
        , m_view(new deploymentinterface::GraphicsView)
        , m_toolBar(new QToolBar)
    {
        m_toolBar->setObjectName("Document ToolBar");
        m_toolBar->setAllowedAreas(Qt::AllToolBarAreas);
        m_toolBar->setMovable(true);

        m_view->setScene(m_scene);
    }

    ~DeploymentInterfacePrivate()
    {
        delete m_toolBar;
        delete m_view;
        delete m_commandsStack;
    }

    QUndoStack *m_commandsStack { nullptr };
    QGraphicsScene *m_scene { nullptr };
    std::unique_ptr<deploymentinterface::DeploymentModel> m_model;
    deploymentinterface::GraphicsView *m_view { nullptr };
    QToolBar *m_toolBar { nullptr };
};

DVEditorCore::DVEditorCore(QObject *parent)
    : shared::EditorCore(parent)
    , d(new DeploymentInterfacePrivate)
{
}

DVEditorCore::~DVEditorCore() { }

void DVEditorCore::setPluginActive(bool active) { }

void DVEditorCore::addMenuFileActions(QMenu *menu, QMainWindow *window) { }

void DVEditorCore::addMenuEditActions(QMenu *menu, QMainWindow *window) { }

void DVEditorCore::addMenuHelpActions(QMenu *menu, QMainWindow *window) { }

void DVEditorCore::addToolBars(QMainWindow *window) { }

shared::ui::GraphicsViewBase *DVEditorCore::chartView()
{
    return d->m_view;
}

QToolBar *DVEditorCore::toolBar()
{
    return d->m_toolBar;
}

QUndoStack *DVEditorCore::undoStack() const
{
    return d->m_commandsStack;
}

void DVEditorCore::populateCommandLineArguments(shared::CommandLineParser *parser) const { }

bool DVEditorCore::renameAsnFile(const QString &oldName, const QString &newName) { }

QString DVEditorCore::filePath() const { }

bool DVEditorCore::save() { }

}
