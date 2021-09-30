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

#include "dvtreeviewmodel.h"

#include "commands/cmdentityattributechange.h"
#include "commandsstackbase.h"
#include "dvmodel.h"
#include "dvnamevalidator.h"

namespace dve {

DVTreeViewModel::DVTreeViewModel(dvm::DVModel *dvModel, shared::cmd::CommandsStackBase *commandsStack, QObject *parent)
    : shared::AbstractVisualizationModel(dvModel, commandsStack, parent)
{
    connect(this, &QStandardItemModel::dataChanged, this, &DVTreeViewModel::onDataChanged);
}

Qt::ItemFlags DVTreeViewModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags itemFlags = shared::AbstractVisualizationModel::flags(index);
    const shared::Id id = index.data(IdRole).toUuid();
    if (auto obj = m_veModel->getObject(id)->as<dvm::DVObject *>()) {
        if (obj->type() == dvm::DVObject::Type::SystemFunction || obj->type() == dvm::DVObject::Type::SystemInterface) {
            // disable edit
            itemFlags &= ~Qt::ItemIsEditable;
        }
    }
    return itemFlags;
}

void DVTreeViewModel::updateItemData(QStandardItem *item, shared::VEObject *object)
{
    dvm::DVObject *obj = qobject_cast<dvm::DVObject *>(object);
    if (!obj) {
        return;
    }
    item->setData(dvm::DVNameValidator::decodeName(obj->type(), obj->title()), Qt::DisplayRole);
    item->setData(QVariant::fromValue(obj), DVObjectRole);

    QPixmap pix;
    switch (obj->type()) {
    case dvm::DVObject::Type::Bus: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/bus.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case dvm::DVObject::Type::Connection: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/connection.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case dvm::DVObject::Type::Device: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/port.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case dvm::DVObject::Type::Function: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/function.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case dvm::DVObject::Type::Message: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/message.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case dvm::DVObject::Type::Node: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/node.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case dvm::DVObject::Type::Partition: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/partition.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    default:
        break;
    }
    item->setData(pix, Qt::DecorationRole);
}

QStandardItem *DVTreeViewModel::createItem(shared::VEObject *obj)
{
    QStandardItem *item = shared::AbstractVisualizationModel::createItem(obj);
    connect(obj, &shared::VEObject::attributeChanged, this, [this](const QString &attrName) {
        if (attrName == dvm::meta::Props::token(dvm::meta::Props::Token::name)) {
            updateItem();
        }
    });
    updateItemData(item, obj);
    return item;
}

void DVTreeViewModel::onDataChanged(
        const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    if (!m_commandsStack) {
        return;
    }

    const QStandardItem *firstItem = itemFromIndex(topLeft);
    const QStandardItem *lastItem = itemFromIndex(bottomRight);
    Q_ASSERT(firstItem->parent() == lastItem->parent());
    for (int row = firstItem->row(); row <= lastItem->row(); ++row) {
        const QStandardItem *parent = firstItem->parent() ? firstItem->parent() : invisibleRootItem();
        if (auto item = parent->child(row)) {
            if (roles.contains(Qt::DisplayRole) || roles.isEmpty()) {
                const shared::Id id = item->data(IdRole).toUuid();
                if (auto obj = m_veModel->getObject(id)->as<dvm::DVObject *>()) {
                    const QString name = dvm::DVNameValidator::encodeName(obj->type(), item->text());
                    if (name != obj->title()) {
                        if (dvm::DVNameValidator::isAcceptableName(obj, name)) {
                            const QVariantHash attributes = { { dvm::meta::Props::token(dvm::meta::Props::Token::name),
                                    name } };
                            auto attributesCmd = new shared::cmd::CmdEntityAttributeChange(obj, attributes);
                            m_commandsStack->push(attributesCmd);
                        } else {
                            updateItemData(item, obj);
                        }
                    }
                }
            }
        }
    }
}

} // namespace dve
