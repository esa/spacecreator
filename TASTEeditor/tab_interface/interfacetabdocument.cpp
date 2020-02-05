/*
  Copyright (C) 2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "interfacetabdocument.h"

#include "app/context/action/editor/dynactioneditor.h"
#include "baseitems/clicknotifieritem.h"
#include "baseitems/common/utils.h"
#include "baseitems/graphicsview.h"
#include "creatortool.h"
#include "interfacetabgraphicsscene.h"
#include "tab_aadl/aadlobjectsmodel.h"
#include "tab_aadl/aadlxmlreader.h"
#include "tab_interface/aadlcommentgraphicsitem.h"
#include "tab_interface/aadlconnectiongraphicsitem.h"
#include "tab_interface/aadlfunctiongraphicsitem.h"
#include "tab_interface/aadlfunctiontypegraphicsitem.h"
#include "tab_interface/aadlinterfacegraphicsitem.h"
#include "tab_interface/colors/colormanagerdialog.h"
#include "tab_interface/properties/dynamicpropertymanager.h"
#include "tab_interface/properties/propertiesdialog.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QMenu>
#include <QMessageBox>

#define WARN_NOT_IMPLEMENTED qWarning() << Q_FUNC_INFO << "Not implemented yet."

namespace taste3 {
namespace document {

InterfaceTabDocument::InterfaceTabDocument(QObject *parent)
    : AbstractTabDocument(parent)
    , m_model(new aadl::AADLObjectsModel(this))
{
    connect(m_model, &aadl::AADLObjectsModel::modelReset, this, &InterfaceTabDocument::clearScene);
    connect(m_model, &aadl::AADLObjectsModel::aadlObjectAdded, this, &InterfaceTabDocument::onAADLObjectAdded);
    connect(m_model, &aadl::AADLObjectsModel::aadlObjectRemoved, this, [this](aadl::AADLObject *object) {
        auto item = m_items.take(object->id());
        m_graphicsScene->removeItem(item);
        delete item;
    });
}

InterfaceTabDocument::~InterfaceTabDocument()
{
    if (m_graphicsView && !m_graphicsView->parent())
        delete m_graphicsView;
}

QWidget *InterfaceTabDocument::createView()
{
    if (!m_graphicsView) {
        m_graphicsView = new GraphicsView;
        connect(m_graphicsView, &GraphicsView::zoomChanged, this, [this](qreal percent) {
            for (auto item : m_graphicsScene->selectedItems()) {
                if (auto connection = qgraphicsitem_cast<aadl::AADLConnectionGraphicsItem *>(item)) {
                    connection->updateGripPoints();
                } else if (auto iObj = qobject_cast<InteractiveObject *>(item->toGraphicsObject())) {
                    iObj->updateGripPoints();
                }
            }
            for (auto item : m_graphicsScene->items())
                if (auto function = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(item))
                    function->instantLayoutUpdate();
            m_actZoomIn->setEnabled(!qFuzzyCompare(percent, m_graphicsView->maxZoomPercent()));
            m_actZoomOut->setEnabled(!qFuzzyCompare(percent, m_graphicsView->minZoomPercent()));
        });
    }
    m_graphicsView->setScene(m_graphicsScene);
    m_graphicsView->setSceneRect(
            QRectF(QPointF(0, 0), m_graphicsView->size() * m_graphicsView->maxZoomPercent() / 100));
    return m_graphicsView;
}

QGraphicsScene *InterfaceTabDocument::createScene()
{
    if (!m_graphicsScene)
        m_graphicsScene = new InterfaceTabGraphicsScene(this);

    return m_graphicsScene;
}

QString InterfaceTabDocument::title() const
{
    return tr("Interface");
}

QMenu *InterfaceTabDocument::customMenu() const
{
    QMenu *root = new QMenu(title());
    QAction *actCommonProps = root->addAction(tr("Common Properties"));
    connect(actCommonProps, &QAction::triggered, this, &InterfaceTabDocument::onAttributesManagerRequested);
    QAction *actDataTypes = root->addAction(tr("Data types"));
    connect(actDataTypes, &QAction::triggered, this, &InterfaceTabDocument::onDataTypesMenuInvoked);
    QAction *actColorScheme = root->addAction(tr("Color Scheme"));
    connect(actColorScheme, &QAction::triggered, this, &InterfaceTabDocument::onColorSchemeMenuInvoked);
    QAction *actDynContext = root->addAction(tr("Context Actions"));
    connect(actDynContext, &QAction::triggered, this, &InterfaceTabDocument::onDynContextEditorMenuInvoked);

    common::registerAction(Q_FUNC_INFO, actCommonProps, "Edit Properties", "Show editor for common Properties");
    common::registerAction(Q_FUNC_INFO, actDataTypes, "Edit Datatypes", "Show editor for common Datatypes");
    common::registerAction(Q_FUNC_INFO, actColorScheme, "Edit Color scheme", "Show editor for common Color schemes");
    common::registerAction(Q_FUNC_INFO, actDynContext, "Context actions",
                           "Show editor for common custom context menu actions");

    return root;
}

const QHash<common::Id, aadl::AADLObject *> &InterfaceTabDocument::objects() const
{
    return m_model->objects();
}

bool InterfaceTabDocument::loadImpl(const QString &path)
{
    if (path.isEmpty() || !QFileInfo(path).exists()) {
        qWarning() << Q_FUNC_INFO << "Invalid path";
        return false;
    }

    aadl::AADLXMLReader parser;
    connect(&parser, &aadl::AADLXMLReader::objectsParsed, this, [this](const QVector<aadl::AADLObject *> &objects) {
        clearScene();
        if (m_model->initFromObjects(objects)) {
            if (auto rootFunctionItem = rootItem()) {
                const QRectF childrenRect = rootFunctionItem->mapRectToScene(rootFunctionItem->childrenBoundingRect());
                m_graphicsView->centerOn(childrenRect.center());
            }
        }
    });
    connect(&parser, &aadl::AADLXMLReader::error, [](const QString &msg) { qWarning() << msg; });

    return parser.parse(path);
}

bool InterfaceTabDocument::saveImpl(const QString & /*path*/)
{
    return false;
}

QVector<QAction *> InterfaceTabDocument::initActions()
{
    if (!m_tool) {
        m_tool = new aadl::CreatorTool(m_graphicsView, m_model, this);
        connect(m_tool, &aadl::CreatorTool::created, this, [this]() {
            if (QAction *currentAction = m_actionGroup->checkedAction())
                currentAction->setChecked(false);
            m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::Pointer);
        });
        connect(m_tool, &aadl::CreatorTool::propertyEditorRequest, this, &InterfaceTabDocument::showPropertyEditor);
    }

    if (!m_actionGroup) {
        m_actionGroup = new QActionGroup(this);
        m_actionGroup->setExclusive(true);
    }

    if (!m_actCreateFunctionType) {
        m_actCreateFunctionType = new QAction(tr("Function Type"));
        common::registerAction(Q_FUNC_INFO, m_actCreateFunctionType, "Function Type", "Create FunctionType object");

        m_actCreateFunctionType->setCheckable(true);
        m_actCreateFunctionType->setActionGroup(m_actionGroup);
        connect(m_actCreateFunctionType, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateFunctionType);
        m_actCreateFunctionType->setIcon(QIcon(":/tab_interface/toolbar/icns/function_type.svg"));
    }

    if (!m_actCreateFunction) {
        m_actCreateFunction = new QAction(tr("Function"));
        common::registerAction(Q_FUNC_INFO, m_actCreateFunction, "Function", "Create Function object");

        m_actCreateFunction->setCheckable(true);
        m_actCreateFunction->setActionGroup(m_actionGroup);
        connect(m_actCreateFunction, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateFunction);

        m_actCreateFunction->setIcon(QIcon(":/tab_interface/toolbar/icns/function.svg"));
    }

    if (!m_actCreateProvidedInterface) {
        m_actCreateProvidedInterface = new QAction(tr("Provided Interface"));
        common::registerAction(Q_FUNC_INFO, m_actCreateProvidedInterface, "Provided Interface",
                               "Create Provided Interface object");

        m_actCreateProvidedInterface->setCheckable(true);
        m_actCreateProvidedInterface->setActionGroup(m_actionGroup);
        connect(m_actCreateProvidedInterface, &QAction::triggered, this,
                &InterfaceTabDocument::onActionCreateProvidedInterface);
        m_actCreateProvidedInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/pi.svg"));
    }

    if (!m_actCreateRequiredInterface) {
        m_actCreateRequiredInterface = new QAction(tr("Required Interface"));
        common::registerAction(Q_FUNC_INFO, m_actCreateRequiredInterface, "Required Interface",
                               "Create Required Interface object");

        m_actCreateRequiredInterface->setCheckable(true);
        m_actCreateRequiredInterface->setActionGroup(m_actionGroup);
        connect(m_actCreateRequiredInterface, &QAction::triggered, this,
                &InterfaceTabDocument::onActionCreateRequiredInterface);
        m_actCreateRequiredInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/ri.svg"));
    }

    if (!m_actCreateComment) {
        m_actCreateComment = new QAction(tr("Comment"));
        common::registerAction(Q_FUNC_INFO, m_actCreateComment, "Comment", "Create Comment object");

        m_actCreateComment->setCheckable(true);
        m_actCreateComment->setActionGroup(m_actionGroup);
        connect(m_actCreateComment, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateComment);
        m_actCreateComment->setIcon(QIcon(":/tab_interface/toolbar/icns/comment.svg"));
    }

    //    if (!m_actGroupConnections) {
    //        m_actGroupConnections = new QAction(tr("Group"));
    //        m_actGroupConnections->setCheckable(true);
    //        m_actGroupConnections->setActionGroup(m_actionGroup);
    //        connect(m_actGroupConnections, &QAction::triggered, this,
    //        &InterfaceTabDocument::onActionGroupConnections);
    //        m_actGroupConnections->setIcon(QIcon(":/tab_interface/toolbar/icns/group.svg"));
    //    }

    if (!m_actCreateConnection) {
        m_actCreateConnection = new QAction(tr("Connection"));
        common::registerAction(Q_FUNC_INFO, m_actCreateConnection, "Connection", "Create Connection object");

        m_actCreateConnection->setCheckable(true);
        m_actCreateConnection->setActionGroup(m_actionGroup);
        connect(m_actCreateConnection, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateConnection);
        m_actCreateConnection->setIcon(QIcon(":/tab_interface/toolbar/icns/connection.svg"));
    }

    if (!m_actRemove) {
        m_actRemove = new QAction(tr("Remove"));
        common::registerAction(Q_FUNC_INFO, m_actRemove, "Remove", "Remove selected object");

        m_actRemove->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/remove.svg")));
        m_actRemove->setShortcut(QKeySequence::Delete);
        connect(m_actRemove, &QAction::triggered, this, &InterfaceTabDocument::onActionRemoveItem);
    }

    if (!m_actZoomIn) {
        m_actZoomIn = new QAction(tr("Zoom In"));
        common::registerAction(Q_FUNC_INFO, m_actZoomIn, "Zoom In", "Scale up the current scene");

        m_actZoomIn->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/zoom_in.svg")));
        m_actZoomIn->setShortcut(QKeySequence::ZoomIn);
        connect(m_actZoomIn, &QAction::triggered, this, &InterfaceTabDocument::onActionZoomIn);
    }

    if (!m_actZoomOut) {
        m_actZoomOut = new QAction(tr("Zoom Out"));
        common::registerAction(Q_FUNC_INFO, m_actZoomOut, "Zoom Out", "Scale down the current scene");

        m_actZoomOut->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/zoom_out.svg")));
        m_actZoomOut->setShortcut(QKeySequence::ZoomOut);
        connect(m_actZoomOut, &QAction::triggered, this, &InterfaceTabDocument::onActionZoomOut);
    }

    if (!m_actExitToRoot) {
        m_actExitToRoot = new QAction(tr("Exit to root funtion"));
        m_actExitToRoot->setActionGroup(m_actionGroup);
        m_actExitToRoot->setVisible(false);
        connect(m_actExitToRoot, &QAction::triggered, this, &InterfaceTabDocument::onActionExitToRootFunction);
        m_actExitToRoot->setIcon(QIcon(":/tab_interface/toolbar/icns/exit.svg"));
    }

    if (!m_actExitToParent) {
        m_actExitToParent = new QAction(tr("Exit to parent function"));
        m_actExitToParent->setActionGroup(m_actionGroup);
        m_actExitToParent->setVisible(false);
        connect(m_actExitToParent, &QAction::triggered, this, &InterfaceTabDocument::onActionExitToParentFunction);
        m_actExitToParent->setIcon(QIcon(":/tab_interface/toolbar/icns/exit_parent.svg"));
    }

    return { m_actCreateFunctionType,
             m_actCreateFunction,
             m_actCreateProvidedInterface,
             m_actCreateRequiredInterface,
             m_actCreateComment,
             m_actCreateConnection,
             m_actRemove,
             m_actZoomIn,
             m_actZoomOut,
             m_actExitToRoot,
             m_actExitToParent };
}

void InterfaceTabDocument::onActionCreateFunctionType()
{
    m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::FunctionType);
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateFunction()
{
    m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::Function);
}

void InterfaceTabDocument::onActionCreateProvidedInterface()
{
    m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::ProvidedInterface);
}

void InterfaceTabDocument::onActionCreateRequiredInterface()
{
    m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::RequiredInterface);
}

void InterfaceTabDocument::onActionCreateComment()
{
    m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::Comment);
}

void InterfaceTabDocument::onActionGroupConnections()
{
    WARN_NOT_IMPLEMENTED;
}

void InterfaceTabDocument::onActionCreateConnection()
{
    m_tool->setCurrentToolType(aadl::CreatorTool::ToolType::MultiPointConnection);
}

void InterfaceTabDocument::onActionRemoveItem()
{
    m_tool->removeSelectedItems();
}

void InterfaceTabDocument::onActionZoomIn()
{
    m_graphicsView->setZoom(m_graphicsView->zoom() + m_graphicsView->zoomStepPercent());
}

void InterfaceTabDocument::onActionZoomOut()
{
    m_graphicsView->setZoom(m_graphicsView->zoom() - m_graphicsView->zoomStepPercent());
}

void InterfaceTabDocument::onActionExitToRootFunction()
{
    onRootItemChanged({});
}

void InterfaceTabDocument::onActionExitToParentFunction()
{
    aadl::AADLObject *parentObject = m_model->rootObject() ? m_model->rootObject()->parentObject() : nullptr;
    onRootItemChanged(parentObject ? parentObject->id() : common::Id());
}

void InterfaceTabDocument::updateItem(QGraphicsItem *item)
{
    Q_ASSERT(item);
    if (!item)
        return;

    switch (item->type()) {
    case aadl::AADLCommentGraphicsItem::Type:
        updateComment(qgraphicsitem_cast<aadl::AADLCommentGraphicsItem *>(item));
        break;
    case aadl::AADLConnectionGraphicsItem::Type:
        updateConnection(qgraphicsitem_cast<aadl::AADLConnectionGraphicsItem *>(item));
        break;
    case aadl::AADLFunctionGraphicsItem::Type:
        updateFunction(qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(item));
        break;
    case aadl::AADLFunctionTypeGraphicsItem::Type:
        updateFunctionType(qgraphicsitem_cast<aadl::AADLFunctionTypeGraphicsItem *>(item));
        break;
    case aadl::AADLInterfaceGraphicsItem::Type:
        updateInterface(qgraphicsitem_cast<aadl::AADLInterfaceGraphicsItem *>(item));
        break;
    default:
        return;
    }
}

void InterfaceTabDocument::updateComment(aadl::AADLCommentGraphicsItem *comment)
{
    comment->updateFromEntity();
}

void InterfaceTabDocument::updateInterface(aadl::AADLInterfaceGraphicsItem *iface)
{
    iface->updateFromEntity();
}

void InterfaceTabDocument::updateFunction(aadl::AADLFunctionGraphicsItem *function)
{
    function->updateFromEntity();
}

void InterfaceTabDocument::updateFunctionType(aadl::AADLFunctionTypeGraphicsItem *functionType)
{
    functionType->updateFromEntity();
}

void InterfaceTabDocument::updateConnection(aadl::AADLConnectionGraphicsItem *connection)
{
    connection->updateFromEntity();
}

QGraphicsItem *InterfaceTabDocument::createItemForObject(aadl::AADLObject *obj)
{
    Q_ASSERT(obj);
    if (!obj)
        return nullptr;

    QGraphicsItem *parentItem = obj->parentObject() ? m_items.value(obj->parentObject()->id()) : nullptr;

    switch (obj->aadlType()) {
    case aadl::AADLObject::AADLObjectType::AADLComment:
        return new aadl::AADLCommentGraphicsItem(qobject_cast<aadl::AADLObjectComment *>(obj), parentItem);
    case aadl::AADLObject::AADLObjectType::AADLIface:
        return new aadl::AADLInterfaceGraphicsItem(qobject_cast<aadl::AADLObjectIface *>(obj), parentItem);
    case aadl::AADLObject::AADLObjectType::AADLConnection:
        if (auto connection = qobject_cast<aadl::AADLObjectConnection *>(obj)) {
            aadl::AADLObjectIface *ifaceStart = connection->sourceInterface();
            auto startItem = qgraphicsitem_cast<aadl::AADLInterfaceGraphicsItem *>(
                    ifaceStart ? m_items.value(ifaceStart->id()) : nullptr);

            aadl::AADLObjectIface *ifaceEnd = connection->targetInterface();
            auto endItem = qgraphicsitem_cast<aadl::AADLInterfaceGraphicsItem *>(
                    ifaceEnd ? m_items.value(ifaceEnd->id()) : nullptr);

            return new aadl::AADLConnectionGraphicsItem(connection, startItem, endItem, parentItem);
        }
        break;
    case aadl::AADLObject::AADLObjectType::AADLFunction:
        return new aadl::AADLFunctionGraphicsItem(qobject_cast<aadl::AADLObjectFunction *>(obj), parentItem);
    case aadl::AADLObject::AADLObjectType::AADLFunctionType:
        return new aadl::AADLFunctionTypeGraphicsItem(qobject_cast<aadl::AADLObjectFunctionType *>(obj), parentItem);
    default: {
        qCritical() << "Unknown object type:" << obj->aadlType();
        break;
    }
    }

    return nullptr;
}

aadl::AADLFunctionGraphicsItem *InterfaceTabDocument::rootItem() const
{
    if (auto rootEntity = m_model->rootObject())
        return qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(m_items.value(rootEntity->id()));
    return nullptr;
}

void InterfaceTabDocument::onAADLObjectAdded(aadl::AADLObject *object)
{
    auto adjustRootItem = [this](aadl::AADLObject *object) {
        if (object->isRootObject())
            return;

        switch (object->aadlType()) {
        case aadl::AADLObject::AADLObjectType::AADLFunction:
        case aadl::AADLObject::AADLObjectType::AADLFunctionType:
            if (auto rootFunctionItem = rootItem()) {
                const QRectF nestedRect = rootFunctionItem->nestedItemsSceneBoundingRect();
                if (nestedRect.isValid()) {
                    const QRectF rootRect = rootFunctionItem->sceneBoundingRect();
                    const QRectF adjustedNestedRect = nestedRect.marginsAdded(utils::kContentMargins);
                    if (!rootRect.contains(adjustedNestedRect)) {
                        rootFunctionItem->setRect(rootRect.united(adjustedNestedRect));
                        for (auto innerItem : rootFunctionItem->childItems()) {
                            if (auto innerFunction = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(innerItem))
                                innerFunction->updateFromEntity();
                        }
                    }
                }
            }
            break;
        default:
            break;
        }
    };

    auto propertyChanged = [this]() {
        if (auto object = qobject_cast<aadl::AADLObject *>(sender())) {
            if (auto item = m_items.value(object->id()))
                updateItem(item);
        }
    };

    auto item = m_items.value(object->id());

    if (!item) {
        item = createItemForObject(object);
        connect(object, &aadl::AADLObject::coordinatesChanged, this, propertyChanged);
        connect(object, &aadl::AADLObject::innerCoordinatesChanged, this, propertyChanged);
        connect(object, &aadl::AADLObject::titleChanged, this, propertyChanged);
        if (auto clickable = qobject_cast<ClickNotifierItem *>(item->toGraphicsObject())) {
            connect(clickable, &ClickNotifierItem::clicked, this, &InterfaceTabDocument::onItemClicked);
            connect(clickable, &ClickNotifierItem::doubleClicked, this, &InterfaceTabDocument::onItemDoubleClicked,
                    Qt::QueuedConnection);
        }
        m_items.insert(object->id(), item);
        if (m_graphicsScene != item->scene())
            m_graphicsScene->addItem(item);
    }
    adjustRootItem(object);
    updateItem(item);
}

void InterfaceTabDocument::onItemClicked()
{
    if (auto iface = qobject_cast<aadl::AADLInterfaceGraphicsItem *>(sender())) {
        qDebug() << "Graphics Iface geometry:" << iface->scenePos() << iface->sceneBoundingRect();
        qDebug() << "Internal Iface data:" << iface->entity()->title() << utils::pos(iface->entity()->coordinates())
                 << utils::pos(iface->entity()->innerCoordinates());
    } else if (auto connection = qobject_cast<aadl::AADLConnectionGraphicsItem *>(sender())) {
        qDebug() << "Graphics Connection geometry:" << connection->points() << connection->currentPoints();
        qDebug() << "Internal Connection data:"
                 << (connection->entity()->title().isEmpty()
                             ? QStringLiteral("Connection %1<>%2")
                                       .arg(connection->startItem()->entity()->interfaceName(),
                                            connection->endItem()->entity()->interfaceName())
                             : connection->entity()->title())
                 << utils::polygon(connection->entity()->coordinates())
                 << utils::polygon(connection->entity()->innerCoordinates());
    } else if (auto function = qobject_cast<aadl::AADLFunctionGraphicsItem *>(sender())) {
        qDebug() << "Graphics Function geometry:" << function->sceneBoundingRect();
        qDebug() << "Internal Function data:" << function->entity()->title()
                 << utils::rect(function->entity()->coordinates())
                 << utils::rect(function->entity()->innerCoordinates());
    } else if (auto functionType = qobject_cast<aadl::AADLFunctionTypeGraphicsItem *>(sender())) {
        qDebug() << "Graphics FunctionType geometry:" << functionType->sceneBoundingRect();
        qDebug() << "Internal FunctionType data:" << functionType->entity()->title()
                 << utils::rect(functionType->entity()->coordinates())
                 << utils::rect(functionType->entity()->innerCoordinates());
    } else if (auto comment = qobject_cast<aadl::AADLCommentGraphicsItem *>(sender())) {
        qDebug() << "Graphics Comment geometry:" << comment->sceneBoundingRect();
        qDebug() << "Internal Comment data:" << comment->entity()->title()
                 << utils::rect(comment->entity()->coordinates()) << utils::rect(comment->entity()->innerCoordinates());
    } else {
        qFatal("Not implemented trace");
    }
}

void InterfaceTabDocument::onItemDoubleClicked()
{
    if (auto clickedItem = qobject_cast<ClickNotifierItem *>(sender())) {
        if (auto clickedEntity = qobject_cast<aadl::AADLObject *>(clickedItem->dataObject())) {
            if (clickedEntity->aadlType() == aadl::AADLObject::AADLObjectType::AADLFunction) {
                if (auto function = qobject_cast<aadl::AADLObjectFunction *>(clickedEntity)) {
                    if (!function->children().isEmpty()) {
                        onRootItemChanged(function->id());
                        return;
                    }
                }
            }
            if (clickedEntity->aadlType() != aadl::AADLObject::AADLObjectType::AADLConnection)
                showPropertyEditor(clickedEntity);
        }
    }
}

void InterfaceTabDocument::onRootItemChanged(const common::Id &id)
{
    m_model->setRootObject(id);
    m_actExitToRoot->setVisible(nullptr != m_model->rootObject());
    m_actExitToParent->setVisible(nullptr != m_model->rootObject());
    if (auto rootItem = qgraphicsitem_cast<aadl::AADLFunctionGraphicsItem *>(m_items.value(id))) {
        const QRect rootItemRect =
                m_graphicsView->viewport()->geometry().marginsRemoved(utils::kContentMargins.toMargins());
        rootItem->setRect({ m_graphicsView->mapToScene(rootItemRect.topLeft()),
                            m_graphicsView->mapToScene(rootItemRect.bottomRight()) });
    }
}

void InterfaceTabDocument::showPropertyEditor(aadl::AADLObject *obj)
{
    aadl::PropertiesDialog *dialog = new aadl::PropertiesDialog(obj, qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceTabDocument::clearScene()
{
    m_graphicsScene->clear();
    m_items.clear();
}

void InterfaceTabDocument::onAttributesManagerRequested()
{
    aadl::DynamicPropertyManager *dialog = new aadl::DynamicPropertyManager(qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceTabDocument::onDataTypesMenuInvoked()
{
    if (auto act = qobject_cast<QAction *>(sender()))
        showNIYGUI(act->text());
}

void InterfaceTabDocument::onColorSchemeMenuInvoked()
{
    aadl::ColorManagerDialog *dialog = new aadl::ColorManagerDialog(qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
}

void InterfaceTabDocument::onDynContextEditorMenuInvoked()
{
    aadl::DynActionEditor *dialog = new aadl::DynActionEditor(qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);

    dialog->open();
    if (!dialog->init())
        dialog->close();
}

void InterfaceTabDocument::showNIYGUI(const QString &title)
{
    QString header = title.isEmpty() ? "NIY" : title;
    QWidget *mainWindow = qobject_cast<QWidget *>(parent());
    QMessageBox::information(mainWindow, header, "Not implemented yet!");
}

} // ns document
} // ns taste3
