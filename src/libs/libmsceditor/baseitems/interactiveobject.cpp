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
   along with this program. If not, see
   <https://www.gnu.org/licenses/lgpl-2.1.html>.
*/

#include "interactiveobject.h"

#include "baseitems/common/mscutils.h"
#include "baseitems/textitem.h"
#include "chartitem.h"
#include "mscentity.h"

#include <QBrush>
#include <QCursor>
#include <QDebug>
#include <QGraphicsScene>
#include <functional>

namespace msc {

InteractiveObject::InteractiveObject(msc::MscEntity *entity, QGraphicsItem *parent)
    : shared::ui::InteractiveObjectBase(parent)
    , m_entity(entity)
{
    setAcceptHoverEvents(true);

    setFlags(QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemSendsScenePositionChanges);

    setCursor(Qt::ArrowCursor);
}

void InteractiveObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = true;
    m_storedZ = zValue();
    setZValue(m_storedZ + 1.);

    showGripPoints();
    updateGripPoints();
    QGraphicsObject::hoverEnterEvent(event);
}

void InteractiveObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hovered = false;
    setZValue(m_storedZ);

    hideGripPoints();

    QGraphicsObject::hoverLeaveEvent(event);
}

QPair<QPointF, bool> InteractiveObject::commentPoint() const
{
    const QRectF br = sceneBoundingRect();
    return qMakePair(QPointF(br.right(), br.center().y()), false);
}

MscEntity *InteractiveObject::modelEntity() const
{
    return m_entity.data();
}

void InteractiveObject::postCreatePolishing()
{
    std::function<bool(QGraphicsItem *)> findTextItem;
    findTextItem = [&](QGraphicsItem *where) {
        for (QGraphicsItem *child : where->childItems()) {
            if (TextItem *textItem = dynamic_cast<TextItem *>(child)) {
                scene()->clearSelection();
                setSelected(true);
                textItem->enableEditMode();
                return true;
            } else if (findTextItem(child)) {
                return true;
            }
        }
        return false;
    };

    findTextItem(this);
}

void InteractiveObject::applyCif()
{
    if (!modelEntity())
        return;

    const QVector<cif::CifBlockShared> &cifs = modelEntity()->cifs();
    if (cifs.size())
        qWarning() << Q_FUNC_INFO << "CIF data ignored for" << modelEntity()->name();
}

cif::CifBlockShared InteractiveObject::cifBlockByType(cif::CifLine::CifType type) const
{
    return modelEntity() ? modelEntity()->cifBlockByType(type) : cif::CifBlockShared();
}

/*!
  \brief InteractiveObject::mainCifType
  An item could have more than one CIF blocks that affects its geometry,
  say usually a Message has both the CifType::Message and CifType::Position.
  While the first one is about "how to draw message", the second is about its
  text/name position which is not so important for layouting process.
  Ancestors should override this to provide
  \return appropriate CIF type (CifType::Message in example above).
*/

cif::CifLine::CifType InteractiveObject::mainCifType() const
{
    return cif::CifLine::CifType::Unknown;
}

bool InteractiveObject::geometryManagedByCif() const
{
    const cif::CifLine::CifType targetType = mainCifType();
    return targetType == cif::CifLine::CifType::Unknown ? false : nullptr != cifBlockByType(targetType);
}

void InteractiveObject::updateCif() { }

void InteractiveObject::moveSilentlyBy(const QPointF &shift)
{
    if (shift.isNull())
        return;

    QSignalBlocker suppressMoved(this);
    moveBy(shift.x(), shift.y());
}

/*!
   Returns the content rect of the msc chart item
 */
QRectF InteractiveObject::getChartContentBox() const
{
    // The chart item should be the first one, when AscendingOrder is used
    for (QGraphicsItem *item : scene()->items(Qt::AscendingOrder)) {
        auto chartItem = dynamic_cast<msc::ChartItem *>(item);
        if (chartItem) {
            return chartItem->contentRect();
        }
    }
    return QRectF();
}

/*!
   Returns the outer bounding box of the chart (the frame around the box)
 */
QRectF InteractiveObject::getChartBox() const
{
    // The chart item should be the first one, when AscendingOrder is used
    for (QGraphicsItem *item : scene()->items(Qt::AscendingOrder)) {
        auto chartItem = dynamic_cast<msc::ChartItem *>(item);
        if (chartItem) {
            return chartItem->boundingRect();
        }
    }
    return QRectF();
}

}
