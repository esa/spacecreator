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

#include "commonvisualizationmodel.h"

#include "aadlnamevalidator.h"
#include "aadlobjectconnection.h"
#include "aadlobjectconnectiongroup.h"
#include "aadlobjectsmodel.h"
#include "aadlxmlreader.h"

#include <QDebug>
#include <QDirIterator>
#include <QFileInfo>

namespace aadlinterface {

CommonVisualizationModel::CommonVisualizationModel(aadl::AADLObjectsModel *aadlModel, QObject *parent)
    : QStandardItemModel(parent)
    , m_aadlModel(aadlModel)
{
    connect(m_aadlModel, &aadl::AADLObjectsModel::modelReset, this, [this]() {
        m_itemCache.clear();
        removeRows(0, rowCount());
    });
    connect(m_aadlModel, &aadl::AADLObjectsModel::aadlObjectsAdded, this, &CommonVisualizationModel::addItems);
    connect(m_aadlModel, &aadl::AADLObjectsModel::aadlObjectRemoved, this, &CommonVisualizationModel::removeItem);
    setSortRole(TypeRole);
}

void CommonVisualizationModel::updateItemData(QStandardItem *item, aadl::AADLObject *obj)
{
    Q_ASSERT(item);
    Q_ASSERT(obj);
    if (!item || !obj) {
        return;
    }

    QString title = obj->titleUI();
    QPixmap pix;
    QFont font;

    switch (obj->aadlType()) {
    case aadl::AADLObject::Type::Unknown:
        return;
    case aadl::AADLObject::Type::RequiredInterface: {
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/ri.svg")).pixmap(16, 16);
        pix = icon;
    } break;

    case aadl::AADLObject::Type::ProvidedInterface: {
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/pi.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case aadl::AADLObject::Type::Connection: {
        static const QPixmap iconHidden =
                QIcon(QLatin1String(":/tab_interface/toolbar/icns/connection.svg")).pixmap(16, 16, QIcon::Disabled);
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/connection.svg")).pixmap(16, 16);
        pix = obj->isGrouped() ? iconHidden : icon;

        if (auto connectionPtr = qobject_cast<aadl::AADLObjectConnection *>(obj)) {
            const QString sourceName =
                    aadl::AADLNameValidator::decodeName(aadl::AADLObject::Type::Function, connectionPtr->sourceName());
            const QString sourceInterfaceName = aadl::AADLNameValidator::decodeName(
                    aadl::AADLObject::Type::RequiredInterface, connectionPtr->sourceInterfaceName());
            const QString targetName =
                    aadl::AADLNameValidator::decodeName(aadl::AADLObject::Type::Function, connectionPtr->targetName());
            const QString targetInterfaceName = aadl::AADLNameValidator::decodeName(
                    aadl::AADLObject::Type::ProvidedInterface, connectionPtr->targetInterfaceName());
            title = QStringLiteral("%1.%2 <-> %3.%4")
                            .arg(sourceName, sourceInterfaceName, targetName, targetInterfaceName);
        }
    } break;
    case aadl::AADLObject::Type::Function: {
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/function.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case aadl::AADLObject::Type::FunctionType: {
        static const QPixmap icon =
                QIcon(QLatin1String(":/tab_interface/toolbar/icns/function_type.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case aadl::AADLObject::Type::Comment: {
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/comment.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case aadl::AADLObject::Type::ConnectionGroup: {
        static const QPixmap icon =
                QIcon(QLatin1String(":/tab_interface/toolbar/icns/connection_group.svg")).pixmap(16, 16);
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
    item->setData(color, Qt::ForegroundRole);
    item->setData(font, Qt::FontRole);
    item->setData(title, Qt::DisplayRole);
    item->setData(pix, Qt::DecorationRole);
}

QStandardItem *CommonVisualizationModel::createItem(aadl::AADLObject *obj)
{
    if (!obj) {
        return nullptr;
    }

    auto item = new QStandardItem(obj->titleUI());
    item->setDragEnabled(true);
    item->setData(obj->id(), IdRole);
    item->setData(static_cast<int>(obj->aadlType()), TypeRole);

    connect(obj, &aadl::AADLObject::titleChanged, this, &CommonVisualizationModel::updateItem);
    connect(obj, &aadl::AADLObject::visibilityChanged, this, &CommonVisualizationModel::updateItem);
    connect(obj, &aadl::AADLObject::groupChanged, this, &CommonVisualizationModel::updateItem);
    updateItemData(item, obj);
    return item;
}

void CommonVisualizationModel::addItem(aadl::AADLObject *obj)
{
    if (obj->aadlType() == aadl::AADLObject::Type::InterfaceGroup) {
        return;
    }

    if (m_itemCache.contains(obj->id())) {
        return;
    }

    const auto item = createItem(obj);
    if (auto parentItem = getParentItem(obj)) {
        parentItem->appendRow(item);
        parentItem->sortChildren(0);
        m_itemCache.insert(obj->id(), item);
        if (obj->aadlType() == aadl::AADLObject::Type::ConnectionGroup) {
            if (auto connectionGroupObj = obj->as<aadl::AADLObjectConnectionGroup *>()) {
                connect(connectionGroupObj, &aadl::AADLObjectConnectionGroup::connectionAdded, this,
                        &CommonVisualizationModel::updateConnectionItem, Qt::UniqueConnection);
                connect(connectionGroupObj, &aadl::AADLObjectConnectionGroup::connectionRemoved, this,
                        &CommonVisualizationModel::updateConnectionItem, Qt::UniqueConnection);
                for (auto connection : connectionGroupObj->groupedConnections()) {
                    updateConnectionItem(connection);
                }
            }
        }
    }
}

void CommonVisualizationModel::updateConnectionItem(aadl::AADLObjectConnection *connection)
{
    if (QStandardItem *groupedConnectionItem = getItem(connection->id())) {
        QStandardItem *groupedConnectionParentItem =
                groupedConnectionItem->parent() ? groupedConnectionItem->parent() : invisibleRootItem();
        getParentItem(connection)->appendRow(groupedConnectionParentItem->takeRow(groupedConnectionItem->row()));
        updateItemData(groupedConnectionItem, connection);
    }
}

void CommonVisualizationModel::addItems(const QVector<aadl::AADLObject *> &objects)
{
    for (auto obj : objects) {
        addItem(obj);
    }
}

void CommonVisualizationModel::removeItem(aadl::AADLObject *obj)
{
    const QStandardItem *item = m_itemCache.take(obj->id());
    obj->disconnect(this);
    if (!item) {
        return;
    } else {
        QStandardItem *parentItem = item->parent() ? item->parent() : invisibleRootItem();
        if (parentItem) {
            parentItem->removeRow(item->row());
        }
    }
}

void CommonVisualizationModel::updateItem()
{
    if (auto obj = qobject_cast<aadl::AADLObject *>(sender())) {
        if (auto item = getItem(obj->id())) {
            updateItemData(item, obj);
        }
    }
}

QStandardItem *CommonVisualizationModel::getParentItem(aadl::AADLObject *obj)
{
    if (!obj) {
        return nullptr;
    }

    return obj->parentObject() ? getItem(obj->parentObject()) : invisibleRootItem();
}

QStandardItem *CommonVisualizationModel::getItem(aadl::AADLObject *obj)
{
    return obj ? getItem(obj->id()) : nullptr;
}

QStandardItem *CommonVisualizationModel::getItem(const shared::Id id)
{
    return id.isNull() ? nullptr : m_itemCache.value(id);
}

VisualizationModel::VisualizationModel(aadl::AADLObjectsModel *aadlModel, QObject *parent)
    : CommonVisualizationModel(aadlModel, parent)
{
    connect(this, &QStandardItemModel::dataChanged, this, &VisualizationModel::onDataChanged);
}

void VisualizationModel::updateItemData(QStandardItem *item, aadl::AADLObject *obj)
{
    CommonVisualizationModel::updateItemData(item, obj);
    if ((item->checkState() == Qt::Checked) != obj->isVisible()) {
        item->setData(obj->isVisible() ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
    }
}

QStandardItem *VisualizationModel::createItem(aadl::AADLObject *obj)
{
    auto item = CommonVisualizationModel::createItem(obj);
    item->setEditable(true);
    item->setCheckable(true);
    item->setDragEnabled(false);
    return item;
}

void VisualizationModel::onDataChanged(
        const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    const QStandardItem *firstItem = itemFromIndex(topLeft);
    const QStandardItem *lastItem = itemFromIndex(bottomRight);
    Q_ASSERT(firstItem->parent() == lastItem->parent());
    for (int row = firstItem->row(); row <= lastItem->row(); ++row) {
        const QStandardItem *parent = firstItem->parent() ? firstItem->parent() : invisibleRootItem();
        if (auto item = parent->child(row)) {
            if (roles.contains(Qt::CheckStateRole) || roles.contains(Qt::DisplayRole) || roles.isEmpty()) {
                const shared::Id id = item->data(IdRole).toUuid();
                if (auto obj = m_aadlModel->getObject(id)) {
                    if (item->isCheckable() && roles.contains(Qt::CheckStateRole)) {
                        obj->setVisible(item->checkState() == Qt::Checked);
                    }
                    if (roles.contains(Qt::DisplayRole)) {
                        obj->setTitle(item->text());
                    }
                }
            }
        }
    }
}

} // namespace aadlinterface
