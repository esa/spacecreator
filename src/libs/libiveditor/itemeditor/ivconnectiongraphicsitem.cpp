/*
  Copyright (C) 2018-2019 European Space Agency - <maxime.perrotin@esa.int>

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

#include "ivconnectiongraphicsitem.h"

#include "colors/colormanager.h"
#include "graphicsviewutils.h"
#include "itemeditor/common/ivutils.h"
#include "itemeditor/graphicsitemhelpers.h"
#include "ivcommentgraphicsitem.h"
#include "ivconnection.h"
#include "ivfunction.h"
#include "ivfunctiongraphicsitem.h"
#include "ivfunctiontypegraphicsitem.h"
#include "ivinterface.h"
#include "ivinterfacegraphicsitem.h"
#include "ivnamevalidator.h"
#include "ui/grippointshandler.h"

#include <QGuiApplication>
#include <QPainter>
#include <QPen>
#include <QtDebug>
#include <QtMath>

namespace ive {

IVConnectionGraphicsItem::IVConnectionGraphicsItem(ivm::IVConnection *connection, IVInterfaceGraphicsItem *startIface,
        IVInterfaceGraphicsItem *endIface, QGraphicsItem *parentItem)
    : shared::ui::VEConnectionGraphicsItem(connection, startIface, endIface, parentItem)
{
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemHasNoContents | QGraphicsItem::ItemClipsToShape);
    setZValue(ZOrder.Connection);
}

IVConnectionGraphicsItem::~IVConnectionGraphicsItem() { }

ivm::IVConnection *IVConnectionGraphicsItem::entity() const
{
    return qobject_cast<ivm::IVConnection *>(m_dataObject);
}

QPainterPath IVConnectionGraphicsItem::shape() const
{
    return m_item->shape();
}

QRectF IVConnectionGraphicsItem::boundingRect() const
{
    return m_item->boundingRect();
}

int IVConnectionGraphicsItem::itemLevel(bool isSelected) const
{
    return gi::itemLevel(entity(), isSelected);
}

void IVConnectionGraphicsItem::rebuildLayout()
{
    if (!m_startItem || !m_startItem->isVisible() || !m_endItem || !m_endItem->isVisible() || !entity()
            || !entity()->isVisible()
            || (gi::nestingLevel(entity()) < gi::kNestingVisibilityLevel && !entity()->isRootObject())) {
        setVisible(false);
        return;
    }

    shared::ui::VEConnectionGraphicsItem::rebuildLayout();
}

shared::ColorManager::HandledColors IVConnectionGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::Connection;
}

void IVConnectionGraphicsItem::applyColorScheme()
{
    const shared::ColorHandler &h = colorHandler();
    m_item->setPen(h.pen());
    update();
}

QString IVConnectionGraphicsItem::prepareTooltip() const
{
    const QString sourceName = ivm::IVNameValidator::decodeName(ivm::IVObject::Type::Function, entity()->sourceName());
    const QString sourceInterfaceName =
            ivm::IVNameValidator::decodeName(ivm::IVObject::Type::RequiredInterface, entity()->sourceInterfaceName());
    const QString targetName = ivm::IVNameValidator::decodeName(ivm::IVObject::Type::Function, entity()->targetName());
    const QString targetInterfaceName =
            ivm::IVNameValidator::decodeName(ivm::IVObject::Type::ProvidedInterface, entity()->targetInterfaceName());
    const QString sign = entity()->sourceInterface()->isRequired() ? "->" : "<-";
    const QString tooltip =
            QString("%1.%2 %3 %4.%5").arg(sourceName, sourceInterfaceName, sign, targetName, targetInterfaceName);
    return tooltip;
}

} // namespace ive
