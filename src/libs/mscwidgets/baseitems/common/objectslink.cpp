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

#include "objectslink.h"

#include "baseitems/interactiveobject.h"
#include "utils.h"

namespace msc {

/*!
  \class msc::ObjectsLink
  \brief Handler for two ObjectAnchor instances ("start" and "end").

  \inmodule MscWidgets

*/

ObjectsLink::ObjectsLink(QObject *parent)
    : QObject(parent)
    , m_anchorStart(new ObjectAnchor(this))
    , m_anchorEnd(new ObjectAnchor(this))
{
}

ObjectsLink::~ObjectsLink() { }

ObjectAnchor *ObjectsLink::source() const
{
    return m_anchorStart;
}

ObjectAnchor *ObjectsLink::target() const
{
    return m_anchorEnd;
}

QPointF ObjectsLink::linkCenter() const
{
    return utils::lineCenter({ m_anchorStart->point(), m_anchorEnd->point() });
}

QPointF ObjectsLink::makeLink(InteractiveObject *source, const QPointF &sourceAnchorPoint, InteractiveObject *target,
        const QPointF &targetAnchorPoint)
{
    bool sourceChanged(false), targetChanged(false);

    {
        QSignalBlocker block(this);
        sourceChanged = replaceSource(source, sourceAnchorPoint, ObjectAnchor::Snap::NoSnap);
        targetChanged = replaceTarget(target, targetAnchorPoint, ObjectAnchor::Snap::NoSnap);
    }

    // jsut one notification is enough:
    if (sourceChanged || targetChanged)
        notifyChanged(sourceChanged ? m_anchorStart : m_anchorEnd);

    return linkCenter();
}

bool ObjectsLink::replaceSource(InteractiveObject *source, const QPointF &anchorPoint, ObjectAnchor::Snap snap)
{
    return replaceObj(m_anchorStart, source, anchorPoint, snap);
}

bool ObjectsLink::replaceTarget(InteractiveObject *target, const QPointF &anchorPoint, ObjectAnchor::Snap snap)
{
    return replaceObj(m_anchorEnd, target, anchorPoint, snap);
}

bool ObjectsLink::replaceObj(
        ObjectAnchor *anchor, InteractiveObject *toObject, const QPointF &toPoint, ObjectAnchor::Snap snap)
{
    return anchor->replace(toObject, toPoint, snap);
}

void ObjectsLink::notifyChanged(ObjectAnchor *anchor) const
{
    const bool isStart = anchor == m_anchorStart;

    if (isStart)
        Q_EMIT sourceAnchorChanged(anchor->point());
    else
        Q_EMIT targetAnchorChanged(anchor->point());
}

} // ns msc
