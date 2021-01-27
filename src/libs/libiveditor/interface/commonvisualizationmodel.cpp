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

#include "aadlconnection.h"
#include "aadlconnectiongroup.h"
#include "aadlmodel.h"
#include "aadlnamevalidator.h"
#include "aadlxmlreader.h"
#include "commandsstack.h"
#include "interface/commands/cmdentityattributechange.h"

#include <QDebug>
#include <QDirIterator>
#include <QFileInfo>

namespace ive {

CommonVisualizationModel::CommonVisualizationModel(
        ivm::AADLModel *aadlModel, cmd::CommandsStack *commandsStack, QObject *parent)
    : QStandardItemModel(parent)
    , m_aadlModel(aadlModel)
    , m_commandsStack(commandsStack)
{
    connect(m_aadlModel, &ivm::AADLModel::modelReset, this, [this]() {
        m_itemCache.clear();
        removeRows(0, rowCount());
    });
    connect(m_aadlModel, &ivm::AADLModel::aadlObjectsAdded, this, &CommonVisualizationModel::addItems);
    connect(m_aadlModel, &ivm::AADLModel::aadlObjectRemoved, this, &CommonVisualizationModel::removeItem);
    setSortRole(TypeRole);
}

void CommonVisualizationModel::updateItemData(QStandardItem *item, ivm::AADLObject *obj)
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
    case ivm::AADLObject::Type::Unknown:
        return;
    case ivm::AADLObject::Type::RequiredInterface: {
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/ri.svg")).pixmap(16, 16);
        pix = icon;
    } break;

    case ivm::AADLObject::Type::ProvidedInterface: {
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/pi.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::AADLObject::Type::Connection: {
        static const QPixmap iconHidden =
                QIcon(QLatin1String(":/tab_interface/toolbar/icns/connection.svg")).pixmap(16, 16, QIcon::Disabled);
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/connection.svg")).pixmap(16, 16);
        pix = obj->isGrouped() ? iconHidden : icon;

        if (auto connectionPtr = qobject_cast<ivm::AADLConnection *>(obj)) {
            const QString sourceName =
                    ivm::AADLNameValidator::decodeName(ivm::AADLObject::Type::Function, connectionPtr->sourceName());
            const QString sourceInterfaceName = ivm::AADLNameValidator::decodeName(
                    ivm::AADLObject::Type::RequiredInterface, connectionPtr->sourceInterfaceName());
            const QString targetName =
                    ivm::AADLNameValidator::decodeName(ivm::AADLObject::Type::Function, connectionPtr->targetName());
            const QString targetInterfaceName = ivm::AADLNameValidator::decodeName(
                    ivm::AADLObject::Type::ProvidedInterface, connectionPtr->targetInterfaceName());
            title = QStringLiteral("%1.%2 <-> %3.%4")
                            .arg(sourceName, sourceInterfaceName, targetName, targetInterfaceName);
        }
    } break;
    case ivm::AADLObject::Type::Function: {
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/function.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::AADLObject::Type::FunctionType: {
        static const QPixmap icon =
                QIcon(QLatin1String(":/tab_interface/toolbar/icns/function_type.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::AADLObject::Type::Comment: {
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/comment.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::AADLObject::Type::ConnectionGroup: {
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

QStandardItem *CommonVisualizationModel::createItem(ivm::AADLObject *obj)
{
    if (!obj) {
        return nullptr;
    }

    auto item = new QStandardItem(obj->titleUI());
    item->setDragEnabled(true);
    item->setData(obj->id(), IdRole);
    item->setData(static_cast<int>(obj->aadlType()), TypeRole);

    connect(obj, &ivm::AADLObject::titleChanged, this, &CommonVisualizationModel::updateItem);
    connect(obj, &ivm::AADLObject::visibilityChanged, this, &CommonVisualizationModel::updateItem);
    connect(obj, &ivm::AADLObject::groupChanged, this, &CommonVisualizationModel::updateItem);
    updateItemData(item, obj);
    return item;
}

void CommonVisualizationModel::addItem(ivm::AADLObject *obj)
{
    if (obj->aadlType() == ivm::AADLObject::Type::InterfaceGroup) {
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
        if (obj->aadlType() == ivm::AADLObject::Type::ConnectionGroup) {
            if (auto connectionGroupObj = obj->as<ivm::AADLConnectionGroup *>()) {
                connect(connectionGroupObj, &ivm::AADLConnectionGroup::connectionAdded, this,
                        &CommonVisualizationModel::updateConnectionItem, Qt::UniqueConnection);
                connect(connectionGroupObj, &ivm::AADLConnectionGroup::connectionRemoved, this,
                        &CommonVisualizationModel::updateConnectionItem, Qt::UniqueConnection);
                for (auto connection : connectionGroupObj->groupedConnections()) {
                    updateConnectionItem(connection);
                }
            }
        }
    }
}

void CommonVisualizationModel::updateConnectionItem(ivm::AADLConnection *connection)
{
    if (QStandardItem *groupedConnectionItem = getItem(connection->id())) {
        QStandardItem *groupedConnectionParentItem =
                groupedConnectionItem->parent() ? groupedConnectionItem->parent() : invisibleRootItem();
        getParentItem(connection)->appendRow(groupedConnectionParentItem->takeRow(groupedConnectionItem->row()));
        updateItemData(groupedConnectionItem, connection);
    }
}

void CommonVisualizationModel::addItems(const QVector<ivm::AADLObject *> &objects)
{
    for (auto obj : objects) {
        addItem(obj);
    }
}

void CommonVisualizationModel::removeItem(ivm::AADLObject *obj)
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
    if (auto obj = qobject_cast<ivm::AADLObject *>(sender())) {
        if (auto item = getItem(obj->id())) {
            updateItemData(item, obj);
        }
    }
}

QStandardItem *CommonVisualizationModel::getParentItem(ivm::AADLObject *obj)
{
    if (!obj) {
        return nullptr;
    }

    return obj->parentObject() ? getItem(obj->parentObject()) : invisibleRootItem();
}

QStandardItem *CommonVisualizationModel::getItem(ivm::AADLObject *obj)
{
    return obj ? getItem(obj->id()) : nullptr;
}

QStandardItem *CommonVisualizationModel::getItem(const shared::Id id)
{
    return id.isNull() ? nullptr : m_itemCache.value(id);
}

VisualizationModel::VisualizationModel(ivm::AADLModel *aadlModel, cmd::CommandsStack *commandsStack, QObject *parent)
    : CommonVisualizationModel(aadlModel, commandsStack, parent)
{
    connect(this, &QStandardItemModel::dataChanged, this, &VisualizationModel::onDataChanged);
}

void VisualizationModel::updateItemData(QStandardItem *item, ivm::AADLObject *obj)
{
    CommonVisualizationModel::updateItemData(item, obj);
    if ((item->checkState() == Qt::Checked) != obj->isVisible()) {
        item->setData(obj->isVisible() ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
    }
}

QStandardItem *VisualizationModel::createItem(ivm::AADLObject *obj)
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
    if (!m_commandsStack) {
        qWarning() << Q_FUNC_INFO << "No command stack set in VisualizationModel";
        return;
    }

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
                        const QString name = ivm::AADLNameValidator::encodeName(obj->aadlType(), item->text());
                        const QVariantHash attributes = { { ivm::meta::Props::token(ivm::meta::Props::Token::name),
                                name } };
                        auto attributesCmd = new cmd::CmdEntityAttributeChange(obj, attributes);
                        m_commandsStack->push(attributesCmd);
                    }
                }
            }
        }
    }
}

} // namespace ive
