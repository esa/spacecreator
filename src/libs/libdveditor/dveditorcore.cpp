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

#include "abstractvisualizationmodel.h"
#include "baseitems/graphicsview.h"
#include "dvappmodel.h"
#include "dvitemmodel.h"
#include "dvmodel.h"
#include "ui/graphicsviewbase.h"

#include <QBoxLayout>
#include <QHeaderView>
#include <QSplitter>
#include <QTreeView>
#include <QUndoCommand>

namespace dve {

struct DVEditorCore::DeploymentInterfacePrivate {
    DeploymentInterfacePrivate(const DeploymentInterfacePrivate &) = delete;
    DeploymentInterfacePrivate &operator=(const DeploymentInterfacePrivate &) = delete;
    DeploymentInterfacePrivate()
        : m_scene(new QGraphicsScene)
        , m_appModel(new DVAppModel)
        , m_model(new DVItemModel(m_appModel->objectsModel(), m_appModel->undoStack()))
        , m_visualizationModel(
                  new shared::AbstractVisualizationModel(m_appModel->objectsModel(), m_appModel->commandsStack()))
        , m_view(new dve::GraphicsView)
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
    }

    QGraphicsScene *m_scene { nullptr };
    std::unique_ptr<dve::DVAppModel> m_appModel;
    std::unique_ptr<dve::DVItemModel> m_model;
    std::unique_ptr<shared::AbstractVisualizationModel> m_visualizationModel;
    QPointer<dve::GraphicsView> m_view;
    QPointer<QToolBar> m_toolBar;
    QWidget *m_mainWidget { nullptr };
};

DVEditorCore::DVEditorCore(QObject *parent)
    : shared::EditorCore(parent)
    , d(new DeploymentInterfacePrivate)
{
}

DVEditorCore::~DVEditorCore() { }

/*!
   Returns the deployment view data model
 */
DVAppModel *DVEditorCore::appModel() const
{
    return d->m_appModel.get();
}

void DVEditorCore::addToolBars(QMainWindow *window)
{
    Q_UNUSED(window)
}

shared::ui::GraphicsViewBase *DVEditorCore::chartView()
{
    return d->m_view;
}

QToolBar *DVEditorCore::toolBar()
{
    return d->m_toolBar;
}

QWidget *DVEditorCore::mainwidget()
{
    if (!d->m_mainWidget) {
        d->m_mainWidget = new QWidget;
        auto rootLayout = new QVBoxLayout;
        rootLayout->setMargin(0);
        d->m_mainWidget->setLayout(rootLayout);

        QSplitter *splitter = new QSplitter(Qt::Horizontal, d->m_mainWidget);
        splitter->setHandleWidth(1);
        auto treeView = new QTreeView(d->m_mainWidget);
        treeView->setHeaderHidden(true);
        treeView->setModel(d->m_visualizationModel.get());
        splitter->addWidget(treeView);
        splitter->addWidget(d->m_view);
        splitter->setStretchFactor(0, 0);
        splitter->setStretchFactor(1, 1);
        rootLayout->addWidget(splitter);
    }
    return d->m_mainWidget;
}

void DVEditorCore::registerBasicActions()
{
    // @todo
    // register undo/redo/...
}

QUndoStack *DVEditorCore::undoStack() const
{
    return d->m_appModel->undoStack();
}

shared::cmd::CommandsStackBase *DVEditorCore::commandsStack() const
{
    return d->m_appModel->commandsStack();
}

void DVEditorCore::populateCommandLineArguments(shared::CommandLineParser *parser) const
{
    Q_UNUSED(parser)
}

bool DVEditorCore::renameAsnFile(const QString &oldName, const QString &newName)
{
    Q_UNUSED(oldName)
    Q_UNUSED(newName)

    return false;
}

QString DVEditorCore::filePath() const
{
    return d->m_appModel->path();
}

bool DVEditorCore::save()
{
    return false;
}

}
