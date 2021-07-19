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

#include "abstractsystemchecks.h"
#include "abstractvisualizationmodel.h"
#include "baseitems/graphicsview.h"
#include "commands/cmdnodeentitycreate.h"
#include "commandsstackbase.h"
#include "dvappmodel.h"
#include "dvappwidget.h"
#include "dvboard.h"
#include "dvcreatortool.h"
#include "dvexporter.h"
#include "dvhwlibraryreader.h"
#include "dvitemmodel.h"
#include "dvmodel.h"
#include "dvpropertiesdialog.h"
#include "dvpropertytemplateconfig.h"
#include "dvvisualizationmodel.h"
#include "errorhub.h"
#include "ui/graphicsviewbase.h"

#include <QBoxLayout>
#include <QBuffer>
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
        , m_hwModel(new dvm::DVModel)
        , m_hwVisualizationModel(new DVVisualizationModel(m_hwModel.get(), m_appModel->commandsStack()))
        , m_exporter(new DVExporter)
        , m_dynPropConfig(dvm::DVPropertyTemplateConfig::instance())
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
    std::unique_ptr<dvm::DVModel> m_hwModel;
    std::unique_ptr<shared::AbstractVisualizationModel> m_hwVisualizationModel;
    std::unique_ptr<dve::DVCreatorTool> m_creatorTool;
    std::unique_ptr<dve::DVExporter> m_exporter;
    shared::PropertyTemplateConfig *m_dynPropConfig { nullptr };

    dve::AbstractSystemChecks *m_systemChecks = nullptr;

    QPointer<QToolBar> m_toolBar;
    QVector<QAction *> m_actions;
    QPointer<DVAppWidget> m_mainWidget;
};

DVEditorCore::DVEditorCore(QObject *parent)
    : shared::EditorCore(parent)
    , d(new DVEditorCorePrivate)
{
    connect(d->m_model.get(), &dve::DVItemModel::itemsSelected, this, &DVEditorCore::onSceneSelectionChanged);
    connect(d->m_model.get(), &dve::DVItemModel::itemDoubleClicked, this, &DVEditorCore::showPropertyEditor);
}

DVEditorCore::~DVEditorCore() { }

/*!
   Returns the deployment view data model
 */
DVAppModel *DVEditorCore::appModel() const
{
    return d->m_appModel.get();
}

void DVEditorCore::setSystemChecker(AbstractSystemChecks *checker)
{
    if (d->m_systemChecks == checker) {
        return;
    }

    if (d->m_systemChecks && d->m_systemChecks->parent() == this) {
        delete d->m_systemChecks;
    }
    d->m_systemChecks = checker;
    if (d->m_creatorTool) {
        d->m_creatorTool->setSystemChecker(checker);
    }
}

AbstractSystemChecks *DVEditorCore::systemChecker() const
{
    return d->m_systemChecks;
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
        for (QAction *action : initActions()) {
            d->m_toolBar->addAction(action);
        }
    }
    return d->m_toolBar;
}

QWidget *DVEditorCore::mainwidget()
{
    if (!d->m_mainWidget) {
        d->m_mainWidget = new DVAppWidget;
        d->m_mainWidget->setGraphicsScene(d->m_model->scene());
        d->m_mainWidget->setAadlModel(d->m_visualizationModel.get());
        d->m_mainWidget->setHWModel(d->m_hwVisualizationModel.get());
        d->m_mainWidget->setActions(initActions());
        connect(d->m_mainWidget->graphicsView(), &GraphicsView::importEntity, this, &DVEditorCore::importEntity);
        connect(d->m_mainWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this,
                &DVEditorCore::onViewSelectionChanged);
    }
    return d->m_mainWidget;
}

QVector<QAction *> DVEditorCore::initActions()
{
    if (d->m_creatorTool) {
        return d->m_actions;
    }

    auto actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);
    d->m_creatorTool.reset(
            new DVCreatorTool(d->m_mainWidget->graphicsView(), d->m_model.get(), d->m_appModel->commandsStack()));
    d->m_creatorTool->setSystemChecker(d->m_systemChecks);

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
    actCreatePartition->setIcon(QIcon(":/toolbar/icns/partition.svg"));
    actCreatePartition->setCheckable(true);
    actCreatePartition->setActionGroup(actionGroup);
    connect(actCreatePartition, &QAction::triggered, this,
            [this]() { d->m_creatorTool->setCurrentToolType(DVCreatorTool::ToolType::Partition); });
    d->m_actions.append(actCreatePartition);

    auto actCreateConnection = new QAction(tr("Connection"));
    actCreateConnection->setIcon(QIcon(":/toolbar/icns/connection.svg"));
    actCreateConnection->setCheckable(true);
    actCreateConnection->setActionGroup(actionGroup);
    connect(actCreateConnection, &QAction::triggered, this,
            [this]() { d->m_creatorTool->setCurrentToolType(DVCreatorTool::ToolType::MultiPointConnection); });
    d->m_actions.append(actCreateConnection);

    auto actRemove = new QAction(tr("Remove"));
    actRemove->setIcon(QIcon(QLatin1String(":/toolbar/icns/delete.svg")));
    actRemove->setEnabled(false);
    actRemove->setShortcut(QKeySequence::Delete);
    connect(actRemove, &QAction::triggered, this, [this]() { d->m_creatorTool->removeSelectedItems(); });
    d->m_actions.append(actRemove);

    auto actZoomIn = new QAction(tr("Zoom In"));
    actZoomIn->setIcon(QIcon(QLatin1String(":/toolbar/icns/zoom_in.svg")));
    actZoomIn->setShortcut(QKeySequence::ZoomIn);
    connect(actZoomIn, &QAction::triggered, this, [this]() {
        d->m_mainWidget->graphicsView()->setZoom(
                d->m_mainWidget->graphicsView()->zoom() + d->m_mainWidget->graphicsView()->zoomStepPercent());
    });
    d->m_actions.append(actZoomIn);

    auto actZoomOut = new QAction(tr("Zoom Out"));
    actZoomOut->setIcon(QIcon(QLatin1String(":/toolbar/icns/zoom_out.svg")));
    actZoomOut->setShortcut(QKeySequence::ZoomOut);
    connect(actZoomOut, &QAction::triggered, this, [this]() {
        d->m_mainWidget->graphicsView()->setZoom(
                d->m_mainWidget->graphicsView()->zoom() - d->m_mainWidget->graphicsView()->zoomStepPercent());
    });
    d->m_actions.append(actZoomOut);

    connect(d->m_mainWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this,
            [this, actRemove](const QItemSelection &selected, const QItemSelection & /*deselected*/) {
                actRemove->setEnabled(!selected.isEmpty());
            });

    return d->m_actions;
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
    return d->m_exporter->exportObjectsSilently(d->m_appModel->objectsModel()->objects().values(), filePath());
}

bool DVEditorCore::saveAs()
{
    return d->m_exporter->exportObjectsInteractively(d->m_appModel->objectsModel()->objects().values(), filePath());
}

DVExporter *DVEditorCore::exporter() const
{
    return d->m_exporter.get();
}

/*!
   Load the HW library from file in the given \p directory
 */
void DVEditorCore::loadHWLibrary(const QString &directory)
{
    QVector<dvm::DVObject *> objects;
    QDirIterator it(directory, QStringList() << "*.xml", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString fileName = it.next();
        shared::ErrorHub::setCurrentFile(fileName);
        dvm::DVHWLibraryReader reader;
        bool ok = reader.readFile(fileName);
        if (ok) {
            objects << reader.parsedObjects();
        }
    }
    shared::ErrorHub::clearCurrentFile();
    d->m_hwModel->initFromObjects(objects);
}

void DVEditorCore::showPropertyEditor(const shared::Id &id)
{
    if (auto obj = d->m_appModel->objectsModel()->getObject(id)) {
        dve::DVPropertiesDialog dialog(
                d->m_dynPropConfig, obj, d->m_appModel->commandsStack(), d->m_mainWidget->window());
        dialog.init();
        dialog.exec();
    }
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

void DVEditorCore::importEntity(const shared::Id &id, const QPointF &sceneDropPoint)
{
    const dvm::DVObject *obj = d->m_hwModel->getObject(id);
    if (!obj) {
        return;
    }
    if (obj->type() == dvm::DVObject::Type::Board) {
        if (auto board = qobject_cast<const dvm::DVBoard *>(obj)) {
            auto cmd = new cmd::CmdNodeEntityCreate(d->m_appModel->objectsModel(), board, sceneDropPoint);
            d->m_appModel->commandsStack()->push(cmd);
        }
    }
}

}
