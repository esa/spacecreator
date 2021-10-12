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

#include "commands/cmdentityattributeschange.h"
#include "commandsstackbase.h"
#include "dvfunction.h"
#include "dvmodel.h"
#include "dvnamevalidator.h"
#include "dvpropertytemplateconfig.h"

#include <QDebug>

namespace dve {

DVTreeViewModel::DVTreeViewModel(dvm::DVModel *dvModel, shared::cmd::CommandsStackBase *commandsStack, QObject *parent)
    : shared::AbstractVisualizationModel(dvModel, commandsStack, parent)
{
    connect(this, &QStandardItemModel::dataChanged, this, &DVTreeViewModel::onDataChanged);
    setColumnCount(2);
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

QVariant DVTreeViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (section >= 0 && section <= columnCount()) {
            switch (section) {
            case 0:
                return { tr("Name") };
            case 1:
                return { tr("Implementation") };
            }
        }
    }

    return QAbstractItemModel::headerData(section, orientation, role);
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

QList<QStandardItem *> DVTreeViewModel::createItems(shared::VEObject *obj)
{
    QList<QStandardItem *> items = shared::AbstractVisualizationModel::createItems(obj);
    connect(obj, &shared::VEObject::attributeChanged, this, [this](const QString &attrName) {
        if (attrName == dvm::meta::Props::token(dvm::meta::Props::Token::name)) {
            updateItem();
        }
        if (attrName == dvm::meta::Props::token(dvm::meta::Props::Token::selected_implementation)) {
            updateImplementation(qobject_cast<dvm::DVFunction *>(sender()));
        }
    });
    updateItemData(items.first(), obj);

    if (auto fn = qobject_cast<dvm::DVFunction *>(obj)) {
        auto implItem = new QStandardItem(fn->usedImplementation());
        implItem->setData(QVariant::fromValue(obj), DVObjectRole);
        items.append(implItem);

        connect(fn, &dvm::DVFunction::usedImplementationChanged, this,
                [this]() { updateImplementation(qobject_cast<dvm::DVFunction *>(sender())); });
    }

    return items;
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
                    switch (topLeft.column()) {
                    case Columns::Name:
                        setName(obj, item);
                        break;
                    case Columns::Implementation: {
                        // implementated in the delegate - to not set changes in the mode when triggered externally
                        break;
                    }
                    }
                }
            }
        }
    }
}

void DVTreeViewModel::setName(dvm::DVObject *obj, QStandardItem *item)
{
    const QString name = dvm::DVNameValidator::encodeName(obj->type(), item->text());
    if (name != obj->title()) {
        if (dvm::DVNameValidator::isAcceptableName(obj, name)) {
            const QList<EntityAttribute> attributes = { EntityAttribute {
                    dvm::meta::Props::token(dvm::meta::Props::Token::name), name, EntityAttribute::Type::Attribute } };
            auto attributesCmd = new shared::cmd::CmdEntityAttributesChange(
                    dvm::DVPropertyTemplateConfig::instance(), obj, attributes);
            m_commandsStack->push(attributesCmd);
        } else {
            updateItemData(item, obj);
        }
    }
}

void DVTreeViewModel::updateImplementation(dvm::DVFunction *fn)
{
    if (!fn) {
        return;
    }

    QStandardItem *implItem = implementationItem(fn);
    if (implItem) {
        implItem->setData(fn->usedImplementation(), Qt::DisplayRole);
        return;
    }
}

QStandardItem *DVTreeViewModel::implementationItem(dvm::DVFunction *fn)
{
    if (!fn) {
        return nullptr;
    }
    QStandardItem *parentItem = getItem(fn->parentObject());
    QModelIndex parentIdx = indexFromItem(parentItem);
    for (int row = 0; row < rowCount(parentIdx); ++row) {
        QModelIndex idx = index(row, Columns::Implementation, parentIdx);
        if (idx.data(DVObjectRole).value<dvm::DVFunction *>() == fn) {
            return itemFromIndex(idx);
        }
    }

    return nullptr;
}

} // namespace dve
