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

#include "commands/cmdentityattributechange.h"
#include "commandsstack.h"
#include "ivconnection.h"
#include "ivconnectiongroup.h"
#include "ivmodel.h"
#include "ivnamevalidator.h"

#include <QDebug>
#include <QDirIterator>
#include <QFileInfo>

namespace ive {

IVVisualizationModelBase::IVVisualizationModelBase(
        ivm::IVModel *ivModel, cmd::CommandsStack *commandsStack, shared::DropData::Type dropType, QObject *parent)
    : shared::AbstractVisualizationModel(ivModel, commandsStack, parent)
    , m_dropType(dropType)
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

    QString title = obj->titleUI();
    QPixmap pix;
    QFont font;
    QPixmap dragPix;
    switch (obj->type()) {
    case ivm::IVObject::Type::Unknown:
        return;
    case ivm::IVObject::Type::RequiredInterface: {
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/ri.svg")).pixmap(16, 16);
        pix = icon;
    } break;

    case ivm::IVObject::Type::ProvidedInterface: {
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/pi.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::Connection: {
        static const QPixmap iconHidden =
                QIcon(QLatin1String(":/tab_interface/toolbar/icns/connection.svg")).pixmap(16, 16, QIcon::Disabled);
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/connection.svg")).pixmap(16, 16);
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
            title = QStringLiteral("%1.%2 <-> %3.%4")
                            .arg(sourceName, sourceInterfaceName, targetName, targetInterfaceName);
        }
    } break;
    case ivm::IVObject::Type::Function: {
        static const QPixmap dragIcon =
                QIcon(QLatin1String(":/tab_interface/toolbar/icns/function.svg")).pixmap(128, 128);
        dragPix = dragIcon;

        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/function.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::FunctionType: {
        static const QPixmap dragIcon =
                QIcon(QLatin1String(":/tab_interface/toolbar/icns/function_type.svg")).pixmap(128, 128);
        dragPix = dragIcon;

        static const QPixmap icon =
                QIcon(QLatin1String(":/tab_interface/toolbar/icns/function_type.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::Comment: {
        static const QPixmap icon = QIcon(QLatin1String(":/tab_interface/toolbar/icns/comment.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case ivm::IVObject::Type::ConnectionGroup: {
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
    item->setData(static_cast<int>(m_dropType), DropRole);
    item->setData(dragPix, CursorPixmapRole);
    item->setData(color, Qt::ForegroundRole);
    item->setData(font, Qt::FontRole);
    item->setData(title, Qt::DisplayRole);
    item->setData(pix, Qt::DecorationRole);
}

QStandardItem *IVVisualizationModelBase::createItem(shared::VEObject *object)
{
    ivm::IVObject *obj = qobject_cast<ivm::IVObject *>(object);
    if (!obj) {
        return nullptr;
    }

    if (obj->type() == ivm::IVObject::Type::InterfaceGroup) {
        return nullptr;
    }

    auto item = AbstractVisualizationModel::createItem(obj);
    item->setData(static_cast<int>(obj->type()), TypeRole);

    connect(obj, &ivm::IVObject::titleChanged, this, &IVVisualizationModelBase::updateItem);
    connect(obj, &ivm::IVObject::visibilityChanged, this, &IVVisualizationModelBase::updateItem);
    connect(obj, &ivm::IVObject::groupChanged, this, &IVVisualizationModelBase::updateItem);

    if (obj->type() == ivm::IVObject::Type::ConnectionGroup) {
        if (auto connectionGroupObj = obj->as<ivm::IVConnectionGroup *>()) {
            connect(connectionGroupObj, &ivm::IVConnectionGroup::connectionAdded, this,
                    &IVVisualizationModelBase::updateConnectionItem, Qt::UniqueConnection);
            connect(connectionGroupObj, &ivm::IVConnectionGroup::connectionRemoved, this,
                    &IVVisualizationModelBase::updateConnectionItem, Qt::UniqueConnection);
            for (auto connection : connectionGroupObj->groupedConnections()) {
                updateConnectionItem(connection);
            }
        }
    }

    updateItemData(item, obj);
    return item;
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

IVVisualizationModel::IVVisualizationModel(ivm::IVModel *ivModel, cmd::CommandsStack *commandsStack, QObject *parent)
    : IVVisualizationModelBase(ivModel, commandsStack, shared::DropData::Type::None, parent)
{
    connect(this, &QStandardItemModel::dataChanged, this, &IVVisualizationModel::onDataChanged);
}

void IVVisualizationModel::updateItemData(QStandardItem *item, shared::VEObject *object)
{
    if (ivm::IVObject *obj = qobject_cast<ivm::IVObject *>(object)) {
        IVVisualizationModelBase::updateItemData(item, obj);
        if ((item->checkState() == Qt::Checked) != obj->isVisible()) {
            item->setData(obj->isVisible() ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);
        }
    }
}

QStandardItem *IVVisualizationModel::createItem(shared::VEObject *obj)
{
    auto item = IVVisualizationModelBase::createItem(obj);
    if (item) {
        item->setEditable(true);
        item->setCheckable(true);
        item->setDragEnabled(false);
    }
    return item;
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
        const QStandardItem *parent = firstItem->parent() ? firstItem->parent() : invisibleRootItem();
        if (auto item = parent->child(row)) {
            if (roles.contains(Qt::CheckStateRole) || roles.contains(Qt::DisplayRole) || roles.isEmpty()) {
                const shared::Id id = item->data(IdRole).toUuid();
                if (auto obj = m_veModel->getObject(id)->as<ivm::IVObject *>()) {
                    if (item->isCheckable() && roles.contains(Qt::CheckStateRole)) {
                        obj->setVisible(item->checkState() == Qt::Checked);
                    }
                    if (roles.contains(Qt::DisplayRole)) {
                        const QString name = ivm::IVNameValidator::encodeName(obj->type(), item->text());
                        if (name != obj->title()) {
                            if (ivm::IVNameValidator::isAcceptableName(obj, name)) {
                                const QVariantHash attributes = {
                                    { ivm::meta::Props::token(ivm::meta::Props::Token::name), name }
                                };
                                auto attributesCmd = new shared::cmd::CmdEntityAttributeChange(obj, attributes);
                                m_commandsStack->push(attributesCmd);
                            } else {
                                item->setData(obj->titleUI(), Qt::DisplayRole);
                            }
                        }
                    }
                }
            }
        }
    }
}

} // namespace ive
