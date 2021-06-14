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
#include "dvappwidget.h"
#include "dvboard.h"
#include "dvboardreader.h"
#include "dvcreatortool.h"
#include "dvitemmodel.h"
#include "dvmodel.h"
#include "ui/graphicsviewbase.h"

#include <QBoxLayout>
#include <QDebug>
#include <QDirIterator>
#include <QHeaderView>
#include <QMainWindow>
#include <QMessageBox>
#include <QSplitter>
#include <QTreeView>
#include <QUndoCommand>

namespace dve {

struct DVEditorCore::DVEditorCorePrivate {
    DVEditorCorePrivate(const DVEditorCorePrivate &) = delete;
    DVEditorCorePrivate &operator=(const DVEditorCorePrivate &) = delete;
    DVEditorCorePrivate()
        : m_appModel(new DVAppModel)
        , m_model(new DVItemModel(m_appModel->objectsModel(), m_appModel->commandsStack()))
        , m_visualizationModel(
                  new shared::AbstractVisualizationModel(m_appModel->objectsModel(), m_appModel->commandsStack()))
        , m_hwModel(new dvm::DVBoardsModel)
    {
    }

    ~DVEditorCorePrivate()
    {
        if (m_toolBar && !m_toolBar->parent())
            delete m_toolBar;
    }

    std::unique_ptr<dve::DVAppModel> m_appModel;
    std::unique_ptr<dve::DVItemModel> m_model;
    std::unique_ptr<shared::AbstractVisualizationModel> m_visualizationModel;
    std::unique_ptr<dvm::DVBoardsModel> m_hwModel;
    std::unique_ptr<dve::DVCreatorTool> m_creatorTool;
    QPointer<QToolBar> m_toolBar;
    QPointer<DVAppWidget> m_mainWidget;
};

DVEditorCore::DVEditorCore(QObject *parent)
    : shared::EditorCore(parent)
    , d(new DVEditorCorePrivate)
{
    connect(d->m_model.get(), &dve::DVItemModel::itemsSelected, this, &DVEditorCore::onSceneSelectionChanged);
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
    if (window)
        window->addToolBar(toolBar());
}

shared::ui::GraphicsViewBase *DVEditorCore::chartView()
{
    return d->m_mainWidget->graphicsView();
}

QToolBar *DVEditorCore::toolBar()
{
    if (!d->m_toolBar) {
        d->m_toolBar = new QToolBar;
        d->m_toolBar->setObjectName("DeploymentToolBar");
        d->m_toolBar->setAllowedAreas(Qt::AllToolBarAreas);
        d->m_toolBar->setMovable(true);

        d->m_creatorTool.reset(
                new DVCreatorTool(d->m_mainWidget->graphicsView(), d->m_model.get(), d->m_appModel->commandsStack()));

        auto actionGroup = new QActionGroup(this);
        actionGroup->setExclusive(true);

        connect(d->m_creatorTool.get(), &DVCreatorTool::created, this, [this, actionGroup]() {
            if (QAction *currentAction = actionGroup->checkedAction())
                currentAction->setChecked(false);
            d->m_creatorTool->setCurrentToolType(DVCreatorTool::ToolType::Pointer);
        });
        connect(d->m_creatorTool.get(), &DVCreatorTool::propertyEditorRequest, this, &DVEditorCore::showPropertyEditor,
                Qt::QueuedConnection);
        connect(d->m_creatorTool.get(), &DVCreatorTool::informUser, this, &DVEditorCore::showInfoMessage);
        connect(d->m_creatorTool.get(), &DVCreatorTool::copyActionTriggered, this, &DVEditorCore::copyItems);
        connect(d->m_creatorTool.get(), &DVCreatorTool::cutActionTriggered, this, &DVEditorCore::cutItems);
        connect(d->m_creatorTool.get(), &DVCreatorTool::pasteActionTriggered, this,
                qOverload<const QPointF &>(&DVEditorCore::pasteItems));

        auto actCreatePartition = new QAction(tr("Partition"));
        actCreatePartition->setIcon(QIcon(":/tab_deployment/toolbar/icns/partition.svg"));
        actCreatePartition->setCheckable(true);
        actCreatePartition->setActionGroup(actionGroup);
        connect(actCreatePartition, &QAction::triggered, this,
                [this]() { d->m_creatorTool->setCurrentToolType(DVCreatorTool::ToolType::Partition); });
        d->m_toolBar->addAction(actCreatePartition);

        auto actCreateConnection = new QAction(tr("Connection"));
        actCreateConnection->setIcon(QIcon(":/tab_deployment/toolbar/icns/connection.svg"));
        actCreateConnection->setCheckable(true);
        actCreateConnection->setActionGroup(actionGroup);
        connect(actCreateConnection, &QAction::triggered, this,
                [this]() { d->m_creatorTool->setCurrentToolType(DVCreatorTool::ToolType::MultiPointConnection); });
        d->m_toolBar->addAction(actCreateConnection);

        d->m_toolBar->addSeparator();

        auto actRemove = new QAction(tr("Remove"));
        actRemove->setIcon(QIcon(QLatin1String(":/tab_deployment/toolbar/icns/delete.svg")));
        actRemove->setEnabled(false);
        actRemove->setShortcut(QKeySequence::Delete);
        connect(actRemove, &QAction::triggered, this, [this]() { d->m_creatorTool->removeSelectedItems(); });
        d->m_toolBar->addAction(actRemove);

        d->m_toolBar->addSeparator();

        auto actZoomIn = new QAction(tr("Zoom In"));
        actZoomIn->setIcon(QIcon(QLatin1String(":/tab_deployment/toolbar/icns/zoom_in.svg")));
        actZoomIn->setShortcut(QKeySequence::ZoomIn);
        connect(actZoomIn, &QAction::triggered, this, [this]() {
            d->m_mainWidget->graphicsView()->setZoom(
                    d->m_mainWidget->graphicsView()->zoom() + d->m_mainWidget->graphicsView()->zoomStepPercent());
        });
        d->m_toolBar->addAction(actZoomIn);

        auto actZoomOut = new QAction(tr("Zoom Out"));
        actZoomOut->setIcon(QIcon(QLatin1String(":/tab_deployment/toolbar/icns/zoom_out.svg")));
        actZoomOut->setShortcut(QKeySequence::ZoomOut);
        connect(actZoomOut, &QAction::triggered, this, [this]() {
            d->m_mainWidget->graphicsView()->setZoom(
                    d->m_mainWidget->graphicsView()->zoom() - d->m_mainWidget->graphicsView()->zoomStepPercent());
        });
        d->m_toolBar->addAction(actZoomOut);

        connect(d->m_mainWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this,
                [this, actRemove](const QItemSelection &selected, const QItemSelection & /*deselected*/) {
                    actRemove->setEnabled(!selected.isEmpty());
                });
    }
    return d->m_toolBar;
}

QWidget *DVEditorCore::mainwidget()
{
    if (!d->m_mainWidget) {
        d->m_mainWidget = new DVAppWidget;
        d->m_mainWidget->setGraphicsScene(d->m_model->scene());
        d->m_mainWidget->setAadlModel(d->m_visualizationModel.get());
        d->m_mainWidget->setHWModel(d->m_hwModel.get());
        connect(d->m_mainWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this,
                &DVEditorCore::onViewSelectionChanged);
    }
    return d->m_mainWidget;
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

/*!
   Load the HW library from file in the given \p directory
 */
void DVEditorCore::loadHWLibrary(const QString &directory)
{
    d->m_hwModel->clear(true);

    QList<dvm::DVBoard *> boards;
    QDirIterator it(directory, QStringList() << "*.xml", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString fileName = it.next();
        dvm::DVBoardReader reader;
        bool ok = reader.readFile(fileName);
        if (ok) {
            boards.append(reader.parsedBoards());
        }
    }
    d->m_hwModel->setObjectList(boards);
}

dvm::DVBoardsModel *DVEditorCore::hwModel() const
{
    return d->m_hwModel.get();
}

void DVEditorCore::showPropertyEditor()
{
    /// TODO:
}

void DVEditorCore::showInfoMessage(const QString &title, const QString &message)
{
    QMessageBox::information(d->m_mainWidget->window(), title, message);
}

void DVEditorCore::copyItems()
{
    /// TODO:
}

void DVEditorCore::cutItems()
{
    /// TODO:
}

void DVEditorCore::pasteItems()
{
    /// TODO:
}

void DVEditorCore::pasteItems(const QPointF &sceneDropPoint)
{
    /// TODO:
}

void DVEditorCore::onSceneSelectionChanged(const QList<shared::Id> &selectedObjects)
{
    QItemSelection itemSelection;
    for (auto id : selectedObjects) {
        const QModelIndex idx = d->m_visualizationModel->indexFromItem(d->m_visualizationModel->getItem(id));
        if (itemSelection.isEmpty()) {
            itemSelection.select(idx, idx);
        } else {
            itemSelection.merge(QItemSelection(idx, idx), QItemSelectionModel::SelectCurrent);
        }
    }
    d->m_mainWidget->selectionModel()->select(itemSelection,
            QItemSelectionModel::Rows | QItemSelectionModel::Current | QItemSelectionModel::ClearAndSelect);
}

void DVEditorCore::onViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    auto updateSelection = [this](const QItemSelection &selection, bool value) {
        for (const QModelIndex &idx : selection.indexes()) {
            if (auto graphicsItem =
                            d->m_model->getItem(idx.data(shared::AbstractVisualizationModel::IdRole).toUuid())) {
                graphicsItem->setSelected(value);
            }
        }
    };

    updateSelection(deselected, false);
    updateSelection(selected, true);
}

}
