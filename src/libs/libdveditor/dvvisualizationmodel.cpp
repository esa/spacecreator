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

#include "dvvisualizationmodel.h"

#include "dvmodel.h"

#include <QFile>

namespace dve {

DVVisualizationModel::DVVisualizationModel(
        dvm::DVModel *dvModel, shared::cmd::CommandsStackBase *commandsStack, QObject *parent)
    : shared::AbstractVisualizationModel(dvModel, commandsStack, parent)
{
}

Qt::ItemFlags DVVisualizationModel::flags(const QModelIndex &index) const
{
    return shared::AbstractVisualizationModel::flags(index) & ~Qt::ItemIsEditable;
}

void DVVisualizationModel::updateItemData(QStandardItem *item, shared::VEObject *obj)
{
    item->setData(static_cast<int>(shared::DropData::Type::ImportableType), DropRole);

    dvm::DVObject *object = qobject_cast<dvm::DVObject *>(obj);
    if (!object) {
        return;
    }
    QPixmap pix;
    switch (object->type()) {
    case dvm::DVObject::Type::Board: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/board.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    case dvm::DVObject::Type::Port: {
        static const QPixmap icon = QIcon(QLatin1String(":/toolbar/icns/port.svg")).pixmap(16, 16);
        pix = icon;
    } break;
    default:
        break;
    }
    item->setData(pix, Qt::DecorationRole);
    if (obj->hasEntityAttribute(dvm::meta::Props::token(dvm::meta::Props::Token::asn1file))) {
        const QString asn1file =
                obj->entityAttributeValue<QString>(dvm::meta::Props::token(dvm::meta::Props::Token::asn1file));
        if (!QFile::exists(asn1file)) {
            item->setData(QColor(Qt::red), Qt::ForegroundRole);
            item->setData(tr("ASN1File doesn't exists: %1").arg(asn1file), Qt::ToolTipRole);
        }
    }
}

QList<QStandardItem *> DVVisualizationModel::createItems(shared::VEObject *obj)
{
    QList<QStandardItem *> items = shared::AbstractVisualizationModel::createItems(obj);
    if (!items.isEmpty()) {
        updateItemData(items[0], obj);
    }
    return items;
}

} // namespace dve
