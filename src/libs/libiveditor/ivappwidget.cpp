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

#include "archetypesmanagerdialog.h"
#include "breadcrumbwidget.h"
#include "commands/cmdconnectionitemcreate.h"
#include "commands/cmdconnectionlayermanage.h"
#include "commands/cmdentitiesimport.h"
#include "commands/cmdentitiesinstantiate.h"
#include "commands/cmdentitiesremove.h"
#include "commands/cmdfunctionitemcreate.h"
#include "commands/cmdinterfaceitemcreate.h"
#include "commandsstack.h"
#include "context/action/actionsmanager.h"
#include "errorhub.h"
#include "geometry.h"
#include "graphicsviewutils.h"
#include "interfacedocument.h"
#include "itemeditor/graphicsitemhelpers.h"
#include "itemeditor/ivfunctiongraphicsitem.h"
#include "itemeditor/ivitemmodel.h"
#include "ivcomponentmodel.h"
#include "ivconnection.h"
#include "ivconnectionlayertype.h"
#include "ivcreatortool.h"
#include "iveditattributesdialog.h"
#include "iveditattributesmodel.h"
#include "iveditorcore.h"
#include "ivexporter.h"
#include "ivnamevalidator.h"
#include "ivvisualizationmodelbase.h"
#include "ivxmlreader.h"
#include "properties/ivpropertiesdialog.h"
#include "qitemselectionmodel.h"
#include "ui_ivappwidget.h"

#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QItemSelectionModel>
#include <QMenu>
#include <QMessageBox>
#include <QPointer>
#include <QProcess>
#include <QStringList>
#include <QTimer>

static QSet<QString> nestedFunctionNames(const QVector<ivm::IVObject *> &objects)
{
    QSet<QString> names;
    std::for_each(objects.constBegin(), objects.constEnd(), [&](ivm::IVObject *obj) {
        if (auto fn = qobject_cast<ivm::IVFunctionType *>(obj)) {
            names.insert(obj->title());
            names.unite(nestedFunctionNames(fn->children()));
        }
    });
    return names;
}

static QSet<QString> nestedFunctionNames(ivm::IVFunctionType *fn)
{
    if (!fn)
        return {};

    QSet<QString> names { fn->title() };
    for (auto childFn : fn->functions()) {
        names << childFn->title();
        names.unite(nestedFunctionNames(childFn));
    }
    for (auto childFn : fn->functionTypes()) {
        names << childFn->title();
        names.unite(nestedFunctionNames(childFn));
    }
    return names;
}

namespace ive {

IVAppWidget::IVAppWidget(IVEditorCore *core, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::IVAppWidget)
    , m_ivCore(core)
    , m_document(core->document())
{
    ui->setupUi(this);
    m_breadCrumb = new shared::BreadcrumbWidget(ui->graphicsView);
    m_breadCrumb->raise();

    ui->splitter->setStretchFactor(0, 0);
    ui->splitter->setStretchFactor(1, 1);

    initGraphicsView();
    initModelView();
    initImportView();
    initSharedView();
    initLayerView();

    connect(m_document->itemsModel(), &IVItemModel::itemDoubleClicked, this, &IVAppWidget::onItemDoubleClicked);
    connect(m_breadCrumb, &shared::BreadcrumbWidget::levelClicked, this, [this](const QString &level) {
        const shared::Id id = shared::Id::fromString(level);
        m_document->itemsModel()->changeRootItem(id);
    });

    for (QAction *action : initActions()) {
        ui->toolBar->addAction(action);
    }
    ui->toolBar->addSeparator();
    for (QAction *action : initViewActions()) {
        ui->toolBar->addAction(action);
    }
}

IVAppWidget::~IVAppWidget()
{
    delete ui;
}

ive::GraphicsView *IVAppWidget::graphicsView() const
{
    return ui->graphicsView;
}

/**
 * Center the view point of the main interfaceview
 */
void IVAppWidget::centerView()
{
    // Get the QGraphicsItem that represents the root object. Root object is the object that is currently shown with
    // nested functions in it.
    auto rootObjectId = m_document->objectsModel()->rootObjectId();
    const QGraphicsItem *rootItem = m_document->itemsModel()->getItem(rootObjectId);
    // Get the items of the entire graph or the nested items within the root object
    const QList<QGraphicsItem *> items = rootItem ? rootItem->childItems() : graphicsView()->scene()->items();

    if (items.isEmpty() && rootItem) {
        // No root item. Center on the center of the entire graph
        graphicsView()->centerOn(rootItem->sceneBoundingRect().center());
    } else {
        // center on the center of the nested items
        QRectF rect;
        for (const QGraphicsItem *item : items) {
            if (item->type() > QGraphicsItem::UserType) {
                rect |= item->sceneBoundingRect();
            }
        }
        graphicsView()->centerOn(rect.center());
    }
}

QAction *IVAppWidget::actionDelete() const
{
    return m_actRemove;
}

void IVAppWidget::showContextMenuForSharedTypesView(const QPoint &pos)
{
    const QModelIndex idx = ui->sharedView->indexAt(pos);
    if (!idx.isValid()) {
        return;
    }

    const shared::Id id = idx.data(static_cast<int>(ive::IVVisualizationModelBase::IdRole)).toUuid();
    const auto obj = m_document->sharedTypesModel()->getObject(id);
    if (!obj) {
        return;
    }

    if (obj->type() != ivm::IVObject::Type::FunctionType) {
        return;
    }

    auto menu = new QMenu;
    QAction *actInstantiateSharedType = menu->addAction(tr("Instantiate entity"));
    connect(
            actInstantiateSharedType, &QAction::triggered, this,
            [this, id]() { instantiateEntity(id, shared::INVALID_POS); }, Qt::QueuedConnection);

    //    QAction *actImportSharedType = menu->addAction(tr("Import shared type"));
    //    connect(actImportSharedType, &QAction::triggered, this,
    //            [this, id]() { QMessageBox::warning(this, tr("Import shared type"), tr("Not implemented yet.")); },
    //            Qt::QueuedConnection);

    QAction *actEditSharedType = menu->addAction(tr("Edit shared type"));
    connect(actEditSharedType, &QAction::triggered, this,
            [this, id] { editExternalEntity(m_document->sharedTypePath(id)); });

    QAction *actRemoveSharedType = menu->addAction(tr("Remove shared type"));
    connect(actRemoveSharedType, &QAction::triggered, this, [this, id]() {
        if (auto model = m_document->sharedTypesModel()) {
            if (auto obj = model->getObject(id)) {
                const QFileInfo fi { model->componentPath(id) };
                model->removeComponent(id);
                if (fi.exists()) {
                    fi.absoluteDir().removeRecursively();
                }
            }
        }
    });

    menu->exec(ui->sharedView->mapToGlobal(pos));
}

void IVAppWidget::editExternalEntity(const QString &path)
{
    const QFileInfo fi(path);
    const QDir dir(fi.absoluteDir());
    const QString defaultProFile = dir.filePath(dir.dirName() + QLatin1String(".pro"));
    const QStringList proFiles = dir.entryList({ QLatin1String("*.pro") }, QDir::Filter::Files);
    QString proFilePath;
    if (proFiles.isEmpty()) {
        QMessageBox::warning(this, tr("Edit component"), tr("There is no pro-file in component folder: %1.").arg(path));
        return;
    } else if (proFiles.size() > 1) {
        if (!QFile::exists(defaultProFile)) {
            QMessageBox::warning(
                    this, tr("Edit component"), tr("There is no pro-file in component folder: %1.").arg(path));
            return;
        }
        proFilePath = defaultProFile;
    } else {
        proFilePath = dir.filePath(proFiles.front());
    }
    const bool ok = QProcess::startDetached(qApp->applicationFilePath(),
            { QLatin1String("-pid"), QString::number(qApp->applicationPid()), proFilePath });
    if (!ok) {
        QMessageBox::warning(this, tr("Edit component"), tr("Error during component opening for editing."));
    } else {
        QProcess::startDetached(qApp->applicationFilePath(),
                { QLatin1String("-pid"), QString::number(qApp->applicationPid()),
                        dir.filePath(shared::kDefaultInterfaceViewFileName) });
    }
}

void IVAppWidget::hideSelectedRows()
{
    QTreeView *treeView = ui->objectsView;
    QAbstractItemModel *treeViewModel = treeView->model();
    QItemSelectionModel *treeViewSelectionModel = treeView->selectionModel();
    int rowCount = treeViewModel->rowCount();
    for (int row = 0; row < rowCount; row++) {
        QModelIndex index = treeViewModel->index(row, 0);
        bool isSelected = treeViewSelectionModel->isSelected(index);
        if (isSelected) {
            treeViewModel->setData(index, QVariant(Qt::Unchecked), Qt::CheckStateRole);
        }
    }
}

void IVAppWidget::hideUnselectedRows()
{
    QTreeView *treeView = ui->objectsView;
    QAbstractItemModel *treeViewModel = treeView->model();
    QItemSelectionModel *treeViewSelectionModel = treeView->selectionModel();
    int rowCount = treeViewModel->rowCount();
    for (int row = 0; row < rowCount; row++) {
        QModelIndex index = treeViewModel->index(row, 0);
        bool isSelected = treeViewSelectionModel->isSelected(index);
        if (!isSelected) {
            // Uncheck row
            treeViewModel->setData(index, QVariant(Qt::Unchecked), Qt::CheckStateRole);
        }
    }
}

void IVAppWidget::showAllRows()
{
    QTreeView *treeView = ui->objectsView;
    QAbstractItemModel *treeViewModel = treeView->model();
    int rowCount = treeViewModel->rowCount();
    for (int row = 0; row < rowCount; row++) {
        QModelIndex index = treeViewModel->index(row, 0);
        treeViewModel->setData(index, QVariant(Qt::Checked), Qt::CheckStateRole);
    }
}

void IVAppWidget::showSelectedRows()
{
    QTreeView *treeView = ui->objectsView;
    QAbstractItemModel *treeViewModel = treeView->model();
    QItemSelectionModel *treeViewSelectionModel = treeView->selectionModel();
    int rowCount = treeViewModel->rowCount();
    for (int row = 0; row < rowCount; row++) {
        QModelIndex index = treeViewModel->index(row, 0);
        bool isSelected = treeViewSelectionModel->isSelected(index);
        if (isSelected) {
            treeViewModel->setData(index, QVariant(Qt::Checked), Qt::CheckStateRole);
        }
    }
}

void IVAppWidget::showContextMenuForComponentsLibraryView(const QPoint &pos)
{
    const QModelIndex idx = ui->importView->indexAt(pos);
    if (!idx.isValid()) {
        return;
    }

    const shared::Id id = idx.data(static_cast<int>(ive::IVVisualizationModelBase::IdRole)).toUuid();
    const auto obj = m_document->componentModel()->getObject(id);
    if (!obj) {
        return;
    }

    if (obj->type() != ivm::IVObject::Type::Function && obj->type() != ivm::IVObject::Type::FunctionType) {
        return;
    }

    auto menu = new QMenu;
    QAction *actImportComponent = menu->addAction(tr("Import entity"));
    connect(
            actImportComponent, &QAction::triggered, this, [this, id]() { importEntity(id, shared::INVALID_POS); },
            Qt::QueuedConnection);

    QAction *actImportAsReference = menu->addAction(tr("Add reference"));
    connect(
            actImportAsReference, &QAction::triggered, this, [this, id]() { linkEntity(id, shared::INVALID_POS); },
            Qt::QueuedConnection);

    QAction *actEditComponent = menu->addAction(tr("Edit component"));
    connect(actEditComponent, &QAction::triggered, this,
            [this, id] { editExternalEntity(m_document->componentPath(id)); });

    QAction *actRemoveComponent = menu->addAction(tr("Remove component"));
    connect(actRemoveComponent, &QAction::triggered, this, [this, id]() {
        if (auto model = m_document->componentModel()) {
            if (auto obj = model->getObject(id)) {
                const QFileInfo fi { model->componentPath(id) };
                model->removeComponent(id);
                if (fi.exists()) {
                    fi.absoluteDir().removeRecursively();
                }
            }
        }
    });

    menu->exec(ui->importView->mapToGlobal(pos));
}

/**
 *  IV Structure - context menu
 */
void IVAppWidget::showContextMenuForIVModel(const QPoint &pos)
{
    QList<QAction *> actions; // The actions that will go in the context menu

    // Are there visible or hidden items among the selected item?
    int visibleItemsSelected = 0;
    int hiddenItemsSelected = 0;
    QTreeView *treeView = ui->objectsView;
    QAbstractItemModel *treeViewModel = treeView->model();
    QItemSelectionModel *treeViewSelectionModel = treeView->selectionModel();
    QModelIndexList selectedRows = treeViewSelectionModel->selectedRows();

    for (QModelIndex modelIndex : selectedRows) {
        QVariant checkState = treeViewModel->data(modelIndex, Qt::CheckStateRole);
        if (checkState == Qt::Checked) {
            ++visibleItemsSelected;
        }
        if (checkState == Qt::Unchecked) {
            ++hiddenItemsSelected;
        }
    }

    // What was clicked
    const QModelIndex idx = treeView->indexAt(pos);

    // Find the model object that was clicked
    const auto obj = m_document->objectsModel()->getObject(
            idx.data(static_cast<int>(ive::IVVisualizationModelBase::IdRole)).toUuid());

    if (obj) {
        // Export selected entities
        if (obj->isFunction() || obj->isFunctionType()) {
            QAction *actExportSelectedEntities = new QAction(tr("Export selected entities"));
            connect(actExportSelectedEntities, &QAction::triggered, m_document.data(),
                    &InterfaceDocument::exportSelectedFunctions);
            actions.append(actExportSelectedEntities);
            ActionsManager::registerAction(
                    Q_FUNC_INFO, actExportSelectedEntities, "Export selected entities", "Export selected entities");
        }
        // Export component type
        if (obj->isFunction() || obj->isFunctionType()) {
            QAction *actExportSelectedSharedType = new QAction(tr("Export component type"));
            connect(actExportSelectedSharedType, &QAction::triggered, m_document.data(),
                    &InterfaceDocument::exportSelectedType);
            actions.append(actExportSelectedSharedType);
            ActionsManager::registerAction(
                    Q_FUNC_INFO, actExportSelectedSharedType, "Export component type", "Export component type");
        }
    }
    // An area not occupied by an entity was clicked

    // Show selected entities
    bool makeShowContextMenuItem = hiddenItemsSelected > 1;
    if (makeShowContextMenuItem) {
        QAction *actShowSelectedEntities = new QAction(tr("Show"));
        connect(actShowSelectedEntities, &QAction::triggered, this, &IVAppWidget::showSelectedRows);
        actions.append(actShowSelectedEntities);
        ActionsManager::registerAction(
                Q_FUNC_INFO, actShowSelectedEntities, "Show selected entities", "Show selected entities");
    }

    // Hide selected entities
    bool makeHideContextMenuItem = visibleItemsSelected > 1;
    if (makeHideContextMenuItem) {
        QAction *actHideSelectedEntities = new QAction(tr("Hide"));
        connect(actHideSelectedEntities, &QAction::triggered, this, &IVAppWidget::hideSelectedRows);
        actions.append(actHideSelectedEntities);
        ActionsManager::registerAction(
                Q_FUNC_INFO, actHideSelectedEntities, "Hide selected entities", "Hide selected entities");
    }

    // Hide Unselected entities
    bool makeHideUnSelectedContextMenuItem = visibleItemsSelected > 0;
    if (makeHideUnSelectedContextMenuItem) {
        QAction *actHideUnselectedEntities = new QAction(tr("Hide Unselected"));
        connect(actHideUnselectedEntities, &QAction::triggered, this, &IVAppWidget::hideUnselectedRows);
        actions.append(actHideUnselectedEntities);
        ActionsManager::registerAction(
                Q_FUNC_INFO, actHideUnselectedEntities, "Hide unselected rows", "Hide unselected rows");
    }

    // Show All Entities

    QAction *actShowAllEntities = new QAction(tr("Show all"));
    connect(actShowAllEntities, &QAction::triggered, this, &IVAppWidget::showAllRows);
    actions.append(actShowAllEntities);
    ActionsManager::registerAction(
            Q_FUNC_INFO, actShowAllEntities, "Show all entities", "All entities that has been hidden becomes visible.");

    // Setup the context menu
    QMenu *menu = new QMenu;
    menu->addActions(actions);
    menu->exec(treeView->mapToGlobal(pos));
}

void IVAppWidget::showAvailableLayers(const QPoint &pos)
{
    const QModelIndex idx = ui->layerView->indexAt(pos);
    if (!idx.isValid()) {
        return;
    }
    const auto *obj = m_document->layersModel()->getObject(
            idx.data(static_cast<int>(ive::IVVisualizationModelBase::IdRole)).toUuid());
    if (obj == nullptr) {
        return;
    }

    QList<QAction *> actions;
    if (obj->type() == ivm::IVObject::Type::ConnectionLayer) {
        const auto *layer = qobject_cast<const ivm::IVConnectionLayerType *>(obj);
        auto *actAddNewLayer = new QAction(tr("Add"));
        connect(actAddNewLayer, &QAction::triggered, this, [&]() {
            auto cmd = new cmd::CmdConnectionLayerCreate(m_document->layersModel(), m_document->objectsModel());
            if (cmd->layer() != nullptr) {
                m_document->commandsStack()->push(cmd);
            } else {
                delete cmd;
            }
        });
        actions.append(actAddNewLayer);

        auto *actDeleteLayer = new QAction(tr("Delete"));
        connect(actDeleteLayer, &QAction::triggered, this, [&]() {
            if (layer != nullptr && layer->title().compare(ivm::IVConnectionLayerType::DefaultLayerName) != 0) {
                auto cmd = new cmd::CmdConnectionLayerDelete(
                        layer->title(), m_document->layersModel(), m_document->objectsModel());
                m_document->commandsStack()->push(cmd);
            }
        });
        actions.append(actDeleteLayer);
    }

    QMenu *menu = new QMenu;
    menu->addActions(actions);
    menu->exec(ui->layerView->mapToGlobal(pos));
}

void IVAppWidget::renameSelectedLayer(QStandardItem *item)
{
    const auto index = item->index();
    const auto *obj = m_document->layersModel()->getObject(
            index.data(static_cast<int>(ive::IVVisualizationModelBase::IdRole)).toUuid());
    if (obj == nullptr) {
        return;
    }

    if (obj->type() == ivm::IVObject::Type::ConnectionLayer) {
        const auto *layer = qobject_cast<const ivm::IVConnectionLayerType *>(obj);
        if (layer != nullptr && layer->title().compare(ivm::IVConnectionLayerType::DefaultLayerName) != 0) {
            const auto itemText = ivm::IVNameValidator::encodeName(layer->type(), item->text());
            disconnect(m_document->layerVisualisationModel(), &IVVisualizationModelBase::itemChanged, this,
                    &IVAppWidget::renameSelectedLayer);
            auto *cmd = new cmd::CmdConnectionLayerRename(
                    layer->title(), itemText, m_document->layersModel(), m_document->objectsModel());
            if (cmd->layer() != nullptr) {
                m_document->commandsStack()->push(cmd);
            } else {
                delete cmd;
            }
            connect(m_document->layerVisualisationModel(), &IVVisualizationModelBase::itemChanged, this,
                    &IVAppWidget::renameSelectedLayer);
        }
    }

    item->setText(obj->titleUI());
}

void IVAppWidget::copyItems()
{
    exportToClipboard(selectedObjects());
}

void IVAppWidget::cutItems()
{
    exportToClipboard(selectedObjects());
    m_tool->removeSelectedItems();
}

void IVAppWidget::wrapItems()
{
    m_document->commandsStack()->beginMacro(tr("Wrap selected into Function"));

    /// Prepare items for serialization
    QBuffer buffer;
    if (!buffer.open(QIODevice::WriteOnly)) {
        shared::ErrorHub::addError(shared::ErrorItem::Error,
                tr("Can't open buffer for exporting: %1").arg(buffer.errorString()), QString());
        return;
    }

    QList<shared::VEObject *> objects;
    for (const QModelIndex &index : m_document->objectsSelectionModel()->selection().indexes()) {
        const int role = static_cast<int>(ive::IVVisualizationModelBase::IdRole);
        if (ivm::IVObject *object = m_document->objectsModel()->getObject(index.data(role).toUuid())) {
            objects.append(object);
        }
    }

    /// Filter out entities without parent functions selected and standalone connections
    auto it = std::remove_if(objects.begin(), objects.end(),
            [&objects, root = m_document->objectsModel()->rootObject()](const shared::VEObject *obj) {
                if (auto iObj = qobject_cast<const ivm::IVConnection *>(obj)) {
                    return !objects.contains(iObj->source()) && !objects.contains(iObj->target());
                }

                return !objects.contains(obj->parentObject()) && obj->parentObject() != root;
            });

    /// deselect excluded from wrapping items
    std::for_each(it, objects.end(), [this](const shared::VEObject *obj) {
        if (auto item = m_document->itemsModel()->getItem(obj->id()))
            item->setSelected(false);
    });
    objects.erase(it, objects.end());

    /// and collect not selected connections to be processed
    QList<shared::VEObject *> notSelectedConnections;
    std::for_each(objects.cbegin(), objects.cend(), [&](const shared::VEObject *obj) {
        if (auto function = obj->as<const ivm::IVFunction *>()) {
            QVector<ivm::IVConnection *> connections;
            for (auto iface : function->interfaces())
                connections << m_document->objectsModel()->getConnectionsForIface(iface->id());
            std::copy_if(connections.cbegin(), connections.cend(), std::back_inserter(notSelectedConnections),
                    [&](const ivm::IVConnection *connection) {
                        return std::find(objects.cbegin(), objects.cend(), connection->target()) != objects.cend()
                                || std::find(objects.cbegin(), objects.cend(), connection->source()) != objects.cend();
                    });
        }
    });
    objects << notSelectedConnections;

    /// Calculate rectangle for new function according to selected rectangular items
    static const QSet<ivm::IVObject::Type> kRectTypes { ivm::IVObject::Type::Function,
        ivm::IVObject::Type::FunctionType, ivm::IVObject::Type::Comment };
    QRectF itemSceneRect;
    for (shared::VEObject *obj : qAsConst(objects)) {
        if (auto iObj = qobject_cast<ivm::IVObject *>(obj)) {
            if (kRectTypes.contains(iObj->type())) {
                itemSceneRect |= shared::graphicsviewutils::rect(iObj->coordinates());
            }
        }
    }
    /// TODO: check if `itemSceneRect` intersects with some rectangular items in the scene
    /// then reduce `itemSceneRect` to minimal accepted size and put in the center of wrapped items

    /// Copy connection init info for those which should be splitted
    const shared::Id wrappingFunctionId = shared::createId();
    struct ConnectionCreationInfo {
        shared::Id parentId;
        shared::Id sourceId;
        shared::Id targetId;
    };
    QList<ivm::IVInterface::CreationInfo> ifacesInfos;
    QList<ConnectionCreationInfo> connectionsInfos;

    /// Remove duplications
    for (auto it = objects.begin(); it != objects.end();) {
        auto dup = std::find(std::next(it), objects.end(), *it);
        if (dup != objects.end()) {
            it = objects.erase(it);
            continue;
        }
        ++it;
    }

    for (auto objIt = objects.begin(); objIt != objects.cend();) {
        if (auto iObj = qobject_cast<const ivm::IVConnection *>(*objIt)) {
            if (!objects.contains(iObj->source())) {
                const QVector<QPointF> intersectionPoints = shared::graphicsviewutils::intersectionPoints(
                        itemSceneRect, shared::graphicsviewutils::polygon(iObj->coordinates()));
                ivm::IVInterface::CreationInfo ifaceInfo =
                        ivm::IVInterface::CreationInfo::fromIface(iObj->targetInterface());
                if (!intersectionPoints.isEmpty())
                    ifaceInfo.position = intersectionPoints.front();
                ifacesInfos.append(ifaceInfo);

                ConnectionCreationInfo outerInfo;
                outerInfo.sourceId = iObj->sourceInterface()->id();
                outerInfo.targetId = ifaceInfo.id;
                outerInfo.parentId = iObj->parentObject() ? iObj->parentObject()->id() : shared::InvalidId;
                connectionsInfos << outerInfo;

                ConnectionCreationInfo innerInfo;
                innerInfo.sourceId = ifaceInfo.id;
                innerInfo.targetId = iObj->targetInterface()->id();
                innerInfo.parentId = wrappingFunctionId;
                connectionsInfos << innerInfo;

                objIt = objects.erase(objIt);
                continue;
            } else if (!objects.contains(iObj->target())) {
                const QVector<QPointF> intersectionPoints = shared::graphicsviewutils::intersectionPoints(
                        itemSceneRect, shared::graphicsviewutils::polygon(iObj->coordinates()));
                ivm::IVInterface::CreationInfo ifaceInfo =
                        ivm::IVInterface::CreationInfo::fromIface(iObj->sourceInterface());
                if (!intersectionPoints.isEmpty())
                    ifaceInfo.position = intersectionPoints.last();
                ifacesInfos.append(ifaceInfo);

                ConnectionCreationInfo innerInfo;
                innerInfo.sourceId = iObj->sourceInterface()->id();
                innerInfo.targetId = ifaceInfo.id;
                innerInfo.parentId = wrappingFunctionId;
                connectionsInfos << innerInfo;

                ConnectionCreationInfo outerInfo;
                outerInfo.sourceId = ifaceInfo.id;
                outerInfo.targetId = iObj->targetInterface()->id();
                outerInfo.parentId = iObj->parentObject() ? iObj->parentObject()->id() : shared::InvalidId;
                connectionsInfos << outerInfo;

                objIt = objects.erase(objIt);
                continue;
            }
        }
        ++objIt;
    }
    /// Serialize prepared for wrapping items
    if (!m_document->exporter()->exportObjects(objects, &buffer, m_document->archetypesModel())) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Error during component export"));
        return;
    }
    buffer.close();

    /// Remove selected items from scene
    QList<ivm::IVObject *> entities;
    for (auto ioIt = objects.cbegin(); ioIt != objects.cend(); ++ioIt) {
        if (auto entity = (*ioIt)->as<ivm::IVObject *>()) {
            if (entity->isRootObject()) {
                continue;
            }
            if (entity->isFixedSystemElement()) {
                continue;
            }
            if (entity->isInterface()) {
                if (auto iface = entity->as<const ivm::IVInterface *>()) {
                    if (iface->isRequiredSystemElement()) {
                        continue;
                    }
                    if (auto srcIface = iface->cloneOf()) {
                        continue;
                    }
                }
            }
            entities.append(entity);
        }
    }
    auto cmdRm = new cmd::CmdEntitiesRemove(entities, m_document->objectsModel());
    cmdRm->setText(tr("Remove selected item(s)"));
    m_document->commandsStack()->push(cmdRm);

    /// Create Wrapping Function
    auto root = qobject_cast<ivm::IVFunction *>(m_document->objectsModel()->rootObject());
    auto cmd = new cmd::CmdFunctionItemCreate(
            m_document->objectsModel(), root, itemSceneRect, QString(), wrappingFunctionId);
    if (m_document->commandsStack()->push(cmd)) {
        /// Inject wrapped items into newly created Function
        ivm::IVXMLReader parser;
        if (parser.read(buffer.data())) {
            QVector<ivm::IVObject *> wrappedObjects = parser.parsedObjects();
            if (!wrappedObjects.isEmpty()) {
                ivm::IVObject::sortObjectList(wrappedObjects);
                auto cmdImport = new cmd::CmdEntitiesImport(wrappedObjects,
                        m_document->objectsModel()->getFunction(wrappingFunctionId), m_document->objectsModel(),
                        m_document->componentModel(), m_document->asn1Check(), shared::INVALID_POS);
                if (m_document->commandsStack()->push(cmdImport)) {
                    auto wrapper = m_document->objectsModel()->getObject(wrappingFunctionId);
                    Q_ASSERT(wrapper);
                    std::for_each(ifacesInfos.begin(), ifacesInfos.end(),
                            [this, fn = wrapper->as<ivm::IVFunctionType *>()](ivm::IVInterface::CreationInfo &info) {
                                info.function = fn;
                                auto cmdIfaceCreate = new cmd::CmdInterfaceItemCreate(info);
                                m_document->commandsStack()->push(cmdIfaceCreate);
                            });

                    std::for_each(
                            connectionsInfos.cbegin(), connectionsInfos.cend(), [this](const auto &connectionInfo) {
                                auto parent = m_document->objectsModel()->getObject(connectionInfo.parentId);
                                auto cmdConnectionCreate = new cmd::CmdConnectionItemCreate(m_document->objectsModel(),
                                        qobject_cast<ivm::IVFunction *>(parent), connectionInfo.sourceId,
                                        connectionInfo.targetId, {});
                                m_document->commandsStack()->push(cmdConnectionCreate);
                            });
                }
            }
        }
    }
    m_document->commandsStack()->endMacro();
}

void IVAppWidget::exportToClipboard(const QList<shared::VEObject *> &objects, QMimeData *mimeData)
{
    QBuffer buffer;
    if (!buffer.open(QIODevice::WriteOnly)) {
        shared::ErrorHub::addError(
                shared::ErrorItem::Error, tr("Can't open buffer for exporting: %1").arg(buffer.errorString()), "");
        return;
    }

    if (!m_document->exporter()->exportObjects(objects, &buffer, m_document->archetypesModel())) {
        shared::ErrorHub::addError(shared::ErrorItem::Error, tr("Error during component export"));
        return;
    }
    buffer.close();

    const QString text = QString::fromUtf8(buffer.data());
    if (mimeData)
        mimeData->setText(text);
    else
        qApp->clipboard()->setText(text);
}

void IVAppWidget::pasteItems(const QPointF &sceneDropPoint)
{
    auto mimeData = QApplication::clipboard()->mimeData();
    if (!mimeData) {
        return;
    }
    if (!mimeData->hasText()) {
        return;
    }

    QGraphicsItem *itemAtScenePos = m_document->scene()->itemAt(sceneDropPoint, graphicsView()->transform());
    while (itemAtScenePos && itemAtScenePos->type() != IVFunctionGraphicsItem::Type) {
        itemAtScenePos = itemAtScenePos->parentItem();
    }
    ivm::IVFunctionType *parentObject = gi::functionObject(itemAtScenePos);
    if (!parentObject && m_document->objectsModel() && m_document->objectsModel()->rootObject()) {
        parentObject = qobject_cast<ivm::IVFunctionType *>(m_document->objectsModel()->rootObject());
    }

    const QByteArray data = mimeData->text().toUtf8();
    ivm::IVXMLReader parser;
    if (parser.read(data)) {
        QVector<ivm::IVObject *> objects = parser.parsedObjects();
        const QHash<shared::Id, EntityAttributes> extAttrs = parser.externalAttributes();
        ivm::IVObject::sortObjectList(objects);
        for (ivm::IVObject *obj : qAsConst(objects)) {
            const EntityAttributes attrs = extAttrs.value(obj->id());
            for (auto attrIt = attrs.constBegin(); attrIt != attrs.constEnd(); ++attrIt) {
                obj->setEntityAttribute(attrIt.value());
            }
        }

        const auto existingFunctionNames = m_document->objectsModel()->nestedFunctionNames();
        const auto intersectedNames = nestedFunctionNames(objects).intersect(existingFunctionNames);
        if (!intersectedNames.isEmpty()) {
            QList<QString> intersectedNamesList;
            intersectedNamesList.reserve(intersectedNames.size());
            for (const QString &name : intersectedNames) {
                intersectedNamesList.append(name);
            }
            const QString msg = tr("Entities couldn't be pasted because of Function names conflict(s): %1")
                                        .arg(intersectedNamesList.join(QLatin1String(", ")));
            shared::ErrorHub::addError(shared::ErrorItem::Error, msg);
            return;
        }

        auto cmdImport = new cmd::CmdEntitiesImport(objects, parentObject, m_document->objectsModel(),
                m_document->componentModel(), m_document->asn1Check(), sceneDropPoint);
        m_document->commandsStack()->push(cmdImport);
    }
}

void IVAppWidget::showPropertyEditor(const shared::Id &id)
{
    Q_ASSERT(m_document);
    if (id.isNull()) {
        return;
    }

    ivm::IVObject *obj = m_document->objectsModel()->getObject(id);
    if (!obj || obj->isInterfaceGroup() || obj->isConnection()) {
        return;
    }

    auto dialog = new ive::IVPropertiesDialog(m_document,
                                              QFileInfo(m_document->path()).absolutePath(),
                                              m_document->dynPropConfig(),
                                              m_document->itemsModel()
                                                  ->getItem<shared::ui::VEInteractiveObject *>(id),
                                              m_document->layersModel(),
                                              m_document->archetypesModel(),
                                              m_document->ivCheck(),
                                              m_document->asn1Check(),
                                              m_document->commandsStack(),
                                              graphicsView());
    dialog->init();
    dialog->setModal(true);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}

void IVAppWidget::showEditAttributesDialog()
{
    shared::cmd::CommandsStackBase::Macro macro(m_document->commandsStack(), tr("Edit Attributes"));
    IVEditAttributesModel functionsModel(IVEditAttributesModel::Function, m_document->objectsModel(), &macro);
    IVEditAttributesModel interfacesModel(IVEditAttributesModel::Interface, m_document->objectsModel(), &macro);
    IVEditAttributesDialog dialog(&functionsModel, &interfacesModel, graphicsView());
    const int result = dialog.exec();
    macro.setComplete(result == QDialog::Accepted);
}

void IVAppWidget::importEntity(const shared::Id &id, QPointF sceneDropPoint)
{
    Q_ASSERT(m_document);
    auto obj = m_document->componentModel()->getObject(id);
    if (!obj) {
        return;
    }

    const auto existingFunctionNames = m_document->objectsModel()->nestedFunctionNames();
    const auto intersectedNames =
            nestedFunctionNames(obj->as<ivm::IVFunctionType *>()).intersect(existingFunctionNames);
    if (!intersectedNames.isEmpty()) {
        QList<QString> intersectedNamesList;
        intersectedNamesList.reserve(intersectedNames.size());
        for (const QString &name : intersectedNames) {
            intersectedNamesList.append(name);
        }
        const QString msg = tr("Chosen entity [%1] couldn't be imported because of Function names conflict(s): %2")
                                    .arg(obj->titleUI(), intersectedNamesList.join(QLatin1String(", ")));
        shared::ErrorHub::addError(shared::ErrorItem::Error, msg);
        return;
    }
    ivm::IVFunctionType *parentObject = functionAtPosition(sceneDropPoint);

    QList<ivm::IVObject *> objects;
    auto component = m_document->componentModel()->component(id);
    ivm::IVXMLReader parser;
    if (!parser.readFile(component->componentPath)) {
        return;
    }
    objects = parser.parsedObjects();
    ivm::IVObject::sortObjectList(objects);
    const QHash<shared::Id, EntityAttributes> extAttrs = parser.externalAttributes();
    for (auto it = extAttrs.constBegin(); it != extAttrs.constEnd(); ++it) {
        auto objIt = std::find_if(objects.constBegin(), objects.constEnd(),
                [id = it.key()](ivm::IVObject *obj) { return obj->id() == id; });
        if (objIt != objects.constEnd()) {
            for (const EntityAttribute &attr : obj->sortedAttributesValues(it.value())) {
                (*objIt)->setEntityAttribute(attr);
            }
        }
    }
    auto cmdImport = new cmd::CmdEntitiesImport(objects, parentObject, m_document->objectsModel(),
            m_document->componentModel(), m_document->asn1Check(), sceneDropPoint);
    m_document->commandsStack()->push(cmdImport);
}

void IVAppWidget::instantiateEntity(const shared::Id &id, QPointF sceneDropPoint)
{
    Q_ASSERT(m_document);
    auto obj = m_document->sharedTypesModel()->getObject(id);
    if (!obj || obj->type() != ivm::IVObject::Type::FunctionType) {
        return;
    }

    ivm::IVFunctionType *parentObject = functionAtPosition(sceneDropPoint);
    /// TODO: check if it's better to load from fs instead
    auto cmdInstantiate = new cmd::CmdEntitiesInstantiate(obj->as<ivm::IVFunctionType *>(), parentObject,
            m_document->objectsModel(), m_document->sharedTypesModel(), m_document->asn1Check(), sceneDropPoint);
    m_document->commandsStack()->push(cmdInstantiate);
}

void IVAppWidget::linkEntity(const shared::Id &id, QPointF sceneDropPoint)
{
    Q_ASSERT(m_document);
    auto obj = m_document->componentModel()->getObject(id);
    if (!obj || obj->type() != ivm::IVObject::Type::Function) {
        return;
    }

    const auto existingFunctionNames = m_document->objectsModel()->nestedFunctionNames();
    const auto intersectedNames =
            nestedFunctionNames(obj->as<ivm::IVFunctionType *>()).intersect(existingFunctionNames);
    if (!intersectedNames.isEmpty()) {
        QList<QString> intersectedNamesList;
        intersectedNamesList.reserve(intersectedNames.size());
        for (const QString &name : intersectedNames) {
            intersectedNamesList.append(name);
        }
        const QString msg =
                tr("Chosen entity [%1] couldn't be imported as reference because of Function names conflict(s): %2")
                        .arg(obj->titleUI(), intersectedNamesList.join(QLatin1String(", ")));
        shared::ErrorHub::addError(shared::ErrorItem::Error, msg);
        return;
    }

    ivm::IVFunctionType *parentObject = functionAtPosition(sceneDropPoint);

    QList<ivm::IVObject *> objects;
    auto component = m_document->componentModel()->component(id);

    ivm::IVXMLReader parser;
    if (!parser.readFile(component->componentPath)) {
        return;
    }
    objects = parser.parsedObjects();
    ivm::IVObject::sortObjectList(objects);
    const QHash<shared::Id, EntityAttributes> extAttrs = parser.externalAttributes();
    for (auto it = extAttrs.constBegin(); it != extAttrs.constEnd(); ++it) {
        auto objIt = std::find_if(objects.constBegin(), objects.constEnd(),
                [id = it.key()](ivm::IVObject *obj) { return obj->id() == id; });
        if (objIt != objects.constEnd()) {
            for (const EntityAttribute &attr : obj->sortedAttributesValues(it.value())) {
                (*objIt)->setEntityAttribute(attr);
            }
        }
    }
    std::for_each(objects.begin(), objects.end(), [](shared::VEObject *obj) {
        obj->setEntityAttribute(ivm::meta::Props::token(ivm::meta::Props::Token::reference), true);
    });

    auto cmdLink = new cmd::CmdEntitiesImport(objects, parentObject, m_document->objectsModel(),
            m_document->componentModel(), m_document->asn1Check(), sceneDropPoint);
    m_document->commandsStack()->push(cmdLink);
}

void IVAppWidget::enterNestedView(const shared::Id &id)
{
    if (id.isNull()) {
        return;
    }

    if (auto entity = m_document->objectsModel()->getObject(id)) {
        if (entity->isFunction()) {
            m_document->itemsModel()->changeRootItem(id);
        }
    }
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

void IVAppWidget::onRootObjectChanged(const shared::Id &rootId)
{
    QList<shared::BreadcrumbWidget::Level> levels;
    if (ivm::IVObject *object = m_document->objectsModel()->getObject(rootId)) {
        Q_ASSERT(object->isFunction());
        auto function = object->as<const ivm::IVFunctionType *>();
        if (!function)
            return;
        levels.append({ function->id().toString(), function->titleUI() });
        while ((function = function->parentObject()->as<const ivm::IVFunctionType *>())) {
            levels.prepend({ function->id().toString(), function->titleUI() });
        }
    }
    m_breadCrumb->setLevels(levels);

    if (m_actExitToRoot) {
        m_actExitToRoot->setEnabled(nullptr != m_document->objectsModel()->rootObject());
    }
    if (m_actExitToParent) {
        m_actExitToParent->setEnabled(nullptr != m_document->objectsModel()->rootObject());
    }
    centerView();
}

void IVAppWidget::checkActionsFromSelection()
{
    const QModelIndexList idxs = m_document->objectsSelectionModel()->selectedIndexes();
    m_actRemove->setEnabled(!idxs.isEmpty());
    auto it = std::find_if(idxs.cbegin(), idxs.cend(), [](const QModelIndex &index) {
        return index.data(static_cast<int>(ive::IVVisualizationModelBase::TypeRole)).toInt()
                == static_cast<int>(ivm::IVObject::Type::Connection);
    });
    const bool functionsOnly = std::all_of(idxs.cbegin(), idxs.cend(), [](const QModelIndex &index) {
        return index.data(static_cast<int>(ive::IVVisualizationModelBase::TypeRole)).toInt()
                == static_cast<int>(ivm::IVObject::Type::Function);
    });
    m_actCreateConnectionGroup->setEnabled((it != std::cend(idxs)) || (functionsOnly && idxs.size() == 2));

    it = std::find_if(idxs.cbegin(), idxs.cend(), [](const QModelIndex &index) {
        return index.data(static_cast<int>(ive::IVVisualizationModelBase::TypeRole)).toInt()
                == static_cast<int>(ivm::IVObject::Type::ConnectionGroup);
    });
    m_actUngroupConnection->setEnabled(it != std::cend(idxs));

    const auto count = std::count_if(idxs.cbegin(), idxs.cend(), [](const QModelIndex &index) {
        return index.data(static_cast<int>(ive::IVVisualizationModelBase::TypeRole)).toInt()
                == static_cast<int>(ivm::IVObject::Type::Function);
    });
    m_actEnterNestedView->setEnabled(count == 1);
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

    connect(ui->graphicsView, &GraphicsView::importEntity, this, &IVAppWidget::importEntity, Qt::QueuedConnection);
    connect(ui->graphicsView, &GraphicsView::instantiateEntity, this, &IVAppWidget::instantiateEntity,
            Qt::QueuedConnection);
    connect(ui->graphicsView, &GraphicsView::copyItems, this, &IVAppWidget::copyItems);
    connect(ui->graphicsView, &GraphicsView::cutItems, this, &IVAppWidget::cutItems);
    connect(ui->graphicsView, &GraphicsView::pasteItems, this, qOverload<>(&IVAppWidget::pasteItems));

    ui->graphicsView->setScene(m_document->scene());
    ui->graphicsView->setUpdatesEnabled(false);
    m_document->itemsModel()->updateSceneRect();
    ui->graphicsView->setUpdatesEnabled(true);
}
/**
 * Init modelView aka. IV Structure aka. objectsView
 */
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
    ui->importView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    ui->importView->setModel(m_document->componentModel());
    connect(ui->importView, &QTreeView::customContextMenuRequested, this,
            &IVAppWidget::showContextMenuForComponentsLibraryView);
}

void IVAppWidget::initSharedView()
{
    ui->sharedView->setObjectName(QLatin1String("SharedView"));
    ui->sharedView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    ui->sharedView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->sharedView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
    ui->sharedView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    ui->sharedView->setModel(m_document->sharedTypesModel());
    connect(ui->sharedView, &QTreeView::customContextMenuRequested, this,
            &IVAppWidget::showContextMenuForSharedTypesView);
}

void IVAppWidget::initLayerView()
{
    ui->layerView->setObjectName(QLatin1String("Connection Layers"));
    ui->layerView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);
    ui->layerView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    ui->layerView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    ui->layerView->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);
    ui->layerView->setSortingEnabled(true);
    connect(ui->layerView, &QTreeView::customContextMenuRequested, this, &IVAppWidget::showAvailableLayers);
    connect(m_document->layerVisualisationModel(), &IVVisualizationModelBase::itemChanged, this,
            &IVAppWidget::renameSelectedLayer);
    ui->layerView->setModel(m_document->layerVisualisationModel());
}

/**
 *  creates the actions in the top part of the toolbar left of the scene
 */
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
    connect(m_tool, &IVCreatorTool::nestedViewRequest, this, &IVAppWidget::enterNestedView, Qt::QueuedConnection);
    connect(m_tool, &IVCreatorTool::informUser, m_document.data(), &InterfaceDocument::showInfoMessage);
    connect(m_tool, &IVCreatorTool::copyActionTriggered, this, &IVAppWidget::copyItems);
    connect(m_tool, &IVCreatorTool::cutActionTriggered, this, &IVAppWidget::cutItems);
    connect(m_tool, &IVCreatorTool::wrapSelectedItemsTriggered, this, &IVAppWidget::wrapItems);
    connect(m_tool, &IVCreatorTool::pasteActionTriggered, this, qOverload<const QPointF &>(&IVAppWidget::pasteItems));

    auto actCreateFunctionType = new QAction(tr("Function Type"));
    ActionsManager::registerAction(Q_FUNC_INFO, actCreateFunctionType, "Function Type", "Create FunctionType object");
    actCreateFunctionType->setCheckable(true);
    actCreateFunctionType->setActionGroup(actionGroup);
    connect(actCreateFunctionType, &QAction::triggered, this,
            [this]() { m_tool->setCurrentToolType(IVCreatorTool::ToolType::FunctionType); });
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

    m_actUngroupConnection = new QAction(tr("Ungroup selected connection(s)"));
    ActionsManager::registerAction(Q_FUNC_INFO, m_actUngroupConnection, "Ungroup Connections", "Ungroup connections");
    m_actUngroupConnection->setActionGroup(actionGroup);
    connect(m_actUngroupConnection, &QAction::triggered, this, [this]() { m_tool->ungroupConnectedItems(); });
    m_actUngroupConnection->setIcon(QIcon(":/toolbar/icns/ungroup_connections.svg"));

    m_actEditAttributes = new QAction(tr("Edit Attributes"));
    ActionsManager::registerAction(Q_FUNC_INFO, m_actEditAttributes, "Edit", "Edit Attributes");
    m_actEditAttributes->setActionGroup(actionGroup);
    m_actEditAttributes->setIcon(QIcon(QLatin1String(":/toolbar/icns/edit.svg")));
    connect(m_actEditAttributes, &QAction::triggered, this, [this]() { showEditAttributesDialog(); });

    m_actRemove = new QAction(tr("Remove"));
    ActionsManager::registerAction(Q_FUNC_INFO, m_actRemove, "Remove", "Remove selected object");
    m_actRemove->setIcon(QIcon(QLatin1String(":/toolbar/icns/delete.svg")));
    m_actRemove->setEnabled(false);
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

    m_actExitToRoot = new QAction(tr("Exit to root function"), this);
    m_actExitToRoot->setActionGroup(actionGroup);
    m_actExitToRoot->setEnabled(false);
    connect(m_actExitToRoot, &QAction::triggered, this, [this]() { m_document->itemsModel()->changeRootItem({}); });
    m_actExitToRoot->setIcon(QIcon(":/toolbar/icns/exit.svg"));

    m_actExitToParent = new QAction(tr("Exit to parent function"), this);
    m_actExitToParent->setActionGroup(actionGroup);
    m_actExitToParent->setEnabled(false);
    connect(m_actExitToParent, &QAction::triggered, this, [this]() {
        ivm::IVObject *parentObject = m_document->objectsModel()->rootObject()
                ? m_document->objectsModel()->rootObject()->parentObject()
                : nullptr;
        m_document->itemsModel()->changeRootItem(parentObject ? parentObject->id() : shared::InvalidId);
    });
    m_actExitToParent->setIcon(QIcon(":/toolbar/icns/exit_parent.svg"));

    m_actEnterNestedView = new QAction(tr("Enter function nested view"), this);
    m_actEnterNestedView->setActionGroup(actionGroup);
    m_actEnterNestedView->setEnabled(false);
    m_actEnterNestedView->setIcon(QIcon(QLatin1String(":/toolbar/icns/nested_view.svg")));
    connect(m_actEnterNestedView, &QAction::triggered, this, [this]() {
        const QModelIndexList idxs = m_document->objectsSelectionModel()->selectedIndexes();
        const auto it = std::find_if(idxs.cbegin(), idxs.cend(), [](const QModelIndex &index) {
            return index.data(static_cast<int>(ive::IVVisualizationModelBase::TypeRole)).toInt()
                    == static_cast<int>(ivm::IVObject::Type::Function);
        });
        if (it != idxs.cend())
            enterNestedView(it->data(static_cast<int>(ive::IVVisualizationModelBase::IdRole)).toUuid());
    });

    m_actShrinkScene = new QAction(tr("Shrink scene"), this);
    m_actShrinkScene->setActionGroup(actionGroup);
    m_actShrinkScene->setEnabled(true);
    m_actShrinkScene->setIcon(QIcon(QLatin1String(":/toolbar/icns/shrink.svg")));
    connect(m_actShrinkScene, &QAction::triggered, this, [this]() {
        m_document->itemsModel()->shrinkScene();
        graphicsView()->centerOn(m_document->scene()->sceneRect().center());
    });

    m_toolbarActions = { actCreateFunctionType, actCreateFunction, actCreateProvidedInterface,
        actCreateRequiredInterface, actCreateComment, actCreateConnection, m_actCreateConnectionGroup,
        m_actUngroupConnection, m_actEditAttributes, m_actRemove, m_actZoomIn, m_actZoomOut, m_actExitToRoot,
        m_actExitToParent, m_actEnterNestedView, m_actShrinkScene };

    connect(m_document->objectsModel(), &ivm::IVModel::rootObjectChanged, this, &IVAppWidget::onRootObjectChanged);
    connect(m_document->objectsSelectionModel(), &QItemSelectionModel::selectionChanged, this,
            &IVAppWidget::checkActionsFromSelection);
    checkActionsFromSelection();

    return m_toolbarActions;
}

// Creates the actions in the bottom part of the toolbar left of the scene
QVector<QAction *> IVAppWidget::initViewActions()
{
    if (!m_viewActions.isEmpty() || !m_ivCore) {
        return m_viewActions;
    }

    m_viewActions.append(m_ivCore->actionToggleMinimap());
    m_viewActions.append(m_ivCore->actionToggleE2EView());
    m_viewActions.append(m_ivCore->actionExportFunctions());
    m_viewActions.append(m_ivCore->actionExportType());
    m_viewActions.append(m_ivCore->actionLaunchModelCheckingWindow());

    auto actionSaveSceneRender =
            new QAction(QIcon(QLatin1String(":/toolbar/icns/render.svg")), tr("Render Scene..."), this);
    ive::ActionsManager::registerAction(
            Q_FUNC_INFO, actionSaveSceneRender, "Render", "Save current scene complete render.");
    connect(actionSaveSceneRender, &QAction::triggered, this, [this]() { m_ivCore->onSaveRenderRequested(); });
    m_viewActions.append(actionSaveSceneRender);

    return m_viewActions;
}

QList<shared::VEObject *> IVAppWidget::selectedObjects() const
{
    QList<shared::VEObject *> objects;
    for (const QModelIndex &index : m_document->objectsSelectionModel()->selection().indexes()) {
        const int role = static_cast<int>(ive::IVVisualizationModelBase::IdRole);
        const shared::Id id = index.data(role).toUuid();
        if (ivm::IVObject *object = m_document->objectsModel()->getObject(id)) {
            objects.append(object);
        }
    }
    return objects;
}

void IVAppWidget::showArchetypeManager()
{
    if (m_document == nullptr || m_document->objectsModel() == nullptr || m_document->archetypesModel() == nullptr) {
        return;
    }

    ive::ArchetypesManagerDialog dialog(m_document, m_document->objectsModel(), m_document->commandsStack(), this);
    dialog.init();
    dialog.exec();
}

/*!
 * Returns the function(type) that is below the given scene position \p screneDropPoint
 * Does eventually update the scene \p screneDropPoint
 */
ivm::IVFunctionType *IVAppWidget::functionAtPosition(QPointF &sceneDropPoint)
{
    ivm::IVFunctionType *parentObject = nullptr;
    if (shared::isValidPosition(sceneDropPoint)) {
        QGraphicsItem *itemAtScenePos = m_document->scene()->itemAt(sceneDropPoint, graphicsView()->transform());
        while (itemAtScenePos && itemAtScenePos->type() != IVFunctionGraphicsItem::Type) {
            itemAtScenePos = itemAtScenePos->parentItem();
        }
        parentObject = gi::functionObject(itemAtScenePos);
    } else {
        parentObject = dynamic_cast<ivm::IVFunctionType *>(m_document->objectsModel()->rootObject());

        /// TODO: find proper place for imporing function in the scene if sceneDropPoint invalid (for example action
        /// from context menu is invoked)
        sceneDropPoint = QPoint();
    }

    return parentObject;
}

} // namespace ive
