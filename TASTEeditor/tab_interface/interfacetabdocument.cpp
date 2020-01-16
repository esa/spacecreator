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

#include "baseitems/clicknotifieritem.h"
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
    m_graphicsView->setSceneRect(QRectF(0, 0, 640, 480));
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

    return root;
}

bool InterfaceTabDocument::loadImpl(const QString &path)
{
    if (path.isEmpty() || !QFileInfo(path).exists()) {
        qWarning() << Q_FUNC_INFO << "Invalid path";
        return false;
    }

    aadl::AADLXMLReader parser;
    connect(&parser, &aadl::AADLXMLReader::objectsParsed, this, [this](const QVector<aadl::AADLObject *> &objects) {
        m_graphicsScene->clear();
        if (m_model->initFromObjects(objects))
            m_graphicsView->setSceneRect(m_graphicsView->scene()->itemsBoundingRect());
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
        m_actCreateFunctionType->setCheckable(true);
        m_actCreateFunctionType->setActionGroup(m_actionGroup);
        connect(m_actCreateFunctionType, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateFunctionType);
        m_actCreateFunctionType->setIcon(QIcon(":/tab_interface/toolbar/icns/function_type.svg"));
    }

    if (!m_actCreateFunction) {
        m_actCreateFunction = new QAction(tr("Function"));
        m_actCreateFunction->setCheckable(true);
        m_actCreateFunction->setActionGroup(m_actionGroup);
        connect(m_actCreateFunction, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateFunction);

        m_actCreateFunction->setIcon(QIcon(":/tab_interface/toolbar/icns/function.svg"));
    }

    if (!m_actCreateProvidedInterface) {
        m_actCreateProvidedInterface = new QAction(tr("Provided Interface"));
        m_actCreateProvidedInterface->setCheckable(true);
        m_actCreateProvidedInterface->setActionGroup(m_actionGroup);
        connect(m_actCreateProvidedInterface, &QAction::triggered, this,
                &InterfaceTabDocument::onActionCreateProvidedInterface);
        m_actCreateProvidedInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/pi.svg"));
    }

    if (!m_actCreateRequiredInterface) {
        m_actCreateRequiredInterface = new QAction(tr("Required Interface"));
        m_actCreateRequiredInterface->setCheckable(true);
        m_actCreateRequiredInterface->setActionGroup(m_actionGroup);
        connect(m_actCreateRequiredInterface, &QAction::triggered, this,
                &InterfaceTabDocument::onActionCreateRequiredInterface);
        m_actCreateRequiredInterface->setIcon(QIcon(":/tab_interface/toolbar/icns/ri.svg"));
    }

    if (!m_actCreateComment) {
        m_actCreateComment = new QAction(tr("Comment"));
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
        m_actCreateConnection->setCheckable(true);
        m_actCreateConnection->setActionGroup(m_actionGroup);
        connect(m_actCreateConnection, &QAction::triggered, this, &InterfaceTabDocument::onActionCreateConnection);
        m_actCreateConnection->setIcon(QIcon(":/tab_interface/toolbar/icns/connection.svg"));
    }

    if (!m_actRemove) {
        m_actRemove = new QAction(tr("Remove"));
        m_actRemove->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/remove.svg")));
        m_actRemove->setShortcut(QKeySequence::Delete);
        connect(m_actRemove, &QAction::triggered, this, &InterfaceTabDocument::onActionRemoveItem);
    }

    if (!m_actZoomIn) {
        m_actZoomIn = new QAction(tr("Zoom In"));
        m_actZoomIn->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/zoom_in.svg")));
        m_actZoomIn->setShortcut(QKeySequence::ZoomIn);
        connect(m_actZoomIn, &QAction::triggered, this, &InterfaceTabDocument::onActionZoomIn);
    }

    if (!m_actZoomOut) {
        m_actZoomOut = new QAction(tr("Zoom Out"));
        m_actZoomOut->setIcon(QIcon(QLatin1String(":/tab_interface/toolbar/icns/zoom_out.svg")));
        m_actZoomOut->setShortcut(QKeySequence::ZoomOut);
        connect(m_actZoomOut, &QAction::triggered, this, &InterfaceTabDocument::onActionZoomOut);
    }

    return { m_actCreateFunctionType,
             m_actCreateFunction,
             m_actCreateProvidedInterface,
             m_actCreateRequiredInterface,
             m_actCreateComment,
             m_actGroupConnections,
             m_actCreateConnection,
             m_actRemove,
             m_actZoomIn,
             m_actZoomOut };
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
    aadl::AADLObjectComment *entity = comment->entity();
    Q_ASSERT(entity);
    if (!entity)
        return;

    comment->setText(entity->title());
    const auto coordinates = entity->coordinates();
    if (coordinates.isEmpty()) {
        comment->instantLayoutUpdate();
    } else {
        comment->setRect({ QPointF(coordinates.value(0), coordinates.value(1)),
                           QPointF(coordinates.value(2), coordinates.value(3)) });
    }
}

void InterfaceTabDocument::updateInterface(aadl::AADLInterfaceGraphicsItem *iface)
{
    aadl::AADLObjectIface *entity = iface->entity();
    Q_ASSERT(entity);
    if (!entity)
        return;

    iface->setInterfaceName(entity->title());
    const auto coordinates = entity->coordinates();
    if (coordinates.isEmpty()) {
        iface->instantLayoutUpdate();
    } else {
        QGraphicsItem *parentItem = nullptr;
        aadl::AADLObject *entityParentObject = entity->parentObject();
        if (entityParentObject)
            parentItem = m_items.value(entityParentObject->id());
        iface->setTargetItem(parentItem, QPointF(coordinates.value(0), coordinates.value(1)));
    }
}

void InterfaceTabDocument::updateFunction(aadl::AADLFunctionGraphicsItem *function)
{
    aadl::AADLObjectFunction *entity = function->entity();
    Q_ASSERT(entity);
    if (!entity)
        return;

    const auto points = entity->coordinates();
    if (points.isEmpty()) {
        function->instantLayoutUpdate();
    } else {
        QGraphicsItem *parentItem = nullptr;
        if (aadl::AADLObject *entityParentObject = entity->parentObject())
            parentItem = m_items.value(entityParentObject->id());
        function->setParentItem(parentItem);
        function->setRect({ QPointF(points.value(0), points.value(1)), QPointF(points.value(2), points.value(3)) });
    }
}

void InterfaceTabDocument::updateFunctionType(aadl::AADLFunctionTypeGraphicsItem *functionType)
{
    aadl::AADLObjectFunctionType *entity = functionType->entity();
    Q_ASSERT(entity);
    if (!entity)
        return;

    const auto coordinates = entity->coordinates();
    if (coordinates.isEmpty()) {
        functionType->instantLayoutUpdate();
    } else {
        QGraphicsItem *parentItem = nullptr;
        if (aadl::AADLObject *entityParentObject = entity->parentObject())
            parentItem = m_items.value(entityParentObject->id());
        functionType->setParentItem(parentItem);
        functionType->setRect({ QPointF(coordinates.value(0), coordinates.value(1)),
                                QPointF(coordinates.value(2), coordinates.value(3)) });
    }
}

void InterfaceTabDocument::updateConnection(aadl::AADLConnectionGraphicsItem *connection)
{
    aadl::AADLObjectConnection *entity = connection->entity();
    Q_ASSERT(entity);
    if (!entity)
        return;

    const auto coordinates = entity->coordinates();
    QVector<QPointF> points;
    for (int idx = 1; idx < coordinates.size(); idx += 2)
        points << QPointF(coordinates.value(idx - 1), coordinates.value(idx));

    aadl::AADLObjectIface *ifaceStart = entity->providedInterface();
    Q_ASSERT(ifaceStart);

    aadl::AADLObjectIface *ifaceEnd = entity->requiredInterface();
    Q_ASSERT(ifaceEnd);

    QGraphicsItem *startItem = m_items.value(ifaceStart->id());
    QGraphicsItem *endItem = m_items.value(ifaceEnd->id());
    if (points.isEmpty()) {
        connection->setEndPoints(qgraphicsitem_cast<aadl::AADLInterfaceGraphicsItem *>(startItem),
                                 qgraphicsitem_cast<aadl::AADLInterfaceGraphicsItem *>(endItem));
        return;
    }

    connection->setPoints(points);
}

QGraphicsItem *InterfaceTabDocument::createItemForObject(aadl::AADLObject *obj)
{
    switch (obj->aadlType()) {
    case aadl::AADLObject::AADLObjectType::AADLComment:
        return new aadl::AADLCommentGraphicsItem(qobject_cast<aadl::AADLObjectComment *>(obj));
    case aadl::AADLObject::AADLObjectType::AADLIface:
        return new aadl::AADLInterfaceGraphicsItem(qobject_cast<aadl::AADLObjectIface *>(obj));
    case aadl::AADLObject::AADLObjectType::AADLConnection:
        return new aadl::AADLConnectionGraphicsItem(qobject_cast<aadl::AADLObjectConnection *>(obj));
    case aadl::AADLObject::AADLObjectType::AADLFunction:
        return new aadl::AADLFunctionGraphicsItem(qobject_cast<aadl::AADLObjectFunction *>(obj));
    case aadl::AADLObject::AADLObjectType::AADLFunctionType:
        return new aadl::AADLFunctionTypeGraphicsItem(qobject_cast<aadl::AADLObjectFunctionType *>(obj));
    default: {
        qCritical() << "Unknown object type:" << obj->aadlType();
        break;
    }
    }

    return nullptr;
}

void InterfaceTabDocument::onAADLObjectAdded(aadl::AADLObject *object)
{
    auto propertyChanged = [this]() {
        if (auto object = qobject_cast<aadl::AADLObject *>(sender())) {
            if (auto item = m_items.value(object->id()))
                updateItem(item);
        }
    };

    if (auto item = createItemForObject(object)) {
        connect(object, &aadl::AADLObject::coordinatesChanged, this, propertyChanged);
        connect(object, &aadl::AADLObject::titleChanged, this, propertyChanged);
        Q_ASSERT(item);
        m_items.insert(object->id(), item);
        m_graphicsScene->addItem(item);
        updateItem(item);
        m_graphicsScene->clearSelection();
        item->setSelected(true);

        if (auto clickable = qobject_cast<ClickNotifierItem *>(item->toGraphicsObject())) {
            connect(clickable, &ClickNotifierItem::clicked, this, &InterfaceTabDocument::onItemClicked);
            connect(clickable, &ClickNotifierItem::doubleClicked, this, &InterfaceTabDocument::onItemDoublelicked);
        }
    }
}

void InterfaceTabDocument::onItemClicked() {}

void InterfaceTabDocument::onItemDoublelicked()
{
    //    if (auto clickedItem = qobject_cast<ClickNotifierItem *>(sender()))
    //        if (auto clickedEntity = qobject_cast<aadl::AADLObject *>(clickedItem->dataObject()))
    //            // switch to inner view
}

void InterfaceTabDocument::showPropertyEditor(aadl::AADLObject *obj)
{
    aadl::PropertiesDialog *dialog = new aadl::PropertiesDialog(obj, qobject_cast<QWidget *>(parent()));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->open();
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

void InterfaceTabDocument::showNIYGUI(const QString &title)
{
    QString header = title.isEmpty() ? "NIY" : title;
    QWidget *mainWindow = qobject_cast<QWidget *>(parent());
    QMessageBox::information(mainWindow, header, "Not implemented yet!");
}

} // ns document
} // ns taste3
