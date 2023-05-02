/*
  Copyright (C) 2020-2021 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivvisualizationmodelbase.h"

#include "commands/cmdchangelayervisibility.h"
#include "commands/cmdentityattributeschange.h"
#include "commandsstack.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivconnectionlayertype.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"
#include "ivpropertytemplateconfig.h"

#include <QDebug>
#include <QDirIterator>
#include <QFileInfo>

namespace ive {

/**
 *  Visualization Model Base
 */

IVVisualizationModelBase::IVVisualizationModelBase(
        ivm::IVModel *ivModel, cmd::CommandsStack *commandsStack, QObject *parent)
    : shared::AbstractVisualizationModel(ivModel, commandsStack, parent)
{
}

void IVVisualizationModelBase::updateItemData(QStandardItem *item, shared::VEObject *object)
{
    ivm::IVObject *obj = qobject_cast<ivm::IVObject *>(object);
    Q_ASSERT(item);
    Q_ASSERT(obj);
    if (!item || !obj) {
        return;
    }

    QString title = ivm::IVNameValidator::decodeName(obj->type(), obj->title());
    QPixmap pix;
    QFont font;
    QPixmap dragPix;
    switch (obj->type()) {
    case ivm::IVObject::Type::Unknown:
        return;
    case ivm::IVObject::Type::RequiredInterface: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/ri.svg")).pixmap(16, 16);
        pix = icon;
    } break;

    case ivm::IVObject::Type::ProvidedInterface: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/pi.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::Connection: {
        static const QPixmap iconHidden =
                QIcon(QLatin1String(":/toolbar/icns/connection.svg")).pixmap(16, 16, QIcon::Disabled);
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/connection.svg")).pixmap(16, 16);
        pix = obj->isGrouped() ? iconHidden : icon;

        if (auto connectionPtr = qobject_cast<ivm::IVConnection *>(obj)) {
            const QString sourceName =
                    ivm::IVNameValidator::decodeName(ivm::IVObject::Type::Function, connectionPtr->sourceName());
            const QString sourceInterfaceName = ivm::IVNameValidator::decodeName(
                    ivm::IVObject::Type::RequiredInterface, connectionPtr->sourceInterfaceName());
            const QString targetName =
                    ivm::IVNameValidator::decodeName(ivm::IVObject::Type::Function, connectionPtr->targetName());
            const QString targetInterfaceName = ivm::IVNameValidator::decodeName(
                    ivm::IVObject::Type::ProvidedInterface, connectionPtr->targetInterfaceName());
            title = QStringLiteral("%1.%2 -> %3.%4")
                            .arg(sourceName, sourceInterfaceName, targetName, targetInterfaceName);
        }
    } break;
    case ivm::IVObject::Type::Function: {
        static const QPixmap dragIcon = QIcon(QLatin1String(":/toolbar/icns/function.svg")).pixmap(128, 128);
        dragPix = dragIcon;

        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/function.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::FunctionType: {
        static const QPixmap dragIcon = QIcon(QLatin1String(":/toolbar/icns/function_type.svg")).pixmap(128, 128);
        dragPix = dragIcon;

        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/function_type.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::Comment: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/comment.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::ConnectionGroup: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/connection_group.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::ConnectionLayer: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/connection_layer.svg")).pixmap(16, 16);
        pix = icon;
    } break;

    default:
        break;
    }

    QColor color;
    if (obj->isGrouped()) {
        color = QColor(Qt::darkGray);
        font.setItalic(true);
    } else if (!obj->isVisible()) {
        color = QColor(Qt::lightGray);
        font.setItalic(true);
        font.setWeight(QFont::Light);
    } else {
        color = QColor(Qt::black);
    }
    item->setData(dragPix, CursorPixmapRole);
    item->setData(color, Qt::ForegroundRole);
    item->setData(font, Qt::FontRole);
    item->setData(title, Qt::DisplayRole);
    item->setData(pix, Qt::DecorationRole);
}

QList<QStandardItem *> IVVisualizationModelBase::createItems(shared::VEObject *object)
{
    ivm::IVObject *obj = qobject_cast<ivm::IVObject *>(object);
    if (!obj) {
        return {};
    }

    if (obj->type() == ivm::IVObject::Type::InterfaceGroup
            || obj->type() == ivm::IVObject::Type::ArchetypeLibraryReference
            || obj->type() == ivm::IVObject::Type::ArchetypeReference) {
        return {};
    }

    QList<QStandardItem *> items = AbstractVisualizationModel::createItems(obj);
    items[0]->setData(static_cast<int>(obj->type()), TypeRole);

    connect(obj, &ivm::IVObject::attributeChanged, this, [&](const QString &name) {
        if (auto ivObj = qobject_cast<ivm::IVObject *>(sender())) {

            static const QStringList monitoredAttributes = {
                ivm::meta::Props::token(ivm::meta::Props::Token::instance_of),
                ivm::meta::Props::token(ivm::meta::Props::Token::name),
                ivm::meta::Props::token(ivm::meta::Props::Token::is_visible),
                ivm::meta::Props::token(ivm::meta::Props::Token::group_name),
            };
            if (monitoredAttributes.contains(name)) {
                updateItem();
                if (name == ivm::meta::Props::token(ivm::meta::Props::Token::instance_of)) {
                    updateChildItems(getItem(ivObj->id()));
                }
            }
        }
    });

    if (obj->type() == ivm::IVObject::Type::ConnectionGroup) {
        if (auto connectionGroupObj = obj->as<ivm::IVConnectionGroup *>()) {
            connect(connectionGroupObj, &ivm::IVConnectionGroup::connectionAdded, this,
                    &IVVisualizationModelBase::updateConnectionItem, Qt::UniqueConnection);
            connect(connectionGroupObj, &ivm::IVConnectionGroup::connectionRemoved, this,
                    &IVVisualizationModelBase::updateConnectionItem, Qt::UniqueConnection);
            for (auto &connection : connectionGroupObj->groupedConnections()) {
                updateConnectionItem(connection);
            }
        }
    }

    if (obj->type() == ivm::IVObject::Type::Connection) {
        if (auto connectionObj = obj->as<ivm::IVConnection *>()) {
            auto updateConnectionOnNameChange = [this, obj](ivm::IVObject *ivObj) {
                if (!ivObj) {
                    return;
                }
                connect(ivObj, &ivm::IVObject::attributeChanged, this, [this, id = obj->id()](const QString &attrName) {
                    if (attrName == ivm::meta::Props::token(ivm::meta::Props::Token::name)) {
                        if (auto item = getItem(id)) {
                            updateItemData(item, m_veModel->getObject(id));
                        }
                    }
                });
            };

            updateConnectionOnNameChange(connectionObj->source());
            updateConnectionOnNameChange(connectionObj->sourceInterface());
            updateConnectionOnNameChange(connectionObj->target());
            updateConnectionOnNameChange(connectionObj->targetInterface());
        }
    }

    updateItemData(items[0], obj);
    return items;
}

void IVVisualizationModelBase::updateConnectionItem(ivm::IVConnection *connection)
{
    if (QStandardItem *groupedConnectionItem = getItem(connection->id())) {
        QStandardItem *groupedConnectionParentItem =
                groupedConnectionItem->parent() ? groupedConnectionItem->parent() : invisibleRootItem();
        QStandardItem *parentItem = getParentItem(connection) ? getParentItem(connection) : invisibleRootItem();
        if (parentItem && groupedConnectionItem && groupedConnectionItem) {
            parentItem->appendRow(groupedConnectionParentItem->takeRow(groupedConnectionItem->row()));
            updateItemData(groupedConnectionItem, connection);
        }
    }
}

/**
 * VisualizationModel
 */

IVVisualizationModel::IVVisualizationModel(ivm::IVModel *ivModel, cmd::CommandsStack *commandsStack, QObject *parent)
    : IVVisualizationModelBase(ivModel, commandsStack, parent)
{
    connect(this, &QStandardItemModel::dataChanged, this, &IVVisualizationModel::onDataChanged);
}

void IVVisualizationModel::updateItemData(QStandardItem *item, shared::VEObject *object)
{
    ivm::IVObject *obj = qobject_cast<ivm::IVObject *>(object);
    if (obj) {
        IVVisualizationModelBase::updateItemData(item, obj);
        item->setEditable(!obj->isReference() && !obj->isInstanceDescendant());

        if ((item->checkState() == Qt::Checked) != obj->isVisible()) {
            item->setData(obj->isVisible() ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
        }
    }
}

QList<QStandardItem *> IVVisualizationModel::createItems(shared::VEObject *obj)
{
    QList<QStandardItem *> items = IVVisualizationModelBase::createItems(obj);
    if (!items.isEmpty()) {
        if (auto ivObj = obj->as<ivm::IVObject *>()) {
            items[0]->setEditable(!ivObj->isReference() && !ivObj->isInstanceDescendant());
            items[0]->setCheckable(true);
            items[0]->setDragEnabled(false);
        }
    }
    return items;
}

void IVVisualizationModel::onDataChanged(
        const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (!m_commandsStack) {
        qWarning() << Q_FUNC_INFO << "No command stack set in VisualizationModel";
        return;
    }

    const QStandardItem *firstItem = itemFromIndex(topLeft);
    const QStandardItem *lastItem = itemFromIndex(bottomRight);
    Q_ASSERT(firstItem->parent() == lastItem->parent());
    for (int row = firstItem->row(); row <= lastItem->row(); ++row) {
        // QStandardItem to ivm::IVObject
        const QStandardItem *parent = firstItem->parent() ? firstItem->parent() : invisibleRootItem();
        QStandardItem *rowItem = parent->child(row);
        if (!rowItem) {
            continue;
        }
        ivm::IVObject *ivObject =
                qobject_cast<ivm::IVObject *>(qStandardItemToIVObject(rowItem)); // each row represents an IVObject
        if (!ivObject) {
            continue;
        }

        if (rowItem->isCheckable() && roles.contains(Qt::CheckStateRole)) {
            // User (un)checked a checkbox. Tell the model object that it is (in)visible.
            ivObject->setVisible(rowItem->checkState() == Qt::Checked);
        }

        // Validate the name of the given by the user in IV Structure before setting in model
        if (roles.contains(Qt::DisplayRole) && rowItem->isEditable()) {
            const QString name = ivm::IVNameValidator::encodeName(ivObject->type(), rowItem->text());
            bool nameHasChanged = name != ivObject->title();
            if (nameHasChanged) {
                if (ivm::IVNameValidator::isAcceptableName(ivObject, name)) {
                    const QList<EntityAttribute> attributes = { EntityAttribute {
                            ivm::meta::Props::token(ivm::meta::Props::Token::name), name,
                            EntityAttribute::Type::Attribute } };
                    auto attributesCmd = new shared::cmd::CmdEntityAttributesChange(
                            ivm::IVPropertyTemplateConfig::instance(), ivObject, attributes);
                    m_commandsStack->push(attributesCmd);
                } else {
                    updateItemData(rowItem, ivObject); //
                }
            }
        }
    }
}

void IVVisualizationModel::setAllItemsVisible()
{
    QHash<shared::Id, shared::VEObject *> ivObjects = m_veModel->objects();
    for (shared::VEObject *veObject : ivObjects) {
        auto ivObject = qobject_cast<ivm::IVObject *>(veObject);
        if (!ivObject) {
            continue;
        }
        if (ivObject->isFunction() || ivObject->isFunctionType() || ivObject->isComment()) {
            ivObject->setVisible(true);
        }
    }
}

/**
 * Layer model
 */
IVLayerVisualizationModel::IVLayerVisualizationModel(
        ivm::IVModel *layerModel, ivm::IVModel *objectsModel, cmd::CommandsStack *commandsStack, QObject *parent)
    : IVVisualizationModelBase(layerModel, commandsStack, parent)
    , m_objectsModel(objectsModel)
{
    connect(this, &QStandardItemModel::dataChanged, this, &IVLayerVisualizationModel::onDataChanged);
}

QList<QStandardItem *> IVLayerVisualizationModel::createItems(shared::VEObject *obj)
{
    QList<QStandardItem *> items = IVVisualizationModelBase::createItems(obj);
    ivm::IVObject *ivObj = qobject_cast<ivm::IVObject *>(obj);

    if (!items.isEmpty()) {
        items[0]->setEditable(true);
        items[0]->setCheckable(true);
        items[0]->setDragEnabled(false);
        items[0]->setData(ivObj->isVisible() ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
    }

    return items;
}

void IVLayerVisualizationModel::onDataChanged(
        const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (!m_commandsStack) {
        qWarning() << Q_FUNC_INFO << "No command stack set in IVLayerVisualizationModel";
        return;
    }

    if (topLeft != bottomRight) {
        qWarning() << "Not allowed to modify more than one layer at a time";
        return;
    }

    QStandardItem *item = itemFromIndex(topLeft);
    if (!item) {
        return;
    }

    if (roles.contains(Qt::CheckStateRole) || roles.contains(Qt::DisplayRole) || roles.isEmpty()) {
        if (item->isCheckable()) {

            const auto rawLayerName = item->text();
            const auto encodedLayerName =
                    ivm::IVNameValidator::encodeName(ivm::IVObject::Type::ConnectionLayer, rawLayerName);
            const auto layerIsChecked = item->checkState() == Qt::Checked;
            const auto ivModel = qobject_cast<ivm::IVModel *>(m_veModel);

            setObjectsVisibility(encodedLayerName, layerIsChecked);

            if (ivm::IVObject *layerObj = ivModel->getObjectByName(
                        encodedLayerName, ivm::IVObject::Type::ConnectionLayer, Qt::CaseSensitivity::CaseSensitive)) {
                layerObj->setVisible(layerIsChecked);

                auto command = new ive::cmd::CmdChangeLayerVisibility();
                m_commandsStack->push(command);
            } else {
                qWarning() << "Can't find " << rawLayerName << " layer";
            }
        }
    }
}

void IVLayerVisualizationModel::setObjectsVisibility(const QString &encodedLayerName, const bool &isVisible)
{
    for (auto entity : m_objectsModel->objects()) {
        auto ivObject = qobject_cast<ivm::IVObject *>(entity);

        switch (ivObject->type()) {
        case ivm::IVObject::Type::Connection: {
            auto connection = ivObject->as<ivm::IVConnection *>();
            if (connection->layer()->title() == encodedLayerName) {
                connection->setVisible(isVisible);
            }
            break;
        }
        case ivm::IVObject::Type::RequiredInterface:
        case ivm::IVObject::Type::ProvidedInterface: {
            auto interface = ivObject->as<ivm::IVInterface *>();
            if (interface->layerName() == encodedLayerName) {
                interface->setVisible(isVisible);
            }
            break;
        }
        default:
            break;
        }
    }
}

} // namespace ive
