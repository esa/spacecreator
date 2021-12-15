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

#include "ivappwidget.h"

#include "commands/cmdentitiesimport.h"
#include "commands/cmdentitiesinstantiate.h"
#include "commandsstack.h"
#include "context/action/actionsmanager.h"
#include "errorhub.h"
#include "graphicsviewutils.h"
#include "interfacedocument.h"
#include "itemeditor/graphicsitemhelpers.h"
#include "itemeditor/ivfunctiongraphicsitem.h"
#include "itemeditor/ivitemmodel.h"
#include "ivcreatortool.h"
#include "iveditorcore.h"
#include "ivexporter.h"
#include "ivvisualizationmodelbase.h"
#include "properties/ivpropertiesdialog.h"
#include "ui_ivappwidget.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QDebug>
#include <QFileInfo>
#include <QIcon>
#include <QMenu>
#include <QTimer>

namespace ive {

IVAppWidget::IVAppWidget(IVEditorCore *core, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IVAppWidget)
    , m_ivCore(core)
    , m_document(core->document())
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 1);

    initGraphicsView();
    initModelView();
    initImportView();
    initSharedView();
    initLayerView();

    connect(m_document->itemsModel(), &IVItemModel::itemDoubleClicked, this, &IVAppWidget::onItemDoubleClicked);

    QTimer::singleShot(0, this, [&]() {
        for (QAction *action : initActions()) {
            ui->toolBar->addAction(action);
        }
        ui->toolBar->addSeparator();
        for (QAction *action : initViewActions()) {
            ui->toolBar->addAction(action);
        }
    });
}

IVAppWidget::~IVAppWidget()
{
    delete ui;
}

ive::GraphicsView *IVAppWidget::graphicsView() const
{
    return ui->graphicsView;
}

void IVAppWidget::centerView()
{
    const QGraphicsItem *item = m_document->itemsModel()->getItem(m_document->objectsModel()->rootObjectId());
    const QList<QGraphicsItem *> items = item ? item->childItems() : graphicsView()->scene()->items();
    if (items.isEmpty() && item) {
        graphicsView()->centerOn(item->sceneBoundingRect().center());
    } else {
        QRectF rect;
        for (const QGraphicsItem *item : items) {
            if (item->type() > QGraphicsItem::UserType) {
                rect |= item->sceneBoundingRect();
            }
        }
        graphicsView()->centerOn(rect.center());
    }
}

void IVAppWidget::showContextMenuForIVModel(const QPoint &pos)
{
    const QModelIndex idx = ui->objectsView->indexAt(pos);
    if (!idx.isValid()) {
        return;
    }

    const auto obj = m_document->objectsModel()->getObject(
            idx.data(static_cast<int>(ive::IVVisualizationModelBase::IdRole)).toUuid());
    if (!obj) {
        return;
    }
    QList<QAction *> actions;

    if (obj->type() == ivm::IVObject::Type::Function || obj->type() == ivm::IVObject::Type::FunctionType) {
        QAction *actExportSelectedEntities = new QAction(tr("Export selected entities"));
        connect(actExportSelectedEntities, &QAction::triggered, m_document.data(),
                &InterfaceDocument::exportSelectedFunctions);
        actions.append(actExportSelectedEntities);
        ActionsManager::registerAction(
                Q_FUNC_INFO, actExportSelectedEntities, "Export selected entities", "Export selected entities");
    }

    if (obj->type() == ivm::IVObject::Type::FunctionType) {
        QAction *actExportSelectedSharedType = new QAction(tr("Export component type"));
        connect(actExportSelectedSharedType, &QAction::triggered, m_document.data(),
                &InterfaceDocument::exportSelectedType);
        actions.append(actExportSelectedSharedType);
        ActionsManager::registerAction(
                Q_FUNC_INFO, actExportSelectedSharedType, "Export component type", "Export component type");
    }
    QMenu *menu = new QMenu;
    menu->addActions(actions);
    menu->exec(ui->objectsView->mapToGlobal(pos));
}

void IVAppWidget::showAvailableLayers(const QPoint &pos)
{
    /** TODO: implement actions triggered from menu **/
    const QModelIndex idx = ui->objectsView->indexAt(pos);
    if (!idx.isValid()) {
        return;
    }

    const auto obj = m_document->objectsModel()->getObject(
            idx.data(static_cast<int>(ive::IVVisualizationModelBase::IdRole)).toUuid());
    if (!obj) {
        return;
    }

    QList<QAction *> actions;
    QMenu *menu = new QMenu;
    menu->addActions(actions);
    menu->exec(ui->layerView->mapToGlobal(pos));
}

void IVAppWidget::copyItems()
{
    QBuffer buffer;
    if (!buffer.open(QIODevice::WriteOnly)) {
        shared::ErrorHub::addError(
                shared::ErrorItem::Error, tr("Can't open buffer for exporting: %1").arg(buffer.errorString()), "");
        return;
    }

    QList<shared::VEObject *> objects;
    for (const QModelIndex &index : m_document->objectsSelectionModel()->selection().indexes()) {
        const int role = static_cast<int>(ive::IVVisualizationModelBase::IdRole);
        if (ivm::IVObject *object = m_document->objectsModel()->getObject(index.data(role).toUuid())) {
            objects.append(object);
        }
    }
    if (!m_document->exporter()->exportObjects(objects, &buffer)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Error during component export"));
        return;
    }
    buffer.close();
    QApplication::clipboard()->setText(QString::fromUtf8(buffer.data()));
}

void IVAppWidget::cutItems()
{
    copyItems();
    m_tool->removeSelectedItems();
}

void IVAppWidget::pasteItems(const QPointF &sceneDropPoint)
{
    const QByteArray data = QApplication::clipboard()->text().toUtf8();
    if (data.isEmpty()) {
        return;
    }

    QGraphicsItem *itemAtScenePos = m_document->scene()->itemAt(sceneDropPoint, graphicsView()->transform());
    while (itemAtScenePos && itemAtScenePos->type() != IVFunctionGraphicsItem::Type) {
        itemAtScenePos = itemAtScenePos->parentItem();
    }
    ivm::IVFunctionType *parentObject = gi::functionObject(itemAtScenePos);
    auto cmdImport = new cmd::CmdEntitiesImport(data, parentObject, m_document->objectsModel(), m_document->asn1Check(),
            sceneDropPoint, QFileInfo(m_document->path()).absolutePath());
    m_document->commandsStack()->push(cmdImport);
}

void IVAppWidget::showPropertyEditor(const shared::Id &id)
{
    Q_ASSERT(m_document);
    if (id.isNull()) {
        return;
    }

    ivm::IVObject *obj = m_document->objectsModel()->getObject(id);
    if (!obj || obj->type() == ivm::IVObject::Type::InterfaceGroup || obj->type() == ivm::IVObject::Type::Connection) {
        return;
    }

    ive::IVPropertiesDialog dialog(QFileInfo(m_document->path()).absolutePath(), m_document->dynPropConfig(), obj,
            m_document->ivCheck(), m_document->asn1Check(), m_document->commandsStack(), graphicsView());
    dialog.init();
    dialog.exec();
}

void IVAppWidget::importEntity(const shared::Id &id, const QPointF &sceneDropPoint)
{
    Q_ASSERT(m_document);
    const auto obj = m_document->importModel()->getObject(id);
    if (!obj) {
        return;
    }
    const auto existingFunctionNames = m_document->objectsModel()->nestedFunctionNames();
    const auto intersectedNames = m_document->importModel()
                                          ->nestedFunctionNames(obj->as<const ivm::IVFunctionType *>())
                                          .intersect(existingFunctionNames);
    if (!intersectedNames.isEmpty()) {
        const QString msg = tr("Chosen entity [%1] couldn't be imported because of Function names conflict(s): %2")
                                    .arg(obj->titleUI(), intersectedNames.toList().join(QLatin1Char('\n')));
        shared::ErrorHub::addError(shared::ErrorItem::Error, msg);
        return;
    }
    QGraphicsItem *itemAtScenePos = m_document->scene()->itemAt(sceneDropPoint, graphicsView()->transform());
    while (itemAtScenePos && itemAtScenePos->type() != IVFunctionGraphicsItem::Type) {
        itemAtScenePos = itemAtScenePos->parentItem();
    }

    QBuffer buffer;
    if (!buffer.open(QIODevice::WriteOnly)) {
        shared::ErrorHub::addError(
                shared::ErrorItem::Error, tr("Can't open buffer for exporting: %1").arg(buffer.errorString()));
        return;
    }

    if (!m_document->exporter()->exportObjects({ obj }, &buffer)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Error during component export"));
        return;
    }
    buffer.close();

    ivm::IVFunctionType *parentObject = gi::functionObject(itemAtScenePos);
    auto cmdImport = new cmd::CmdEntitiesImport(buffer.data(), parentObject, m_document->objectsModel(),
            m_document->asn1Check(), sceneDropPoint, QFileInfo(m_document->path()).absolutePath());
    m_document->commandsStack()->push(cmdImport);
}

void IVAppWidget::instantiateEntity(const shared::Id &id, const QPointF &sceneDropPoint)
{
    Q_ASSERT(m_document);
    const auto obj = m_document->sharedModel()->getObject(id);
    if (!obj || obj->type() != ivm::IVObject::Type::FunctionType) {
        return;
    }
    QGraphicsItem *itemAtScenePos = m_document->scene()->itemAt(sceneDropPoint, graphicsView()->transform());
    while (itemAtScenePos && itemAtScenePos->type() != IVFunctionGraphicsItem::Type) {
        itemAtScenePos = itemAtScenePos->parentItem();
    }
    ivm::IVFunctionType *parentObject = gi::functionObject(itemAtScenePos);
    auto cmdInstantiate = new cmd::CmdEntitiesInstantiate(
            obj->as<ivm::IVFunctionType *>(), parentObject, m_document->objectsModel(), sceneDropPoint);
    m_document->commandsStack()->push(cmdInstantiate);
}

void IVAppWidget::onItemDoubleClicked(const shared::Id &id)
{
    if (id.isNull()) {
        return;
    }

    if (auto entity = m_document->objectsModel()->getObject(id)) {
        if (entity->isFunction()) {
            if (auto fn = entity->as<ivm::IVFunction *>()) {
                if (fn->hasNestedChildren()) {
                    m_document->itemsModel()->changeRootItem(id);
                    return;
                }
            }
        }
        showPropertyEditor(id);
    }
}

void IVAppWidget::onItemCreated(const shared::Id &id)
{
    if (id.isNull()) {
        return;
    }

    showPropertyEditor(id);
}

void IVAppWidget::pasteItems()
{
    const QPoint viewportCursorPos = graphicsView()->viewport()->mapFromGlobal(QCursor::pos());
    QPointF sceneDropPoint;
    if (graphicsView()
                    ->viewport()
                    ->rect()
                    .marginsRemoved(shared::graphicsviewutils::kContentMargins.toMargins())
                    .contains(viewportCursorPos)) {
        sceneDropPoint = graphicsView()->mapToScene(viewportCursorPos);
    }
    pasteItems(sceneDropPoint);
}

void IVAppWidget::initGraphicsView()
{
    Q_ASSERT(m_document.data());

    connect(ui->graphicsView, &ive::GraphicsView::zoomChanged, this, [this](qreal percent) {
        m_document->itemsModel()->zoomChanged();
        m_actZoomIn->setEnabled(!qFuzzyCompare(percent, ui->graphicsView->maxZoomPercent()));
        m_actZoomOut->setEnabled(!qFuzzyCompare(percent, ui->graphicsView->minZoomPercent()));
    });

    connect(ui->graphicsView, &GraphicsView::importEntity, this, &IVAppWidget::importEntity);
    connect(ui->graphicsView, &GraphicsView::instantiateEntity, this, &IVAppWidget::instantiateEntity);
    connect(ui->graphicsView, &GraphicsView::copyItems, this, &IVAppWidget::copyItems);
    connect(ui->graphicsView, &GraphicsView::cutItems, this, &IVAppWidget::cutItems);
    connect(ui->graphicsView, &GraphicsView::pasteItems, this, qOverload<>(&IVAppWidget::pasteItems));

    ui->graphicsView->setScene(m_document->scene());
    ui->graphicsView->setUpdatesEnabled(false);
    m_document->itemsModel()->updateSceneRect();
    ui->graphicsView->setUpdatesEnabled(true);
}

void IVAppWidget::initModelView()
{
    ui->objectsView->setObjectName(QLatin1String("IVModelView"));
    ui->objectsView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    ui->objectsView->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    ui->objectsView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(ui->objectsView, &QTreeView::customContextMenuRequested, this, &IVAppWidget::showContextMenuForIVModel);

    ui->objectsView->setModel(m_document->visualisationModel());
    ui->objectsView->setSelectionModel(m_document->objectsSelectionModel());
}

void IVAppWidget::initImportView()
{
    ui->importView->setObjectName(QLatin1String("ImportView"));
    ui->importView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    ui->importView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->importView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    ui->importView->setModel(m_document->importVisualisationModel());
}

void IVAppWidget::initSharedView()
{
    ui->sharedView->setObjectName(QLatin1String("SharedView"));
    ui->sharedView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    ui->sharedView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->sharedView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    ui->sharedView->setModel(m_document->sharedVisualisationModel());
}

void IVAppWidget::initLayerView()
{
    ui->layerView->setObjectName(QLatin1String("IV Connection Layers"));
    ui->layerView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    ui->layerView->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    ui->layerView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    connect(ui->layerView, &QTreeView::customContextMenuRequested, this, &IVAppWidget::showAvailableLayers);
    ui->layerView->setModel(m_document->layerVisualisationModel());
}

QVector<QAction *> IVAppWidget::initActions()
{
    Q_ASSERT(m_document.data());
    if (m_tool != nullptr) {
        return m_toolbarActions;
    }

    auto actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);

    m_tool = new IVCreatorTool(graphicsView(), m_document);
    connect(m_tool, &IVCreatorTool::created, this, [this, actionGroup]() {
        if (QAction *currentAction = actionGroup->checkedAction()) {
            currentAction->setChecked(false);
        }
        m_tool->setCurrentToolType(IVCreatorTool::ToolType::Pointer);
    });
    connect(m_tool, &IVCreatorTool::functionCreated, m_document.data(), &InterfaceDocument::prepareEntityNameForEditing,
            Qt::QueuedConnection);
    connect(m_tool, &IVCreatorTool::propertyEditorRequest, this, &IVAppWidget::showPropertyEditor,
            Qt::QueuedConnection);
    connect(m_tool, &IVCreatorTool::informUser, m_document.data(), &InterfaceDocument::showInfoMessage);
    connect(m_tool, &IVCreatorTool::copyActionTriggered, this, &IVAppWidget::copyItems);
    connect(m_tool, &IVCreatorTool::cutActionTriggered, this, &IVAppWidget::cutItems);
    connect(m_tool, &IVCreatorTool::pasteActionTriggered, this, qOverload<const QPointF &>(&IVAppWidget::pasteItems));

    auto actCreateFunctionType = new QAction(tr("Function Type"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateFunctionType, "Function Type", "Create FunctionType object");
    actCreateFunctionType->setCheckable(true);
    actCreateFunctionType->setActionGroup(actionGroup);
    connect(actCreateFunctionType, &QAction::triggered, this, [this]() {
        m_tool->setCurrentToolType(IVCreatorTool::ToolType::FunctionType);
        qWarning() << Q_FUNC_INFO << "Not implemented yet.";
    });
    actCreateFunctionType->setIcon(QIcon(":/toolbar/icns/function_type.svg"));

    auto actCreateFunction = new QAction(tr("Function"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateFunction, "Function", "Create Function object");
    actCreateFunction->setCheckable(true);
    actCreateFunction->setActionGroup(actionGroup);
    connect(actCreateFunction, &QAction::triggered, this,
            [this]() { m_tool->setCurrentToolType(IVCreatorTool::ToolType::Function); });
    actCreateFunction->setIcon(QIcon(":/toolbar/icns/function.svg"));

    auto actCreateProvidedInterface = new QAction(tr("Provided Interface"));
    ActionsManager::registerAction(
            Q_FUNC_INFO, actCreateProvidedInterface, "Provided Interface", "Create Provided Interface object");
    actCreateProvidedInterface->setCheckable(true);
    actCreateProvidedInterface->setActionGroup(actionGroup);
    connect(actCreateProvidedInterface, &QAction::triggered, this,
            [this]() { m_tool->setCurrentToolType(IVCreatorTool::ToolType::ProvidedInterface); });
    actCreateProvidedInterface->setIcon(QIcon(":/toolbar/icns/pi.svg"));

    auto actCreateRequiredInterface = new QAction(tr("Required Interface"));
    ActionsManager::registerAction(
            Q_FUNC_INFO, actCreateRequiredInterface, "Required Interface", "Create Required Interface object");
    actCreateRequiredInterface->setCheckable(true);
    actCreateRequiredInterface->setActionGroup(actionGroup);
    connect(actCreateRequiredInterface, &QAction::triggered, this,
            [this]() { m_tool->setCurrentToolType(IVCreatorTool::ToolType::RequiredInterface); });
    actCreateRequiredInterface->setIcon(QIcon(":/toolbar/icns/ri.svg"));

    auto actCreateComment = new QAction(tr("Comment"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateComment, "Comment", "Create Comment object");
    actCreateComment->setCheckable(true);
    actCreateComment->setActionGroup(actionGroup);
    connect(actCreateComment, &QAction::triggered, this,
            [this]() { m_tool->setCurrentToolType(IVCreatorTool::ToolType::Comment); });
    actCreateComment->setIcon(QIcon(":/toolbar/icns/comment.svg"));

    auto actCreateConnection = new QAction(tr("Connection"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateConnection, "Connection", "Create Connection object");
    actCreateConnection->setCheckable(true);
    actCreateConnection->setActionGroup(actionGroup);
    connect(actCreateConnection, &QAction::triggered, this,
            [this]() { m_tool->setCurrentToolType(IVCreatorTool::ToolType::MultiPointConnection); });
    actCreateConnection->setIcon(QIcon(":/toolbar/icns/connection.svg"));

    m_actCreateConnectionGroup = new QAction(tr("Create Connection Group"));
    ActionsManager::registerAction(
            Q_FUNC_INFO, m_actCreateConnectionGroup, "Connection Group", "Create Connection group");
    m_actCreateConnectionGroup->setActionGroup(actionGroup);
    connect(m_actCreateConnectionGroup, &QAction::triggered, this, [this]() { m_tool->groupSelectedItems(); });
    m_actCreateConnectionGroup->setIcon(QIcon(":/toolbar/icns/connection_group.svg"));

    auto actManageConnectionLayers = new QAction(tr("Manage Connection Layers"));
    ActionsManager::registerAction(
            Q_FUNC_INFO, actManageConnectionLayers, "Connection Layer", "Manage Connection Layers");
    actManageConnectionLayers->setCheckable(false);
    //actManageConnectionLayers->setActionGroup(actionGroup);
    connect(actManageConnectionLayers, &QAction::toggled, this, [this]() {
        m_tool->setCurrentToolType(IVCreatorTool::ToolType::ConnectionLayer);
        qWarning() << Q_FUNC_INFO << "Not implemented yet.";
    });
    actManageConnectionLayers->setIcon(QIcon(":/toolbar/icns/connection_layer.svg"));

    m_actRemove = new QAction(tr("Remove"));
    ActionsManager::registerAction(Q_FUNC_INFO, m_actRemove, "Remove", "Remove selected object");
    m_actRemove->setIcon(QIcon(QLatin1String(":/toolbar/icns/delete.svg")));
    m_actRemove->setEnabled(false);
    m_actRemove->setShortcut(QKeySequence::Delete);
    connect(m_actRemove, &QAction::triggered, this, [this]() { m_tool->removeSelectedItems(); });

    m_actZoomIn = new QAction(tr("Zoom In"));
    ActionsManager::registerAction(Q_FUNC_INFO, m_actZoomIn, "Zoom In", "Scale up the current scene");
    m_actZoomIn->setIcon(QIcon(QLatin1String(":/toolbar/icns/zoom_in.svg")));
    m_actZoomIn->setShortcut(QKeySequence::ZoomIn);
    connect(m_actZoomIn, &QAction::triggered, this,
            [this]() { graphicsView()->setZoom(graphicsView()->zoom() + graphicsView()->zoomStepPercent()); });

    m_actZoomOut = new QAction(tr("Zoom Out"));
    ActionsManager::registerAction(Q_FUNC_INFO, m_actZoomOut, "Zoom Out", "Scale down the current scene");
    m_actZoomOut->setIcon(QIcon(QLatin1String(":/toolbar/icns/zoom_out.svg")));
    m_actZoomOut->setShortcut(QKeySequence::ZoomOut);
    connect(m_actZoomOut, &QAction::triggered, this,
            [this]() { graphicsView()->setZoom(graphicsView()->zoom() - graphicsView()->zoomStepPercent()); });

    m_actExitToRoot = new QAction(tr("Exit to root function"));
    m_actExitToRoot->setActionGroup(actionGroup);
    m_actExitToRoot->setEnabled(false);
    connect(m_actExitToRoot, &QAction::triggered, this, [this]() { m_document->itemsModel()->changeRootItem({}); });
    m_actExitToRoot->setIcon(QIcon(":/toolbar/icns/exit.svg"));

    m_actExitToParent = new QAction(tr("Exit to parent function"));
    m_actExitToParent->setActionGroup(actionGroup);
    m_actExitToParent->setEnabled(false);
    connect(m_actExitToParent, &QAction::triggered, this, [this]() {
        ivm::IVObject *parentObject = m_document->objectsModel()->rootObject()
                ? m_document->objectsModel()->rootObject()->parentObject()
                : nullptr;
        m_document->itemsModel()->changeRootItem(parentObject ? parentObject->id() : shared::InvalidId);
    });
    m_actExitToParent->setIcon(QIcon(":/toolbar/icns/exit_parent.svg"));

    m_toolbarActions = { actCreateFunctionType, actCreateFunction, actCreateProvidedInterface,
        actCreateRequiredInterface, actCreateComment, actCreateConnection, m_actCreateConnectionGroup,
        actManageConnectionLayers, m_actRemove, m_actZoomIn, m_actZoomOut, m_actExitToRoot, m_actExitToParent };

    connect(m_document->objectsModel(), &ivm::IVModel::rootObjectChanged, this, [this](const shared::Id &rootId) {
        Q_UNUSED(rootId)
        if (m_actExitToRoot) {
            m_actExitToRoot->setEnabled(nullptr != m_document->objectsModel()->rootObject());
        }
        if (m_actExitToParent) {
            m_actExitToParent->setEnabled(nullptr != m_document->objectsModel()->rootObject());
        }
        centerView();
    });
    connect(m_document->objectsSelectionModel(), &QItemSelectionModel::selectionChanged, this,
            [this](const QItemSelection &selected, const QItemSelection & /*deselected*/) {
                m_actRemove->setEnabled(!selected.isEmpty());
                const QModelIndexList idxs = selected.indexes();
                auto it = std::find_if(idxs.cbegin(), idxs.cend(), [](const QModelIndex &index) {
                    return index.data(static_cast<int>(ive::IVVisualizationModelBase::TypeRole)).toInt()
                            == static_cast<int>(ivm::IVObject::Type::Connection);
                });
                m_actCreateConnectionGroup->setEnabled(it != std::cend(idxs));
            });
    return m_toolbarActions;
}

QVector<QAction *> IVAppWidget::initViewActions()
{
    if (!m_viewActions.isEmpty() || !m_ivCore) {
        return m_viewActions;
    }

    m_viewActions.append(m_ivCore->actionToggleMinimap());
    m_viewActions.append(m_ivCore->actionToggleE2EView());
    m_viewActions.append(m_ivCore->actionExportFunctions());
    m_viewActions.append(m_ivCore->actionExportType());

    auto actionSaveSceneRender =
            new QAction(QIcon(QLatin1String(":/toolbar/icns/render.svg")), tr("Render Scene..."), this);
    ive::ActionsManager::registerAction(
            Q_FUNC_INFO, actionSaveSceneRender, "Render", "Save current scene complete render.");
    connect(actionSaveSceneRender, &QAction::triggered, this, [this]() { m_ivCore->onSaveRenderRequested(); });
    m_viewActions.append(actionSaveSceneRender);

    return m_viewActions;
}

} // namespace ive
