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
  along with this program. If not, see
  <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "ivinterfacegroupgraphicsitem.h"

#include "ivinterfacegroup.h"

namespace ive {

IVInterfaceGroupGraphicsItem::IVInterfaceGroupGraphicsItem(ivm::IVInterfaceGroup *entity, QGraphicsItem *parent)
    : IVInterfaceGraphicsItem(entity, parent)
{
}

shared::ColorManager::HandledColors IVInterfaceGroupGraphicsItem::handledColorType() const
{
    return shared::ColorManager::HandledColors::IfaceGroup;
}

ivm::IVInterfaceGroup *IVInterfaceGroupGraphicsItem::entity() const
{
    return qobject_cast<ivm::IVInterfaceGroup *>(m_dataObject);
}

QPainterPath IVInterfaceGroupGraphicsItem::typePath() const
{
    auto iface = qobject_cast<ivm::IVInterface *>(entity());
    if (!iface)
        return {};

    QPainterPath kindPath;
    static const qreal kindBaseValue = typeIconHeight();
    const qreal kindWidth = kindBaseValue / 2;
    kindPath.lineTo(-kindWidth, -kindBaseValue / 4);
    kindPath.lineTo(0, -kindBaseValue / 2);
    kindPath.lineTo(kindWidth, -kindBaseValue / 4);
    kindPath.lineTo(0, 0);

    kindPath.moveTo(-kindWidth * 2 / 3, -kindBaseValue / 6);
    kindPath.lineTo(-kindWidth, 0);
    kindPath.lineTo(0, kindBaseValue / 4);
    kindPath.lineTo(kindWidth, 0);
    kindPath.lineTo(kindWidth * 2 / 3, -kindBaseValue / 6);

    kindPath.moveTo(-kindWidth * 2 / 3, kindBaseValue / 6);
    kindPath.lineTo(-kindWidth, kindBaseValue / 4);
    kindPath.lineTo(0, kindBaseValue / 2);
    kindPath.lineTo(kindWidth, kindBaseValue / 4);
    kindPath.lineTo(kindWidth * 2 / 3, kindBaseValue / 6);

    return kindPath;
}

QPainterPath IVInterfaceGroupGraphicsItem::ifacePath() const
{
    QSet<int> dirs;
    for (const auto obj : entity()->entities()) {
        dirs.insert(static_cast<int>(obj->direction()));
    }
    if (dirs.size() == 1) {
        return IVInterfaceGraphicsItem::ifacePath();
    }

    static const qreal baseHeightValue = typeIconHeight();
    static const qreal baseWidthValue = baseLength();
    QPainterPath path;
    path.addPolygon(QVector<QPointF> { QPointF(-2 * baseHeightValue / 3, 0), QPointF(0, -baseWidthValue / 2),
            QPointF(2 * baseHeightValue / 3, 0), QPointF(0, baseWidthValue / 2) });
    path.closeSubpath();
    return path;
}

} // namespace ive
